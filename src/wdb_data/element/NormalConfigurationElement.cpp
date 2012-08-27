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

#include "NormalConfigurationElement.h"
#ifdef BOOST_XML_PARSE
#include <boost/property_tree/ptree.hpp>
#else
#include "../xmlutil.h"
#include <libxml++/libxml++.h>
#endif


#ifdef BOOST_XML_PARSE

	NormalConfigurationElement::NormalConfigurationElement(const boost::property_tree::ptree & wdb)
	{
		valueParameterName = wdb.get<std::string>("valueparameter.<xmlattr>.name");
		levelParameterName = wdb.get<std::string>("levelparameter.<xmlattr>.name");
		levelFrom = wdb.get<float>("levelparameter.<xmlattr>.from", 0);
		levelTo = wdb.get<float>("levelparameter.<xmlattr>.to", 0);
		valueCoefficient = wdb.get<float>("conversion.<xmlattr>.coefficient", 1);
		valueConstant = wdb.get<float>("conversion.<xmlattr>.constant", 0);
	}

#else

NormalConfigurationElement::NormalConfigurationElement(const xmlpp::Element & wdb) :
		valueCoefficient(1), valueConstant(0)
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

#endif

void NormalConfigurationElement::create(std::vector<WdbSaveSpecification> & out, const locationforecast::DataElement & element) const
{
	WdbSaveSpecification ret;

	ret.setValue(valueConstant + (element.value() * valueCoefficient));
	ret.setLocation(element.location());
	ret.setReferenceTime(element.referenceTime());
	ret.setValidFrom(element.validFrom());
	ret.setValidTo(element.validTo());
	ret.setValueParameter(valueParameterName);
	ret.setLevelParameter(levelParameterName);
	ret.setLevelFrom(levelFrom);
	ret.setLevelTo(levelTo);

	out.push_back(ret);
}
