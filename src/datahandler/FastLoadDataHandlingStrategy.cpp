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

#include "FastLoadDataHandlingStrategy.h"
#include <locationforecast/Document.h>
#include <configuration/LoaderConfiguration.h>
#include <configuration/LocationforecastConfiguration.h>
#include <iostream>

FastLoadDataHandlingStrategy::FastLoadDataHandlingStrategy(const locationforecast::LoaderConfiguration & conf) :
	out_(std::cout),
	conf_(conf)
{
}

FastLoadDataHandlingStrategy::~FastLoadDataHandlingStrategy()
{
}

void FastLoadDataHandlingStrategy::handle(const locationforecast::Document & document)
{
	out_ << "locationforecast\t88,0,88\n";

	for ( locationforecast::Document::const_iterator element = document.begin(); element != document.end(); ++ element )
	{
		if ( conf_.locationforecastConfiguration().canCreateSaveSpecificationFor(* element) )
		{
			std::vector<WdbSaveSpecification> saveSpecs;
			conf_.locationforecastConfiguration().createSaveSpecification(saveSpecs, * element);
			for ( std::vector<WdbSaveSpecification>::const_iterator spec = saveSpecs.begin(); spec != saveSpecs.end(); ++ spec )
				spec->getFastloadText(out_);
		}
	}

	out_ << std::endl;
}
