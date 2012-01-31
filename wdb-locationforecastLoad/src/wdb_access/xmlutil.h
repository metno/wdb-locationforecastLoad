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


/**
 * Read an attribute from an element node, and try to interpret it as the
 * type T.
 *
 * @throw boost::bad_lexical_cast if unable to interpret the value as the
 *                                given type
 *
 * @param element the element node to read an attribute from
 * @param attributeName The attribute to fetch
 * @param defaultValue This value is returned if the attribute did not exist,
 *                     or its value was empty
 *
 * @return interpreted value for the given attribute, or defaultValue if the
 *         attribute value was empty or did not exist.
 */
template<typename T>
T getAttributeValue(const xmlpp::Element & element, const std::string & attributeName, T defaultValue)
{
	std::string value = element.get_attribute_value(attributeName);
	if ( value.empty() )
		return defaultValue;
	return boost::lexical_cast<T>(value);
}

/**
 * Get a single element node that is a direct child of the given parent, and
 * that has the given name. An error is raised if the number of nodes with the
 * given name is different from 1.
 *
 * @throw std::runtime_error if number of nodes with the given name != 1
 *
 * @param parent Parent node for the one we want
 * @param name of the node we want
 *
 * @return The requested element node.
 */
const xmlpp::Element & getSingleElement(const xmlpp::Element & parent, const std::string & name);


/**
 * Same as getSindleElement, but instead of throwing an exception, return NULL
 * if the given node did not exist. The returned pointer should not be deleted
 *
 * @throw std::runtime_error if number of nodes with the given name > 1
 *
 * @param parent Parent node for the one we want
 * @param name of the node we want
 *
 * @return A pointer to the requested element node.
 */
const xmlpp::Element * getSingleElementIfExists(const xmlpp::Element & parent, const std::string & name);


#endif /* XMLUTIL_H_ */
