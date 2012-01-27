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

#ifndef TIMERANGE_H_
#define TIMERANGE_H_

#include <string>

namespace locationforecast
{

class TimeRange
{
public:
	typedef std::string Time;

	TimeRange(const Time & from, const Time & to);
	~TimeRange();

	bool encloses(const Time & t) const;

	const Time & from() const { return from_; }
	const Time & to() const { return to_; }

private:
	Time from_;
	Time to_;
};

inline bool operator < (const TimeRange & a, const TimeRange & b)
{
	if ( a.from() != b.from() )
		return a.from() < b.from();
	return a.to() < b.to();
}

} /* namespace locationforecast */
#endif /* TIMERANGE_H_ */
