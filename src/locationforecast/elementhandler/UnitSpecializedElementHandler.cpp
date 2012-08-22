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

#include "UnitSpecializedElementHandler.h"
#include <boost/lexical_cast.hpp>
#ifdef BOOST_XML_PARSE
#include <boost/property_tree/ptree.hpp>
#else
#include <libxml++/libxml++.h>
#endif


namespace locationforecast
{

UnitSpecializedElementHandler::UnitSpecializedElementHandler(const std::string & parameter, const std::string & unitName) :
		parameter_(parameter), unitName_(unitName)
{
}

UnitSpecializedElementHandler::~UnitSpecializedElementHandler()
{
}

#ifdef BOOST_XML_PARSE
ElementHandler::Data UnitSpecializedElementHandler::extract(const boost::property_tree::ptree & element) const
{
	Data ret;

	ret.parameter = parameter_;
	ret.unit = unitName_;
	try
	{
		ret.value = element.get<float>("<xmlattr>." + ret.unit);
	}
	catch ( std::exception & e )
	{
		throw ExtractionError(e.what());
	}

	return ret;
}
#else
ElementHandler::Data UnitSpecializedElementHandler::extract(const xmlpp::Element & element) const
{
	Data ret;

	ret.parameter = parameter_;
	ret.unit = unitName_;

	std::string value = element.get_attribute_value(ret.unit);
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
	return ret;
}
#endif

} /* namespace locationforecast */
