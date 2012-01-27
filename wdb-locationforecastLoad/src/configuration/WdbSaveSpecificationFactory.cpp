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

#include "WdbSaveSpecificationFactory.h"
#include <wdbLogHandler.h>
#include <libxml++/libxml++.h>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <stdexcept>


WdbSaveSpecificationFactory::WdbSaveSpecificationFactory(const boost::filesystem::path & configFile)
{
	if ( not exists(configFile) )
		throw std::runtime_error("Unable to find configuration file: " + configFile.string());
	if ( is_directory(configFile) )
		throw std::runtime_error("Configuration file is a directory: " + configFile.string());

	xmlpp::DomParser parser;
	parser.parse_file(configFile.string());

	setup_(* parser.get_document()->get_root_node());
}

void WdbSaveSpecificationFactory::setup_(const xmlpp::Element & rootNode)
{
	BOOST_FOREACH(const xmlpp::Node * configuration, rootNode.get_children("configuration"))
	{
		const xmlpp::Element * configurationElement = dynamic_cast<const xmlpp::Element *>(configuration);
		if ( ! configurationElement )
			continue;
		BOOST_FOREACH( const xmlpp::Node * node, configurationElement->get_children("data") )
		{
			const xmlpp::Element * parameter = dynamic_cast<const xmlpp::Element *>(node);
			if ( ! parameter )
				continue;

			addParameter_(* parameter);
		}
	}
}

namespace
{
const xmlpp::Element & getSingleElement(const xmlpp::Element & parent, const std::string & name)
{
	const xmlpp::Node::NodeList & nodes = parent.get_children(name);
	if ( nodes.empty() )
		throw std::runtime_error("Unable to find " + name + " in configuration " + parent.get_path());

	const xmlpp::Node * node = nodes.front();

	if ( nodes.size() > 1 )
		throw std::runtime_error("Many elements of name " + node->get_path() + " is not allowed");

	const xmlpp::Element * element = dynamic_cast<const xmlpp::Element *>(node);
	if ( ! element )
		throw std::runtime_error(node->get_path() + " is not an element node");

	return * element;
}

const xmlpp::Element * getSingleElementIfExists(const xmlpp::Element & parent, const std::string & name)
{
	const xmlpp::Node::NodeList & nodes = parent.get_children(name);
	if ( nodes.empty() )
		return 0;

	const xmlpp::Node * node = nodes.front();

	if ( nodes.size() > 1 )
		throw std::runtime_error("Many elements of name " + node->get_path() + " is not allowed");

	const xmlpp::Element * element = dynamic_cast<const xmlpp::Element *>(node);
	if ( ! element )
		throw std::runtime_error(node->get_path() + " is not an element node");

	return element;
}

}

void WdbSaveSpecificationFactory::addParameter_(const xmlpp::Element & parameterNode)
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.load.locationforecast" );
	std::string xmlName = parameterNode.get_attribute_value("name");
	if ( xmlName.empty() )
	{
		log.warn("A parameter in configuration was ignored because it had no xml name");
		return;
	}

	const xmlpp::Element & wdb = getSingleElement(parameterNode, "wdb");
	std::string specialHandling = wdb.get_attribute_value("specialhandling");
	if ( specialHandling == "true" )
		return;

	Configuration config(wdb);

	translations_[xmlName] = config;
}

WdbSaveSpecificationFactory::~WdbSaveSpecificationFactory()
{
}

bool WdbSaveSpecificationFactory::hasTranslationFor(const locationforecast::DataElement & element) const
{
	return translations_.find(element.parameter()) != translations_.end();
}

WdbSaveSpecification WdbSaveSpecificationFactory::create(const locationforecast::DataElement & element) const
{
	if ( not element.complete() )
		throw std::invalid_argument("incomplete data element");

	ParameterTranslation::const_iterator find = translations_.find(element.parameter());
	if ( find == translations_.end() )
		throw std::runtime_error("Missing translation for parameter " + element.parameter());

	const Configuration & config = find->second;

	WdbSaveSpecification ret;
	ret.value_ = config.valueConstant + (element.value() * config.valueCoefficient);
	ret.location_ = element.location();
	ret.referenceTime_ = element.referenceTime();
	ret.validFrom_ = element.validFrom();
	ret.validTo_ = element.validTo();
	ret.valueParameter_ = config.valueParameterName;
	ret.levelParameter_ = config.levelParameterName;
	ret.levelFrom_ = config.levelFrom;
	ret.levelTo_ = config.levelTo;

	return ret;
}

namespace
{
template<typename T>
T getAttributeValue(const xmlpp::Element & element, const std::string & attributeName, T defaultValue)
{
	std::string value = element.get_attribute_value(attributeName);
	if ( value.empty() )
		return defaultValue;
	return boost::lexical_cast<T>(value);
}
}

WdbSaveSpecificationFactory::Configuration::Configuration(const xmlpp::Element & wdb) :
		valueCoefficient(1),
		valueConstant(0)
{
	const xmlpp::Element & parameter = getSingleElement(wdb, "valueparameter");
	valueParameterName = parameter.get_attribute_value("name");

	const xmlpp::Element & level = getSingleElement(wdb, "levelparameter");
	levelParameterName = level.get_attribute_value("name");


	levelFrom = getAttributeValue<float>(level, "from", 0);
	levelTo = getAttributeValue<float>(level, "to", 0);

	const xmlpp::Element * conversion = getSingleElementIfExists(wdb, "conversion");
	if ( conversion )
	{
		valueCoefficient = getAttributeValue<float>(* conversion, "coefficient", 1);
		valueConstant = getAttributeValue<float>(* conversion, "constant", 0);
	}
}
