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
#include "xmlutil.h"
#include <configuration/LoaderConfiguration.h>
#include <wdbLogHandler.h>
#include <libxml++/libxml++.h>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <stdexcept>



WdbSaveSpecificationFactory::WdbSaveSpecificationFactory(const locationforecast::LoaderConfiguration & options) :
	options_(options)
{
	const boost::filesystem::path & configFile = options.translation().translationConfiguration;

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

	translations_[xmlName] = ConfigurationElement::get(wdb);
}

WdbSaveSpecificationFactory::~WdbSaveSpecificationFactory()
{
}

bool WdbSaveSpecificationFactory::hasTranslationFor(const locationforecast::DataElement & element) const
{
	return translations_.find(element.parameter()) != translations_.end();
}


void WdbSaveSpecificationFactory::create(std::vector<wdb::load::FloatDataEntry> & out, const locationforecast::DataElement & element) const
{
	if ( not element.complete() )
		throw std::invalid_argument("incomplete data element");

	const locationforecast::LoaderConfiguration::LoadingOptions & loading = options_.loading();

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

	if ( not options_.loading().referenceTime.empty() )
	{
		BOOST_FOREACH(wdb::load::FloatDataEntry & entry, out)
			entry.referenceTime(options_.loading().referenceTime);
	}

	std::string dataProvider = "locationforecast";
	if ( not options_.loading().dataProvider.empty() )
		dataProvider = options_.loading().dataProvider;
	BOOST_FOREACH(wdb::load::FloatDataEntry & entry, out)
		entry.dataProviderName(dataProvider);
}
