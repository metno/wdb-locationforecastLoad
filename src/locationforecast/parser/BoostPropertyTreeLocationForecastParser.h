/*
 locationforecastLoad 

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

#ifndef BOOSTPROPERTYTREELOCATIONFORECASTPARSER_H_
#define BOOSTPROPERTYTREELOCATIONFORECASTPARSER_H_

#include "LocationForecastParser.h"
#include <boost/property_tree/ptree_fwd.hpp>

namespace locationforecast
{
class LocationforecastConfiguration;

class BoostPropertyTreeLocationForecastParser: public LocationForecastParser
{
public:
	BoostPropertyTreeLocationForecastParser(LocationforecastConfiguration & configuration);
	virtual ~BoostPropertyTreeLocationForecastParser();

	virtual DataList parse(std::istream & document) const;

private:
	void parseTime_(DataElement workingData, std::vector<DataElement> & out, const boost::property_tree::ptree & node) const;
	void parseLocation_(DataElement workingData, std::vector<DataElement> & out, const boost::property_tree::ptree & node) const;
	void parseParameter_(DataElement workingData, std::vector<DataElement> & out, const std::string & parameter, const boost::property_tree::ptree & node) const;

	LocationforecastConfiguration & configuration_;
};

} /* namespace locationforecast */
#endif /* BOOSTPROPERTYTREELOCATIONFORECASTPARSER_H_ */
