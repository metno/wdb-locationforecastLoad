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

#include "Document.h"
#include "TimeRange.h"
#include <wdbLogHandler.h>
#include <libxml++/libxml++.h>
#include <curl/curl.h>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>


namespace locationforecast
{

Document::Document(std::istream & sin, const boost::filesystem::path & configuration)
{
	parseConfiguration_(configuration);
	parse_(sin, elements_);
}

namespace
{
size_t writeToStream(char * ptr, size_t size, size_t nmemb, void *userdata)
{
	std::ostream * s = (std::ostream *) userdata;

	for ( int i = 0; i < size * nmemb; ++ i )
		(*s) << ptr[i];

	return size * nmemb;
}
}

Document::Document(const std::string & url, const boost::filesystem::path & configuration)
{
	parseConfiguration_(configuration);

	std::stringstream data;

	curl_global_init(CURL_GLOBAL_NOTHING);
	CURL * curl = curl_easy_init();
	if ( ! curl )
		throw std::runtime_error("Unable to initalize web handler");
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToStream);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) & data);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "wdb-locationforecastLoad/"VERSION);
	CURLcode res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	parse_(data, elements_);
}

Document::Document(const boost::filesystem::path & file, const boost::filesystem::path & configuration)
{
	if ( ! exists(file) )
		throw NoSuchFile(file.string() + "does not exist");
	if ( is_directory(file) )
		throw FileIsDirectory(file.string() + " is a directory");

	parseConfiguration_(configuration);

	boost::filesystem::ifstream xmlStream(file);
	parse_(xmlStream, elements_);
}

Document::~Document()
{
}

void Document::parseConfiguration_(const boost::filesystem::path & configuration)
{
	if ( ! exists(configuration) )
		throw NoSuchFile("Configuration file " + configuration.string() + "does not exist");
	if ( is_directory(configuration) )
		throw FileIsDirectory("Configuration file " + configuration.string() + " is a directory");

	xmlpp::DomParser parser;
	parser.parse_file(configuration.string());
	if ( ! parser )
		throw ParseException("Error when parsing config file");

	const xmlpp::Node * root = parser.get_document()->get_root_node();

	BOOST_FOREACH(const xmlpp::Node * node, root->find("/locationforecastLoad/configuration/data"))
	{
		const xmlpp::Element * element = dynamic_cast<const xmlpp::Element *>(node);
		if ( ! element ) // should never happen
			continue;
		std::string name = element->get_attribute_value("name");
		std::string unit = element->get_attribute_value("unit");
		parameterUnits_[name] = unit;
		handlers_[name] = ElementHandler::get(name, unit);
	}
}

ElementHandler::Data Document::getParameterData_(const xmlpp::Element & parameterElement)
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.locationforecastLoad.xmlparse" );

	const std::string parameter = parameterElement.get_name();

	ElementHandler::Ptr & handler = handlers_[parameter];
	if ( ! handler )
	{
		log.warnStream() << "No handler for parameter <" << parameter << ">. Trying to parse contents anyway.";
		handler = ElementHandler::get(parameter);
	}
	return handler->extract(parameterElement);
}

void Document::parseParameter_(DataElement workingData, std::vector<DataElement> & out, const xmlpp::Node * node)
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

void Document::parseLocation_(DataElement workingData, std::vector<DataElement> & out, const xmlpp::Node * node)
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
		parseParameter_(workingData, out, node);
}

void Document::parseTime_(DataElement workingData, std::vector<DataElement> & out, const xmlpp::Node * node)
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
		parseLocation_(workingData, out, node);
}

void Document::parse_(std::istream & s, std::vector<DataElement> & out)
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.locationforecastLoad.xmlparse" );

	xmlpp::DomParser parser;
	parser.parse_stream(s);
	if ( parser )
	{
		const xmlpp::Node * root = parser.get_document()->get_root_node();

		typedef std::map<TimeRange, std::string> ReferenceTimesForValidTimes;
		ReferenceTimesForValidTimes referenceTimes;
		BOOST_FOREACH(const xmlpp::Node * modelNode, root->find("/weatherdata/meta/model"))
		{
			const xmlpp::Element * model = dynamic_cast<const xmlpp::Element *>(modelNode);
			if ( ! model )
				continue;
			std::string from = model->get_attribute_value("from");
			std::string to = model->get_attribute_value("to");
			std::string referenceTime = model->get_attribute_value("termin");

			referenceTimes[TimeRange(from, to)] = referenceTime;
		}

		std::vector<DataElement> dataOut;
		BOOST_FOREACH( const xmlpp::Node * node, root->find("/weatherdata/product/time") )
		{
			DataElement workingData;
			parseTime_(workingData, dataOut, node);
		}

		BOOST_FOREACH(DataElement & element, dataOut)
			BOOST_FOREACH(const ReferenceTimesForValidTimes::value_type & refFromValid, referenceTimes)
				if ( refFromValid.first.encloses(element.validTo()) )
				{
					element.referenceTime(refFromValid.second);
					if ( element.complete() )
						out.push_back(element);
					else
						log.errorStream() << "Internal error: unable to fully understand data with parameter <" << element.parameter() << '>';
				}
	}
}


}
