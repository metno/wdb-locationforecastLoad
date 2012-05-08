/*
    locationforecastLoad

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

#include <configuration/LoaderConfiguration.h>
#include "datahandler/list/PrintingDataHandlingStrategy.h"
#include "datahandler/save/SavingDataHandlingStrategy.h"
#include "datahandler/FastLoadDataHandlingStrategy.h"
#include "locationforecast/Document.h"
#include <wdbLogHandler.h>
#include <boost/scoped_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>


#define PROGRAM "locationforecastLoad"

void version(std::ostream & out)
{
	out << PROGRAM" (wdb) "VERSION << std::endl;
}

void help(const boost::program_options::options_description & options, std::ostream & out)
{
	version(out);
	out << '\n';

	out << "Usage: "PROGRAM" [OPTIONS] [URL_OR_PLACENAME ...]\n\n";

	out << "Loads a locationforecast xml document, as retrived from api.met.no. See\n"
			"http://api.yr.no/weatherapi/locationforecast/1.8/documentation for details \n"
			"about this format.\n\n";

	out << "There are two ways to load data into wdb via this program. Either you provide\n"
			"a complete URL for reading, or you provide a place name from the wdb database\n"
			"you are writing to. This last functionality will not work if you have chosen\n"
			"the --list option.\n\n";

	out << "Note that the output from the --list option is suitable for sunning with wdb's\n"
			"fastload program.\n\n";

	out << "Options:\n";
	out << options << std::endl;
}


DataHandlingStrategy * getHandlingStrategy(const locationforecast::LoaderConfiguration & conf)
{
	if ( conf.output().list )
		return new FastLoadDataHandlingStrategy(conf);
	else
		return new SavingDataHandlingStrategy(conf);
}

int main(int argc, char ** argv)
{
	locationforecast::LoaderConfiguration conf;

	try
	{
		conf.parse( argc, argv );
		if ( conf.general().version )
		{
			version(std::cout);
			exit(0);
		}
		if ( conf.general().help )
		{
			help(conf.shownOptions(), std::cout);
			exit(0);
		}
	}
	catch ( std::exception & e )
	{
		std::clog << e.what() << "\n\n";
		help(conf.shownOptions(), std::clog);
		exit(1);
	}

	wdb::WdbLogHandler logHandler( conf.logging().loglevel, conf.logging().logfile );
	WDB_LOG & log = WDB_LOG::getInstance("wdb.locationforecastLoad");

	boost::scoped_ptr<DataHandlingStrategy> dataHandler(getHandlingStrategy(conf));

	if ( conf.input().file.empty() )
	{
		log.info("Loading data from stdin");
		locationforecast::Document doc(std::cin, conf.translation().translationConfiguration);
		dataHandler->handle(doc);
		log.info("Loading complete");
	}
	else
	{

		BOOST_FOREACH(const std::string & url, conf.input().file)
		{
			try
			{
				log.infoStream() << "Loading " << url;

				if ( url.find_first_of("://") != std::string::npos )
				{
					locationforecast::Document doc(url, conf.translation().translationConfiguration);
					dataHandler->handle(doc);
				}
				else
				{
					DataHandlingStrategy::Position pos = dataHandler->getPosition(url);
					locationforecast::Document doc(pos.longitude, pos.latitude, conf.translation().translationConfiguration);
					dataHandler->handle(doc);
				}

				log.info("Loading complete");

				timespec s = {0, 200000000};
				timespec remaining;
				nanosleep(& s, & remaining);
			}
			catch ( std::exception & e )
			{
				log.error(e.what());
			}
		}
	}
	log.debug("done");
}

