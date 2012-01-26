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
#include <locationforecast/Document.h>
#include <wdb/LoaderConfiguration.h>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

SaveDataTransactor::SaveDataTransactor(const wdb::load::LoaderConfiguration & conf, const locationforecast::Document & document) :
	conf_(conf),
	document_(document)
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


void SaveDataTransactor::operator () (pqxx::work & transaction)
{
	Escaper escape(transaction);

	transaction.exec("SELECT wci.begin('" + escape(conf_.database().user) + "')");

	BOOST_FOREACH(const locationforecast::Document::value_type & element, document_)
	{
		if ( specificationFactory_.hasTranslationFor(element) )
		{
			const WdbSaveSpecification & spec = specificationFactory_.create(element);
			const std::string writeQuery = spec.getReadQuery(escape, getPlaceName_(transaction, spec.location()));
			//std::cout << writeQuery << std::endl;
			transaction.exec(writeQuery);
		}
	}
//#error jeg finner ikke dataen igjen i wci_int.floatvalue, selv om de er i wdb_int.floatvalue
}

const std::string & SaveDataTransactor::getPlaceName_(pqxx::work & transaction, const std::string & geometryPoint)
{
	std::string & placeName = nameFromGeometry_[geometryPoint];
	if ( placeName.empty() )
	{
		const std::string safePoint = transaction.esc(geometryPoint);
		std::string query = "SELECT * FROM wci.getNameforGeometry('" + safePoint + "')";
		pqxx::result r = transaction.exec(query);
		if ( r.empty() )
		{
			if ( conf_.loading().loadPlaceDefinition )
			{
				std::string newPlaceName = boost::to_lower_copy(geometryPoint);

				std::ostringstream query;
				query << "SELECT wci.addPlacePoint('"
						<< transaction.esc(newPlaceName)
						<< "', st_geomfromtext('"
						<< safePoint
						<<"', 4030))";
				transaction.exec(query.str());
				placeName = newPlaceName;
			}
			else
				throw std::runtime_error("No existing place definition for the given geometry");
		}
		else if ( r.size() > 1 )
			throw std::runtime_error("Many names for same geometry!");
		else
			placeName = r[0][0].as<std::string>();
	}
	return placeName;
}
