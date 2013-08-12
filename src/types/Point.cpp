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
#include <boost/regex.hpp>
#include <sstream>
#include <limits>
#include <algorithm>
#include <cctype>

namespace type
{

namespace
{
void verify(const std::string & wkt)
{
	if ( not boost::regex_match(wkt, boost::basic_regex<char>("POINT\\(\\-?\\d*[.]?\\d+ \\-?\\d*[.]?\\d+\\)")) )
		throw std::runtime_error("Invalid wkt: " + wkt);
}

std::string stripTrailingZero(const std::string & s)
{
	std::string::size_type f = s.find_last_not_of('0');
	if ( f == std::string::npos )
		return "";
	if ( s[f] == '.' )
		-- f;
	return s.substr(0, f + 1);
}
}

Point::Point()
{
}

Point::Point(const std::string & wkt) :
		wkt_(wkt)
{
	verify(wkt_);
}

Point::Point(const char * wkt) :
		wkt_(wkt)
{
	verify(wkt_);

}

Point::Point(const std::string & longitude, const std::string & latitude)
{
	std::ostringstream s;
	s << "POINT(" << stripTrailingZero(longitude) << ' ' << stripTrailingZero(latitude) << ')';
	wkt_ = s.str();
	verify(wkt_);
}


Point::~Point()
{
}

namespace
{
inline char lower(char c)
{
	return std::tolower(c);
}
}

std::string Point::wdbCanonicalName() const
{
	std::string ret = wkt();
	std::transform(ret.begin(), ret.end(), ret.begin(), lower);
	return ret;

}

bool Point::initialized() const
{
	return not wkt_.empty();
}

bool operator == (const Point & a, const Point & b)
{
	return a.wkt() == b.wkt();
}

bool operator != (const Point & a, const Point & b)
{
	return not (a == b);
}

bool operator < (const Point & a, const Point & b)
{
	return a.wkt() < b.wkt();
}

std::ostream & operator << (std::ostream & s, const Point & p)
{
	return s << p.wkt();
}


} /* namespace type */
