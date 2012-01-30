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


#ifndef XMLUTIL_H_
#define XMLUTIL_H_

#include <boost/lexical_cast.hpp>
#include <libxml++/nodes/element.h>
#include <string>



template<typename T>
T getAttributeValue(const xmlpp::Element & element, const std::string & attributeName, T defaultValue)
{
	std::string value = element.get_attribute_value(attributeName);
	if ( value.empty() )
		return defaultValue;
	return boost::lexical_cast<T>(value);
}

const xmlpp::Element & getSingleElement(const xmlpp::Element & parent, const std::string & name);

const xmlpp::Element * getSingleElementIfExists(const xmlpp::Element & parent, const std::string & name);


#endif /* XMLUTIL_H_ */
