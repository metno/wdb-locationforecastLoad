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

#include "PrintingDataHandlingStrategy.h"
#include <locationforecast/Document.h>
#include <boost/foreach.hpp>
#include <iostream>

PrintingDataHandlingStrategy::PrintingDataHandlingStrategy()
{
}

PrintingDataHandlingStrategy::~PrintingDataHandlingStrategy()
{
}

void PrintingDataHandlingStrategy::handle(const locationforecast::Document & document)
{
	type::Point location;
	std::string validFrom;
	std::string validTo;
	std::string parameter;

	BOOST_FOREACH(const locationforecast::Document::value_type & element, document)
	{
		if ( location != element.location() )
		{
			location = element.location();
			std::cout << location.wkt() << ":\n";
		}
		if ( validFrom != element.validFrom() or validTo != element.validTo())
		{
			validFrom = element.validFrom();
			validTo = element.validTo();
			std::cout << "\t" << validFrom;
			if ( validTo != validFrom )
				std::cout << " - " << validTo;
			std::cout << ":\n";
		}
		std::cout << "\t\t" << element.parameter() << "\t" << element.value() << std::endl;
	}
}
