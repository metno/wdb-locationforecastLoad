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
#include <locationforecast/DataElement.h>
#include <boost/filesystem/path.hpp>
#include <map>


namespace xmlpp
{
class Element;
}

class WdbSaveSpecificationFactory
{
public:
	explicit WdbSaveSpecificationFactory(const boost::filesystem::path & configFile);
	~WdbSaveSpecificationFactory();

	bool hasTranslationFor(const locationforecast::DataElement & element) const;

	WdbSaveSpecification create(const locationforecast::DataElement & element) const;

private:
	class Configuration
	{
	public:
		Configuration() {}
		Configuration(const xmlpp::Element & wdb);

		std::string valueParameterName;
		std::string levelParameterName;
		float levelFrom;
		float levelTo;

		float valueCoefficient;
		float valueConstant;
	};

	void setup_(const xmlpp::Element & rootNode);
	void addParameter_(const xmlpp::Element & parameterNode);

	typedef std::map<std::string, Configuration> ParameterTranslation;
	ParameterTranslation translations_;
};


#endif /* WDBSAVESPECIFICATIONFACTORY_H_ */
