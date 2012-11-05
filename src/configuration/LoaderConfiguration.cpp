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

#include "LoaderConfiguration.h"
#include "LocationforecastConfiguration.h"
#include <algorithm>

namespace locationforecast
{

LoaderConfiguration::LoaderConfiguration() :
		wdb::load::LoaderConfiguration("locationforecast"),
		locationforecastConfiguration_(0)
{
	using namespace boost::program_options;

	options_description translation("Parameter translation");
	translation.add_options()
			("translation",
			value(& translation_.translationConfiguration)->default_value(SYSCONFDIR"/locationforecastLoad.conf.xml"),
			"Read parameter translation configuration from the given file"
			);

	configOptions().add( translation );
	shownOptions().add( translation );

	options_description models("Models");
	models.add_options()
			("models,m",
			value(& modelsToLoad_),
			"Only load the models with the given name(s) in the meta.model.name tag"
			);

	configOptions().add( models );
	shownOptions().add( models );

}

LoaderConfiguration::~LoaderConfiguration()
{
	delete locationforecastConfiguration_;
}

bool LoaderConfiguration::shouldProcessModel(const std::string & modelName) const
{
	if ( modelsToLoad_.empty() )
		return true;

	return std::find(modelsToLoad_.begin(), modelsToLoad_.end(), modelName) != modelsToLoad_.end();
}

LocationforecastConfiguration & LoaderConfiguration::locationforecastConfiguration() const
{
	if ( ! locationforecastConfiguration_ )
		locationforecastConfiguration_ = new LocationforecastConfiguration(* this);
	return * locationforecastConfiguration_;
}

}
