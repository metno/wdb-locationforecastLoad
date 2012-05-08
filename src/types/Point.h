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
	Point(double longitude, double latitude);
	~Point();

	double longitude() const { return longitude_; }
	double latitude() const { return latitude_; }

	std::string wkt() const;

	std::string wdbCanonicalName() const;

	bool initialized() const;

private:
	double longitude_;
	double latitude_;
};

inline bool operator == (const Point & a, const Point & b)
{
	return ((not a.initialized()) and (not b.initialized()) ) or
			a.longitude() == b.longitude() and a.latitude() == b.latitude();
}
inline bool operator != (const Point & a, const Point & b)
{
	return not (a == b);
}

inline bool operator < (const Point & a, const Point & b)
{
	if ( a.longitude() != b.longitude() )
		return a.longitude() < b.longitude();
	return a.latitude() < b.latitude();
}

inline std::ostream & operator << (std::ostream & s, const Point & p)
{
	return s << "POINT(" << p.longitude() << ' ' << p.latitude() << ')';
}

} /* namespace type */
#endif /* POINT_H_ */
