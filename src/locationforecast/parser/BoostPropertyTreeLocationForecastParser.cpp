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

#include "BoostPropertyTreeLocationForecastParser.h"
#include <configuration/LocationforecastConfiguration.h>
#include "../LocationForecastException.h"
#include "../TimeRange.h"
#include <wdbLogHandler.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <map>
#include <string>


namespace locationforecast
{

BoostPropertyTreeLocationForecastParser::BoostPropertyTreeLocationForecastParser(LocationforecastConfiguration & configuration) :
		configuration_(configuration)
{
}

BoostPropertyTreeLocationForecastParser::~BoostPropertyTreeLocationForecastParser()
{
}

BoostPropertyTreeLocationForecastParser::DataList BoostPropertyTreeLocationForecastParser::parse(std::istream & document) const
{
	DataList out;

	WDB_LOG & log = WDB_LOG::getInstance( "wdb.locationforecastLoad.xmlparse" );

	log.debug("Starting parse");

	try
	{
		using boost::property_tree::ptree;
		ptree root;
		read_xml(document, root);

		const ptree & weatherdata = root.get_child("weatherdata");

		typedef std::map<TimeRange, std::string> ReferenceTimesForValidTimes;
		ReferenceTimesForValidTimes referenceTimes;

		const ptree & metaTag = weatherdata.get_child("meta");
		for ( ptree::const_iterator meta = metaTag.begin(); meta != metaTag.end(); ++ meta )
		{
			if ( meta->first == "model")
			{
				ptree model = meta->second;
				std::string name = model.get<std::string>("<xmlattr>.name");
				if ( configuration_.shouldProcessModel(name) )
				{
					std::string from = model.get<std::string>("<xmlattr>.from");
					std::string to = model.get<std::string>("<xmlattr>.to");
					std::string referenceTime = model.get<std::string>("<xmlattr>.termin");
					referenceTimes[TimeRange(from, to)] = referenceTime;
				}
			}
		}

		std::vector<DataElement> dataOut;
		const ptree & productTag = weatherdata.get_child("product");
		for ( ptree::const_iterator product = productTag.begin(); product != productTag.end(); ++ product )
			if ( product->first == "time" )
			{
				DataElement workingData;
				parseTime_(workingData, dataOut, product->second);
			}

		for ( std::vector<DataElement>::iterator element = dataOut.begin(); element != dataOut.end(); ++ element )
			for ( ReferenceTimesForValidTimes::const_iterator refFromValid = referenceTimes.begin(); refFromValid != referenceTimes.end(); ++ refFromValid )
				if ( refFromValid->first.encloses(element->validTo()) )
				{
					element->referenceTime(refFromValid->second);
					if ( element->complete() )
						out.push_back(* element);
					else
						log.errorStream() << "Internal error: unable to fully understand data with parameter <" << element->parameter() << '>';
				}
	}
	catch ( boost::property_tree::ptree_error & e )
	{
		throw ParseException(e.what());
	}

	log.debug("Parsing of document complete");

	return out;
}

void BoostPropertyTreeLocationForecastParser::parseTime_(DataElement workingData, std::vector<DataElement> & out, const boost::property_tree::ptree & node) const
{
	std::string validFrom = node.get<std::string>("<xmlattr>.from");
	std::string validTo = node.get<std::string>("<xmlattr>.to");

	workingData.validFrom(validFrom);
	workingData.validTo(validTo);

	for ( boost::property_tree::ptree::const_iterator n = node.begin(); n != node.end(); ++ n )
		if ( n->first == "location" )
			parseLocation_(workingData, out, n->second);
}

void BoostPropertyTreeLocationForecastParser::parseLocation_(DataElement workingData, std::vector<DataElement> & out, const boost::property_tree::ptree & node) const
{
	type::Point point(
			node.get<std::string>("<xmlattr>.longitude"),
			node.get<std::string>("<xmlattr>.latitude"));
	workingData.location(point);

	for ( boost::property_tree::ptree::const_iterator n = node.begin(); n != node.end(); ++ n )
		if ( n->first != "<xmlattr>" )
			parseParameter_(workingData, out, n->first, n->second);
}

void BoostPropertyTreeLocationForecastParser::parseParameter_(DataElement workingData, std::vector<DataElement> & out, const std::string & parameter, const boost::property_tree::ptree & node) const
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.locationforecastLoad.xmlparse" );

	try
	{
		ElementHandler::Ptr handler = configuration_.getHandler(parameter);
		if ( ! handler )
		{
			log.warnStream() << "No handler for parameter <" << parameter << ">. Trying to parse contents anyway.";
			handler = ElementHandler::get(parameter);
		}
		ElementHandler::Data parameterData = handler->extract(node);

		workingData.parameter(parameterData.parameter);
		workingData.value(parameterData.value);

		out.push_back(workingData);
	}
	catch ( std::exception & e )
	{
		log.warnStream() << "Unable to handle parameter <" << parameter << ": " << e.what();
	}
}

} /* namespace locationforecast */
