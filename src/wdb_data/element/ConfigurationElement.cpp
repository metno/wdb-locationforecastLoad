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


#include "ConfigurationElement.h"
#include "NormalConfigurationElement.h"
#include "IgnoreConfigurationElement.h"
#ifdef BOOST_XML_PARSE
#include <boost/property_tree/ptree.hpp>
#else
#include <libxml++/libxml++.h>
#endif

#ifdef BOOST_XML_PARSE

ConfigurationElement::Ptr ConfigurationElement::get(const boost::property_tree::ptree & wdb)
{
	if ( wdb.get<std::string>("<xmlattr>.process", std::string()) == "ignore" )
		return Ptr(new IgnoreConfigurationElement);
	return Ptr(new NormalConfigurationElement(wdb));
}

#else

ConfigurationElement::Ptr ConfigurationElement::get(const xmlpp::Element & wdb)
{
	std::string processInstruction = wdb.get_attribute_value("process");
	if ( processInstruction == "ignore" )
		return Ptr(new IgnoreConfigurationElement());

	return Ptr(new NormalConfigurationElement(wdb));
}

#endif
