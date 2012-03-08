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

#include "SavingDataHandlingStrategy.h"
#include "queries.h"
#include <locationforecast/Document.h>
#include <configuration/LoaderConfiguration.h>
#include <wdbLogHandler.h>
#include <boost/foreach.hpp>

SavingDataHandlingStrategy::SavingDataHandlingStrategy(const locationforecast::LoaderConfiguration & conf) :
		connection_(conf),
		conf_(conf),
		factory_(conf)
{
}

SavingDataHandlingStrategy::~SavingDataHandlingStrategy()
{
}


void SavingDataHandlingStrategy::handle(const locationforecast::Document & document)
{
	WDB_LOG & log = WDB_LOG::getInstance("wdb.locationforecastLoad");
	log.debug("Starting to save data in database");

	std::vector<wdb::load::FloatDataEntry> toSave;

	BOOST_FOREACH(const locationforecast::Document::value_type & data, document)
		if ( factory_.hasTranslationFor(data) )
			factory_.create(toSave, data);

	if ( not document.placeName().empty() )
		BOOST_FOREACH ( wdb::load::FloatDataEntry & entry, toSave )
			entry.placeName(document.placeName());

	connection_.write(toSave);

//	SaveDataTransactor t(conf_, document);
//	connection_.perform(t);

	log.debug("Save complete");
}

SavingDataHandlingStrategy::Position SavingDataHandlingStrategy::getPosition(const std::string & placeName)
{
	WDB_LOG & log = WDB_LOG::getInstance("wdb.locationforecastLoad");

	std::ostringstream query;

	pqxx::work w(connection_);

	queries::wciBegin(w, conf_);

	query << "SELECT st_x(placegeometry), st_y(placegeometry) FROM wci.getplacepoint('" << w.esc(placeName) << "')";
	log.debug(query.str());
	pqxx::result r = w.exec(query.str());

	if ( r.empty() )
		throw std::runtime_error(placeName + ": no such place in database");
	if ( r.size() > 1 ) // should never happen
		throw std::runtime_error(placeName + " returned many points!");

	Position ret;
	ret.longitude = r[0][0].as<float>();
	ret.latitude = r[0][1].as<float>();

	return ret;
}
