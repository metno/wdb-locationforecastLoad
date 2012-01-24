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

#ifndef WDBSAVESPECIFICATION_H_
#define WDBSAVESPECIFICATION_H_

#include <string>
#include <boost/function.hpp>

class WdbSaveSpecificationFactory;

class WdbSaveSpecification
{
public:
	WdbSaveSpecification();
	~WdbSaveSpecification();

	std::string getReadQuery(boost::function<std::string (const std::string &)> escape) const;

//	float value() const { return value_; }
//	const std::string & location() const { return location_; }
//	const std::string & referenceTime() const { return referenceTime_; }
//	const std::string & validFrom() const { return validFrom_; }
//	const std::string & validTo() const { return validTo_; }
//	const std::string & valueParameter() const { return valueParameter_; }
//	const std::string & levelParameter() const { return levelParameter_; }
//	float levelFrom() const { return levelFrom_; }
//	float levelTo() const { return levelTo_; }

private:
	friend class WdbSaveSpecificationFactory;

	float value_;
	std::string location_;
	std::string referenceTime_;
	std::string validFrom_;
	std::string validTo_;
	std::string valueParameter_;
	std::string levelParameter_;
	float levelFrom_;
	float levelTo_;
};

#endif /* WDBSAVESPECIFICATION_H_ */
