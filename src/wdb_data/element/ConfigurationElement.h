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


#ifndef CONFIGURATIONELEMENT_H_
#define CONFIGURATIONELEMENT_H_

#include "../WdbSaveSpecification.h"
#include <locationforecast/DataElement.h>
#include <boost/shared_ptr.hpp>
#include <vector>

#ifdef BOOST_XML_PARSE
#include <boost/property_tree/ptree_fwd.hpp>
#else
namespace xmlpp
{
class Element;
}
#endif

class ConfigurationElement
{
public:
	virtual ~ConfigurationElement() {}

	virtual void create(std::vector<WdbSaveSpecification> & out, const locationforecast::DataElement & element) const =0;

	typedef boost::shared_ptr<ConfigurationElement> Ptr;

#ifdef BOOST_XML_PARSE
	static Ptr get(const boost::property_tree::ptree & wdb);
#else
	static Ptr get(const xmlpp::Element & wdb);
#endif
};


#endif /* CONFIGURATIONELEMENT_H_ */
