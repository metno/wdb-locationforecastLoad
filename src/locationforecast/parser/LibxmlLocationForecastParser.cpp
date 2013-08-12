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

#include "LibxmlLocationForecastParser.h"
#include "../TimeRange.h"
#include "../LocationForecastException.h"
#include <wdbLogHandler.h>
#include <libxml++/libxml++.h>
#include <boost/lexical_cast.hpp>

namespace locationforecast
{

LibxmlLocationForecastParser::LibxmlLocationForecastParser(LocationforecastConfiguration & configuration) :
		configuration_(configuration)
{
}

LibxmlLocationForecastParser::~LibxmlLocationForecastParser()
{
}

LibxmlLocationForecastParser::DataList LibxmlLocationForecastParser::parse(std::istream & document) const
{
	DataList out;

	WDB_LOG & log = WDB_LOG::getInstance( "wdb.locationforecastLoad.xmlparse" );

	log.debug("Starting parse");

	xmlpp::DomParser parser;
	parser.parse_stream(document);
	if ( parser )
	{
		const xmlpp::Node * root = parser.get_document()->get_root_node();

		if ( root->find("/weatherdata").size() != 1 )
			throw ParseException("Invalid document");

		typedef std::map<TimeRange, std::string> ReferenceTimesForValidTimes;
		ReferenceTimesForValidTimes referenceTimes;
		const xmlpp::NodeSet & modelSet = root->find("/weatherdata/meta/model");
		for ( xmlpp::NodeSet::const_iterator it = modelSet.begin(); it != modelSet.end(); ++ it )
		{
			const xmlpp::Element * model = dynamic_cast<const xmlpp::Element *>(* it);
			if ( ! model )
				continue;

			std::string name = model->get_attribute_value("name");
			if ( configuration_.shouldProcessModel(name) )
			{
				std::string from = model->get_attribute_value("from");
				std::string to = model->get_attribute_value("to");
				std::string referenceTime = model->get_attribute_value("termin");

				referenceTimes[TimeRange(from, to)] = referenceTime;
			}
		}

		std::vector<DataElement> dataOut;
		const xmlpp::NodeSet & nodes = root->find("/weatherdata/product/time");
		//for ( const xmlpp::Node * node : root->find("/weatherdata/product/time") )
		for ( xmlpp::NodeSet::const_iterator it = nodes.begin(); it != nodes.end(); ++ it )
		{
			DataElement workingData;
			parseTime_(workingData, dataOut, * it);
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
	else
		throw ParseException("Error when initializing xml parser");

	log.debug("Parsing of document complete");

	return out;
}

ElementHandler::Data LibxmlLocationForecastParser::getParameterData_(const xmlpp::Element & parameterElement) const
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.locationforecastLoad.xmlparse" );

	const std::string parameter = parameterElement.get_name();

	ElementHandler::Ptr handler = configuration_.getHandler(parameter);
	if ( ! handler )
	{
		log.warnStream() << "No handler for parameter <" << parameter << ">. Trying to parse contents anyway.";
		handler = ElementHandler::get(parameter);
	}
	return handler->extract(parameterElement);
}

void LibxmlLocationForecastParser::parseParameter_(DataElement workingData, std::vector<DataElement> & out, const xmlpp::Node * node) const
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.locationforecastLoad.xmlparse" );

	const xmlpp::Element * element = dynamic_cast<const xmlpp::Element *>(node);
	if ( element )
	{
		try
		{
			ElementHandler::Data parameterData = getParameterData_(* element);
			workingData.parameter(parameterData.parameter);
			workingData.value(parameterData.value);
		}
		catch ( ElementHandler::ExtractionError & e )
		{
			log.warnStream() << "Unable to handle parameter <" << element->get_name() << ": " << e.what();
		}
		out.push_back(workingData);
	}
}

void LibxmlLocationForecastParser::parseLocation_(DataElement workingData, std::vector<DataElement> & out, const xmlpp::Node * node) const
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

	type::Point point(locationElement->get_attribute_value("longitude"),
			locationElement->get_attribute_value("latitude"));
	workingData.location(point);

	const xmlpp::Node::NodeList & children = node->get_children();
	//for ( const xmlpp::Node * node : children )
	for ( xmlpp::Node::NodeList::const_iterator it = children.begin(); it != children.end(); ++ it )
		parseParameter_(workingData, out, * it);
}

void LibxmlLocationForecastParser::parseTime_(DataElement workingData, std::vector<DataElement> & out, const xmlpp::Node * node) const
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

	const xmlpp::Node::NodeList & children = node->get_children();
	//for ( const xmlpp::Node * node : children )
	for ( xmlpp::Node::NodeList::const_iterator it = children.begin(); it != children.end(); ++ it )
		parseLocation_(workingData, out, * it);
}

} /* namespace locationforecast */
