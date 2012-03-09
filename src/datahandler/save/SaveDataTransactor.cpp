/*
 wdb-locationforecastLoad

 Copyright (C) 2012 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 E-mail: post@met.no

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA  02110-1301, USA
 */

#include "SaveDataTransactor.h"
#include "queries.h"
#include <locationforecast/Document.h>
#include <configuration/LoaderConfiguration.h>
#include <wdbLogHandler.h>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/scoped_ptr.hpp>
#include <iostream>

SaveDataTransactor::SaveDataTransactor(const locationforecast::LoaderConfiguration & conf, const locationforecast::Document & document, OutputMode outputMode) :
	conf_(conf),
	document_(document),
	specificationFactory_(conf),
	outputMode_(outputMode)
{
}

SaveDataTransactor::~SaveDataTransactor()
{
}

namespace
{
	class Escaper
	{
	public:
		explicit Escaper(const pqxx::work & transaction) : transaction_(transaction) {}

		std::string operator () (const std::string & s) const
		{
			return transaction_.esc(s);
		}

	private:
		const pqxx::work & transaction_;
	};
}

struct DataproviderIndicator
{
	DataproviderIndicator()
	{
		std::cout << "locationforecast\n";
	}
	~DataproviderIndicator()
	{
		std::cout << std::endl;
	}
};

void SaveDataTransactor::operator () (pqxx::work & transaction)
{
	WDB_LOG & log = WDB_LOG::getInstance("wdb.locationforecastLoad.query");

	queries::wciBegin(transaction, conf_);

	Escaper escape(transaction);

	boost::scoped_ptr<DataproviderIndicator> indicator(outputMode_ == FastLoad ? new DataproviderIndicator : 0);

	BOOST_FOREACH(const locationforecast::Document::value_type & element, document_)
	{
		if ( specificationFactory_.hasTranslationFor(element) )
		{
			std::vector<WdbSaveSpecification> saveSpecs;
			specificationFactory_.create(saveSpecs, element);
			BOOST_FOREACH(const WdbSaveSpecification & spec, saveSpecs)
			{
				if ( outputMode_ == WciWrite )
				{
					const std::string writeQuery = spec.getWriteQuery(escape, getPlaceName_(transaction, spec.location(), spec.referenceTime()));
					log.debug(writeQuery);
					transaction.exec(writeQuery);
				}
				else //if ( outputMode_ == Fastload )
				{
					spec.getFastloadText(std::cout, getPlaceName_(transaction, spec.location(), spec.referenceTime()));
				}
			}
		}
	}
}

const std::string & SaveDataTransactor::getPlaceName_(pqxx::work & transaction, const std::string & geometryPoint, const std::string & referencetime)
{
	if ( not conf_.loading().placeName.empty() )
		return getCustomPlaceName_(transaction, geometryPoint);

	WDB_LOG & log = WDB_LOG::getInstance("wdb.locationforecastLoad.query");

	std::string & placeName = nameFromGeometry_[geometryPoint];
	if ( placeName.empty() )
	{
		const std::string safePoint = transaction.esc(geometryPoint);
		const std::string rtime = transaction.esc(referencetime);
		// some code is commented out because there are lots of errors regarding place valid times in test databases
		std::string query = "SELECT * FROM wci.getNameforGeometry('" + safePoint + /*"', '" + rtime + */"')";
		log.debug(query);
		pqxx::result r = transaction.exec(query);
		if ( r.empty() )
		{
			if ( conf_.loading().loadPlaceDefinition )
			{
				std::string newPlaceName = boost::to_lower_copy(geometryPoint);
				createPlacePoint(transaction, newPlaceName, geometryPoint);
				placeName = newPlaceName;
			}
			else
				throw std::runtime_error("No existing place definition for the given geometry: - " + geometryPoint);
		}
		else if ( r.size() > 1 )
			throw std::runtime_error("Many names for same geometry: " + geometryPoint);
		else
			placeName = r[0][0].as<std::string>();
	}
	return placeName;
}

const std::string & SaveDataTransactor::getCustomPlaceName_(pqxx::work & transaction, const std::string & geometryPoint)
{
	WDB_LOG & log = WDB_LOG::getInstance("wdb.locationforecastLoad.query");
	std::string & placeName = nameFromGeometry_[geometryPoint];
	if ( placeName.empty() )
	{
		const std::string & ret = conf_.loading().placeName;

		std::ostringstream query;
		query << "SELECT st_astext(placegeometry) FROM wci.getplacepoint('"<< transaction.esc(ret) << "')";
		log.debug(query.str());
		pqxx::result r = transaction.exec(query.str());
		if ( r.empty() )
		{
			if ( not conf_.loading().loadPlaceDefinition )
				throw std::runtime_error("No existing place definition for the given geometry");
			createPlacePoint(transaction, ret, geometryPoint);
			placeName = conf_.loading().placeName;
		}
	}
	return placeName;
}

void SaveDataTransactor::createPlacePoint(pqxx::work & transaction, const std::string & name, const std::string & geometryPoint)
{
	WDB_LOG & log = WDB_LOG::getInstance("wdb.locationforecastLoad.query");
	std::ostringstream query;
	query << "SELECT wci.addPlacePoint('"
			<< transaction.esc(name)
			<< "', st_geomfromtext('"
			<< transaction.esc(geometryPoint)
			<<"', 4030))";
	log.debug(query.str());
	transaction.exec(query.str());
}
