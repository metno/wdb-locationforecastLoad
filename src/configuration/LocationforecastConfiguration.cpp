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
#include <locationforecast/LocationForecastException.h>
#include <wdbLogHandler.h>
#include <boost/filesystem.hpp>
#ifdef BOOST_XML_PARSE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#else
#include <libxml++/libxml++.h>
#include <wdb_data/xmlutil.h>
#endif


namespace locationforecast
{

LocationforecastConfiguration::LocationforecastConfiguration(const LoaderConfiguration & configuration) :
		conf_(configuration)
{
	boost::filesystem::path configFile(configuration.translation().translationConfiguration);
#ifdef BOOST_XML_PARSE
	if ( ! exists(configFile) )
		throw NoSuchFile("Configuration file " + configFile.string() + "does not exist");
	if ( is_directory(configFile) )
		throw FileIsDirectory("Configuration file " + configFile.string() + " is a directory");

	try
	{
		using boost::property_tree::ptree;
		ptree pt;
		read_xml(configFile.string(), pt);

		auto baseUrl = pt.get_child("locationforecastLoad.configuration.locationforecast.source");
		if ( baseUrl.size() == 1 )
			baseUrl_ = baseUrl.front().second.data();
		if ( baseUrl.size() > 1 )
			throw ParseException("Many locationforecast/source elements in configuration");

		for ( const ptree::value_type & dataElement : pt.get_child("locationforecastLoad.configuration") )
		{
			if ( dataElement.first == "data" )
			{
				const ptree & data = dataElement.second;

				std::string name = data.get<std::string>("<xmlattr>.name");
				std::string unit = data.get("<xmlattr>.unit", std::string());
				parameterUnits_[name] = unit;
				handlers_[name] = ElementHandler::get(name, unit);


				const ptree & wdb = data.get_child("wdb");
				translations_[name] = ConfigurationElement::get(wdb);

//				const xmlpp::Element & wdb = getSingleElement(* element, "wdb");
//				translations_[name] = ConfigurationElement::get(wdb);
			}
		}
	}
	catch ( std::exception & e )
	{
		throw ParseException(e.what());
	}
#else
	if ( ! exists(configFile) )
		throw NoSuchFile("Configuration file " + configFile.string() + "does not exist");
	if ( is_directory(configFile) )
		throw FileIsDirectory("Configuration file " + configFile.string() + " is a directory");

	xmlpp::DomParser parser;
	parser.parse_file(configFile.string());
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

		const xmlpp::Element & wdb = getSingleElement(* element, "wdb");
		translations_[name] = ConfigurationElement::get(wdb);
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

bool LocationforecastConfiguration::canCreateSaveSpecificationFor(const locationforecast::DataElement & element) const
{
	return translations_.find(element.parameter()) != translations_.end();
}

void LocationforecastConfiguration::createSaveSpecification(std::vector<WdbSaveSpecification> & out, const locationforecast::DataElement & element) const
{
	if ( not element.complete() )
		throw std::invalid_argument("incomplete data element");

	const locationforecast::LoaderConfiguration::LoadingOptions & loading = conf_.loading();

	if ( not loading.valueParameter.empty() )
		throw std::runtime_error("valueparameter override is not supported");
	if ( not loading.levelParameter.empty() )
		throw std::runtime_error("levelparameter override is not supported");
	if ( loading.dataVersion != -999 )
		throw std::runtime_error("dataversion override is not supported");
	if ( loading.confidenceCode != 0 )
		throw std::runtime_error("confidencecode override is not supported");

	ParameterTranslation::const_iterator find = translations_.find(element.parameter());
	if ( find == translations_.end() )
		throw std::runtime_error("Missing translation for parameter " + element.parameter());

	const ConfigurationElement::Ptr & config = find->second;
	config->create(out, element);

	if ( not conf_.loading().referenceTime.empty() )
	{
		for ( std::vector<WdbSaveSpecification>::iterator spec = out.begin(); spec != out.end(); ++ spec )
			spec->setReferenceTime(conf_.loading().referenceTime);
	}
}

} /* namespace locationforecast */
