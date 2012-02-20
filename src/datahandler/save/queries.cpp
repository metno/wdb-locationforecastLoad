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


#include "queries.h"
#include <configuration/LoaderConfiguration.h>
#include <wdbLogHandler.h>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

namespace queries
{
namespace
{
int getInt(const std::string & s)
{
	return boost::lexical_cast<int>(boost::algorithm::trim_copy(s));
}
}

void wciBegin(pqxx::work & transaction, const locationforecast::LoaderConfiguration & conf)
{
	WDB_LOG & log = WDB_LOG::getInstance("wdb.locationforecastLoad.query");

	std::string dataProvider = conf.loading().dataProvider;
	if ( dataProvider.empty() )
		dataProvider = conf.loading().defaultDataProvider;


	std::ostringstream query;
	query << "SELECT wci.begin('" + transaction.esc(dataProvider) + '\'';
	if ( not conf.loading().nameSpace.empty() )
	{
		std::vector<std::string> namespaceElements;
		boost::algorithm::split(namespaceElements, conf.loading().nameSpace, boost::algorithm::is_any_of(","));

		if ( namespaceElements.size() == 1 )
			for ( int i = 0; i < 3; ++ i )
				query << ", " << getInt(namespaceElements.front());
		else if ( namespaceElements.size() == 3 )
			BOOST_FOREACH(const std::string & s, namespaceElements)
				query << ", " << getInt(s);
	}
	query << ')';

	std::string beginQuery = query.str();

	log.debug(beginQuery);
	transaction.exec(beginQuery);
}

}



