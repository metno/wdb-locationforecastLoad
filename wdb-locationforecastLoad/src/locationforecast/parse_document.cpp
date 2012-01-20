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

#include "parse_document.h"
#include <libxml++/libxml++.h>
#include <boost/foreach.hpp>
#include <iostream>


namespace locationforecast
{

void parse_document(std::istream & s, std::vector<DataElement> & out)
{
	xmlpp::DomParser parser;
	parser.parse_stream(s);
	if ( parser )
	{
		const xmlpp::Node * root = parser.get_document()->get_root_node();

		xmlpp::Node::NodeList products = root->get_children("product");

		BOOST_FOREACH(const xmlpp::Node * product, products)
		{
			BOOST_FOREACH(const xmlpp::Node * time, product->get_children() )
			{
				BOOST_FOREACH(const xmlpp::Node * location, time->get_children() )
				{
					BOOST_FOREACH(const xmlpp::Node * node, location->get_children() )
					{
						const xmlpp::Element * element = dynamic_cast<const xmlpp::Element *>(node);
						if ( element )
						{
//							std::cout << element->get_name() << ":\t";
//							BOOST_FOREACH(const xmlpp::Attribute * attribute, element->get_attributes())
//								std::cout << attribute->get_name() << '=' << attribute->get_value() << ' ';
//							std::cout << std::endl;
							DataElement newElement;
							out.push_back(newElement);
						}
					}
				}
			}
		}
	}
}

}




