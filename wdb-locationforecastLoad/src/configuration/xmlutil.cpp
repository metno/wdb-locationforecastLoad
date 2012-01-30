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

#include "xmlutil.h"
#include <libxml++/libxml++.h>
#include <stdexcept>

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


