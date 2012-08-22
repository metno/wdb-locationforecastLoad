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

#include "SimpleElementHandler.h"
#include <boost/lexical_cast.hpp>
#ifdef BOOST_XML_PARSE
#include <boost/property_tree/ptree.hpp>
#else
#include <libxml++/libxml++.h>
#endif


namespace locationforecast
{

SimpleElementHandler::SimpleElementHandler(const std::string & parameter) :
		parameter_(parameter)
{
}

SimpleElementHandler::~SimpleElementHandler()
{
}

#ifdef BOOST_XML_PARSE
ElementHandler::Data SimpleElementHandler::extract(const boost::property_tree::ptree & element) const
{
	Data ret;

	ret.parameter = parameter_;
	try
	{
		ret.value = element.get<float>("<xmlattr>.value");
		ret.unit = element.get<std::string>("<xmlattr>.unit");
	}
	catch ( std::exception & e )
	{
		throw ExtractionError(e.what());
	}
	return ret;
}
#else
ElementHandler::Data SimpleElementHandler::extract(const xmlpp::Element & element) const
{
	Data ret;

	ret.parameter = parameter_;
	std::string value = element.get_attribute_value("value");
	if ( value.empty() )
		throw ExtractionError("Missing value attribute in element");
	try
	{
		ret.value = boost::lexical_cast<float>(value);
	}
	catch ( boost::bad_lexical_cast & )
	{
		throw ExtractionError("Unable to parse value as float: " + value);
	}
	ret.unit = element.get_attribute_value("unit");
	if ( ret.unit.empty() )
		throw ExtractionError("Missing unit attribute for parameter " + ret.parameter);

	return ret;
}
#endif

} /* namespace locationforecast */
