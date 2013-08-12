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

#ifndef POINT_H_
#define POINT_H_

#include <string>
#include <ostream>

namespace type
{

class Point
{
public:
	Point();
	Point(const std::string & wkt);
	Point(const char * wkt);
	Point(const std::string & longitude, const std::string & latitude);
	~Point();

	const std::string & wkt() const { return wkt_; }

	std::string wdbCanonicalName() const;

	bool initialized() const;

private:
	std::string wkt_;
};

bool operator == (const Point & a, const Point & b);
bool operator != (const Point & a, const Point & b);
bool operator < (const Point & a, const Point & b);
std::ostream & operator << (std::ostream & s, const Point & p);

} /* namespace type */

#endif /* POINT_H_ */
