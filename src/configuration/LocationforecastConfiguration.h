/*
 locationforecastLoad 

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

#ifndef LIBXMLCONFIGURATIONPARSER_H_
#define LIBXMLCONFIGURATIONPARSER_H_

#include "LoaderConfiguration.h"
#include <locationforecast/elementhandler/ElementHandler.h>
#include <wdb_data/WdbSaveSpecification.h>
#include <wdb_data/element/ConfigurationElement.h>
#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>
#include <map>
#include <string>

namespace locationforecast
{

class LocationforecastConfiguration : boost::noncopyable
{
public:
	LocationforecastConfiguration(const LoaderConfiguration & configuration);
	~LocationforecastConfiguration();

	std::string baseUrl() const { return baseUrl_; }
	const ElementHandler::Ptr & getHandler(const std::string & parameter);

	bool canCreateSaveSpecificationFor(const locationforecast::DataElement & element) const;

	void createSaveSpecification(std::vector<WdbSaveSpecification> & out, const locationforecast::DataElement & element) const;

	bool shouldProcessModel(const std::string & modelName) const
	{
		return conf_.shouldProcessModel(modelName);
	}

private:
	std::string baseUrl_;
	std::map<std::string, std::string> parameterUnits_;
	std::map<std::string, ElementHandler::Ptr> handlers_;

	const LoaderConfiguration & conf_;

	typedef std::map<std::string, ConfigurationElement::Ptr> ParameterTranslation;
	ParameterTranslation translations_;
};

} /* namespace locationforecast */
#endif /* LIBXMLCONFIGURATIONPARSER_H_ */
