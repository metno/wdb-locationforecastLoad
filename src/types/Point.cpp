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

#include "Point.h"
#include <sstream>
#include <limits>


namespace type
{

Point::Point() :
		longitude_(std::numeric_limits<double>::quiet_NaN()),
		latitude_(std::numeric_limits<double>::quiet_NaN())
{
}

Point::Point(double longitude, double latitude) :
		longitude_(longitude),
		latitude_(latitude)
{
}

Point::~Point()
{
}

std::string Point::wkt() const
{
	std::ostringstream s;
	s << * this;
	return s.str();
}

std::string Point::wdbCanonicalName() const
{
	std::ostringstream s;
	s << "point(" << longitude_ << ' ' << latitude_ << ')';
	return s.str();
}

bool Point::initialized() const
{
	// this will be false if longitude_ or latitude_ is NaN
	return longitude_ == longitude_ and
			latitude_ == latitude_;
}


} /* namespace type */
