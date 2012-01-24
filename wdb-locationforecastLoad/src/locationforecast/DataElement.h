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

#ifndef DATAELEMENT_H_
#define DATAELEMENT_H_

#include <string>

namespace locationforecast
{

class DataElement
{
public:
	DataElement();
	~DataElement();

	bool complete() const;


	void value(float value)
	{
		value_ = value;
	}
	float value() const
	{
		return value_;
	}

	void parameter(const std::string & parameter)
	{
		parameter_ = parameter;
	}
	const std::string & parameter() const
	{
		return parameter_;
	}

	void location(const std::string & location)
	{
		location_ = location;
	}
	const std::string & location() const
	{
		return location_;
	}

	void validFrom(const std::string & time)
	{
		validFrom_ = time;
	}
	const std::string & validFrom() const
	{
		return validFrom_;
	}

	void validTo(const std::string & time)
	{
		validTo_ = time;
	}
	const std::string & validTo() const
	{
		return validTo_;
	}


private:
	float value_;
	std::string parameter_;
	std::string location_;
	std::string validFrom_;
	std::string validTo_;
};

}
#endif /* DATAELEMENT_H_ */
