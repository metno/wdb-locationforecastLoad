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

#include "LocationforecastConfiguration.h"
#include "../LocationForecastException.h"
#include <wdbLogHandler.h>
#include <boost/filesystem.hpp>
#ifdef BOOST_XML_PARSE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#else
#include <libxml++/libxml++.h>
#endif


namespace locationforecast
{

LocationforecastConfiguration::LocationforecastConfiguration(const boost::filesystem::path & configuration)
{
#ifdef BOOST_XML_PARSE
	if ( ! exists(configuration) )
		throw NoSuchFile("Configuration file " + configuration.string() + "does not exist");
	if ( is_directory(configuration) )
		throw FileIsDirectory("Configuration file " + configuration.string() + " is a directory");

	try
	{
		using boost::property_tree::ptree;
		ptree pt;
		read_xml(configuration.string(), pt);

		auto baseUrl = pt.get_child("locationforecastLoad.configuration.locationforecast.source");
		if ( baseUrl.size() == 1 )
			baseUrl_ = baseUrl.front().second.data();
		if ( baseUrl.size() > 1 )
			throw ParseException("Many locationforecast/source elements in configuration");

		for ( const ptree::value_type & data : pt.get_child("locationforecastLoad.configuration") )
		{
			if ( data.first == "data" )
			{
				std::string name = data.second.get<std::string>("<xmlattr>.name");
				std::string unit = data.second.get("<xmlattr>.unit", std::string());
				parameterUnits_[name] = unit;
				handlers_[name] = ElementHandler::get(name, unit);
			}
		}
	}
	catch ( std::exception & e )
	{
		throw ParseException(e.what());
	}
#else
	if ( ! exists(configuration) )
		throw NoSuchFile("Configuration file " + configuration.string() + "does not exist");
	if ( is_directory(configuration) )
		throw FileIsDirectory("Configuration file " + configuration.string() + " is a directory");

	xmlpp::DomParser parser;
	parser.parse_file(configuration.string());
	if ( ! parser )
		throw ParseException("Error when parsing config file");

	const xmlpp::Node * root = parser.get_document()->get_root_node();

	xmlpp::NodeSet sourceNodes = root->find("/locationforecastLoad/configuration/locationforecast/source");
	if ( sourceNodes.size() == 1 )
	{
		const xmlpp::Element * element = dynamic_cast<const xmlpp::Element *>(sourceNodes.front());
		baseUrl_ = element->get_attribute_value("url");
		//baseUrl_ = "http://api.met.no/weatherapi/locationforecast/1.8/?lat=$LATITUDE$;lon=$LONGITUDE$";
	}
	else if ( not sourceNodes.empty() )
		throw ParseException("Many locationforecast/source elements in configuration");

	for ( const xmlpp::Node * node : root->find("/locationforecastLoad/configuration/data"))
	{
		const xmlpp::Element * element = dynamic_cast<const xmlpp::Element *>(node);
		if ( ! element ) // should never happen
			continue;
		std::string name = element->get_attribute_value("name");
		std::string unit = element->get_attribute_value("unit");
		parameterUnits_[name] = unit;
		handlers_[name] = ElementHandler::get(name, unit);
	}
#endif
}

LocationforecastConfiguration::~LocationforecastConfiguration()
{
}

const ElementHandler::Ptr & LocationforecastConfiguration::getHandler(const std::string & parameter)
{
	ElementHandler::Ptr & handler = handlers_[parameter];
	if ( ! handler )
	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.locationforecastLoad.xmlparse" );
		log.warnStream() << "No handler for parameter <" << parameter << ">. Trying to parse contents anyway.";
		handler = ElementHandler::get(parameter);
	}
	return handler;
}

} /* namespace locationforecast */
