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

#ifndef WDBSAVESPECIFICATIONFACTORY_H_
#define WDBSAVESPECIFICATIONFACTORY_H_

#include "WdbSaveSpecification.h"
#include "element/ConfigurationElement.h"
#include <locationforecast/DataElement.h>
#include <boost/filesystem/path.hpp>
#include <map>
#include <vector>


namespace xmlpp
{
class Element;
}
namespace locationforecast
{
class LoaderConfiguration;
}

/**
 * Create WdbSaveSpecification objects, based on locationforecast::DataElement
 * objects.
 */
class WdbSaveSpecificationFactory
{
public:

	/**
	 * Initialize, and read configuration from the given object.
	 *
	 * @param options configuration for how to create the data
	 */
	explicit WdbSaveSpecificationFactory(const locationforecast::LoaderConfiguration & options);
	~WdbSaveSpecificationFactory();

	/**
	 * Do we have any specification for how to convert the given object to a
	 * WdbSaveSpecification?
	 *
	 * @param element to check if we have a translation for
	 *
	 * @return true if we have a translation for the given element
	 */
	bool hasTranslationFor(const locationforecast::DataElement & element) const;

	/**
	 * Generate WdbSaveSpecification object(s) from the given element object.
	 *
	 * @param out result data goes here, at the back
	 * @param element The base object for WdbSaveSpecification creation
	 */
	void  create(std::vector<WdbSaveSpecification> & out, const locationforecast::DataElement & element) const;

private:
	void setup_(const xmlpp::Element & rootNode);
	void addParameter_(const xmlpp::Element & parameterNode);

	const locationforecast::LoaderConfiguration & options_;
	typedef std::map<std::string, ConfigurationElement::Ptr> ParameterTranslation;
	ParameterTranslation translations_;
};


#endif /* WDBSAVESPECIFICATIONFACTORY_H_ */
