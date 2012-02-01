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

/**
 * A time period, consisting of a start time and an end time.
 *
 * Note that since the developer is lazy, and nothing more is needed, this
 * class has been implemented using strings.
 */
class TimeRange
{
public:
	/**
	 * An instant in time. The time objects are supposed to be of the form
	 * 2012-01-31T12:00:00Z.
	 */
	typedef std::string Time;

	/**
	 * Create a new time range object.
	 *
	 * @see Time
	 *
	 * @param from Starting time point for range, inclusive
	 * @param to Ending time point for range, inclusive.
	 */
	TimeRange(const Time & from, const Time & to);
	~TimeRange();

	/**
	 * Is the given time instant on or after this->from() and before or on
	 * this->to()?
	 */
	bool encloses(const Time & t) const;

	/**
	 * Staring time range, inclusive
	 */
	const Time & from() const { return from_; }

	/**
	 * Ending time range, inclusive
	 */
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
