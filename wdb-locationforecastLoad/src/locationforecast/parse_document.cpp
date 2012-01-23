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

#include "parse_document.h"
#include "elementhandler/ElementHandler.h"
#include <wdbLogHandler.h>
#include <libxml++/libxml++.h>
#include <boost/foreach.hpp>
#include <iostream>
#include <map>


namespace locationforecast
{
namespace
{

ElementHandler::Data getParameterData(const xmlpp::Element & parameterElement)
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.locationforecastLoad.xmlparse" );

	const std::string parameter = parameterElement.get_name();

	static std::map<std::string, ElementHandler::Ptr> handlers;
	if ( handlers.empty() )
	{
		// TODO: move into a config file
		handlers["temperature"] = ElementHandler::get("temperature");
		handlers["windDirection"] = ElementHandler::get("windDirection", "deg");
		handlers["windSpeed"] = ElementHandler::get("windSpeed", "mps");
		handlers["humidity"] = ElementHandler::get("humidity");
		handlers["pressure"] = ElementHandler::get("pressure");
		handlers["cloudiness"] = ElementHandler::get("cloudiness", "percent");
		handlers["fog"] = ElementHandler::get("fog", "percent");
		handlers["lowClouds"] = ElementHandler::get("lowClouds", "percent");
		handlers["mediumClouds"] = ElementHandler::get("mediumClouds", "percent");
		handlers["highClouds"] = ElementHandler::get("highClouds", "percent");
		handlers["precipitation"] = ElementHandler::get("precipitation");
		handlers["symbol"] = ElementHandler::get("symbol", "number");
	}
	ElementHandler::Ptr handler = handlers[parameter];
	if ( ! handler )
	{
		log.warnStream() << "No handler for parameter <" << parameter << ">. Trying to parse contents anyway.";
		handler = ElementHandler::get(parameter);
	}
	return handler->extract(parameterElement);
}

void parseParameter(DataElement workingData, std::vector<DataElement> & out, const xmlpp::Node * node)
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.locationforecastLoad.xmlparse" );

	const xmlpp::Element * element = dynamic_cast<const xmlpp::Element *>(node);
	if ( element )
	{
		try
		{
			ElementHandler::Data parameterData = getParameterData(* element);
			workingData.parameter(parameterData.parameter);
			workingData.value(parameterData.value);
		}
		catch ( ElementHandler::ExtractionError & e )
		{
			log.warnStream() << "Unable to handle parameter <" << element->get_name() << ": " << e.what();
		}
		if ( workingData.complete() )
			out.push_back(workingData);
		else // should never happen
			log.errorStream() << "Internal error: Trying to save data with missing elements";
	}
}

void parseLocation(DataElement workingData, std::vector<DataElement> & out, const xmlpp::Node * node)
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.locationforecastLoad.xmlparse" );

	const xmlpp::Element * locationElement = dynamic_cast<const xmlpp::Element *>(node);
	if ( ! locationElement )
		return;

	if ( locationElement->get_name() != "location" )
	{
		log.warnStream() << "Unexepected element in data: " << locationElement->get_path();
		return;
	}

	std::ostringstream location;
	location << "POINT(";
	location << locationElement->get_attribute_value("longitude");
	location << ' ';
	location << locationElement->get_attribute_value("latitude");
	location << ')';
	workingData.location(location.str());

	BOOST_FOREACH(const xmlpp::Node * node, node->get_children() )
		parseParameter(workingData, out, node);
}

void parseTime(DataElement workingData, std::vector<DataElement> & out, const xmlpp::Node * node)
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.locationforecastLoad.xmlparse" );

	const xmlpp::Element * timeElement = dynamic_cast<const xmlpp::Element *>(node);
	if ( ! timeElement )
		return;

	if ( timeElement->get_name() != "time" )
	{
		log.warnStream() << "Unexepected element in data: " << timeElement->get_path();
		return;
	}

	std::string validFrom = timeElement->get_attribute_value("from");
	std::string validTo = timeElement->get_attribute_value("to");

	workingData.validFrom(validFrom);
	workingData.validTo(validTo);

	BOOST_FOREACH(const xmlpp::Node * node, node->get_children() )
		parseLocation(workingData, out, node);
}

void parseProduct(std::vector<DataElement> & out, const xmlpp::Node * node)
{
	BOOST_FOREACH(const xmlpp::Node * time, node->get_children() )
	{
		DataElement workingData;
		parseTime(workingData, out, time);
	}
}
}

void parse_document(std::istream & s, std::vector<DataElement> & out)
{
	xmlpp::DomParser parser;
	parser.parse_stream(s);
	if ( parser )
	{
		const xmlpp::Node * root = parser.get_document()->get_root_node();

		xmlpp::Node::NodeList products = root->get_children("product");

		BOOST_FOREACH(const xmlpp::Node * product, products)
			parseProduct(out, product);
	}
}

}
