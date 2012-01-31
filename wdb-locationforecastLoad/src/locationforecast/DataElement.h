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

/**
 * A single data element, normally acquired by traversing an iterator over a
 * Document object. Objects of this class is always created without any
 * particular initialization. It must be populated by calling its setter
 * methods. You may check whether all values have got a valid value by calling
 * the complete() method.
 *
 * @see Document
 */
class DataElement
{
public:
	/**
	 * Create an uninitialized object
	 */
	DataElement();

	~DataElement();

	/**
	 * Do all elements in this object have a valid value?
	 */
	bool complete() const;

	/**
	 * Set the parameter's value
	 *
	 * @param value the value to set
	 */
	void value(float value)
	{
		value_ = value;
	}

	/**
	 * Get the parameter's value
	 *
	 * @return the value, of NaN if not set
	 */
	float value() const
	{
		return value_;
	}


	/**
	 * Set the value parameter name
	 *
	 * @param parameter name of the value parameter
	 */
	void parameter(const std::string & parameter)
	{
		parameter_ = parameter;
	}

	/**
	 * Get the value parameter
	 *
	 * @return the value parameter name
	 */
	const std::string & parameter() const
	{
		return parameter_;
	}

	/**
	 * Set the location. This is supposed to be a WKT POINT, such as this:
	 * POINT(10.7461 59.9127), which points to Oslo.
	 */
	void location(const std::string & location)
	{
		location_ = location;
	}

	/**
	 * Get the location, as a WKT POINT string
	 */
	const std::string & location() const
	{
		return location_;
	}

	/**
	 * Set the model run time for this data
	 *
	 * @param time the runtime for the model
	 */
	void referenceTime(const std::string & time)
	{
		referenceTime_ = time;
	}

	/**
	 * Get the mode run time
	 *
	 * @return runtime for the model which generated this piece of data.
	 */
	const std::string & referenceTime() const
	{
		return referenceTime_;
	}

	/**
	 * Set the first time point for which this data is valid.
	 *
	 * @param time the valid time point to set
	 */
	void validFrom(const std::string & time)
	{
		validFrom_ = time;
	}

	/**
	 * Get the first time point for which this data is valid.
	 *
	 * @return the first valid time point for this object
	 */
	const std::string & validFrom() const
	{
		return validFrom_;
	}

	/**
	 * Set the last time point for which this data is valid.
	 *
	 * @param time the valid time point to set
	 */
	void validTo(const std::string & time)
	{
		validTo_ = time;
	}

	/**
	 * Get the last time point for which this data is valid.
	 *
	 * @return the last valid time point for this object
	 */
	const std::string & validTo() const
	{
		return validTo_;
	}


private:
	float value_;
	std::string parameter_;
	std::string location_;
	std::string referenceTime_;
	std::string validFrom_;
	std::string validTo_;
};

}
#endif /* DATAELEMENT_H_ */
