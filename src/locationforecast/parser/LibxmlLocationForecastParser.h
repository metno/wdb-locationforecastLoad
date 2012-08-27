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

#ifndef LIBXMLLOCATIONFORECASTPARSER_H_
#define LIBXMLLOCATIONFORECASTPARSER_H_

#include "LocationForecastParser.h"
#include <configuration/LocationforecastConfiguration.h>
#include "../elementhandler/ElementHandler.h"
#include <boost/filesystem/path.hpp>
#include <iosfwd>
#include <map>


namespace xmlpp
{
class Node;
class Element;
}

namespace locationforecast
{

class LibxmlLocationForecastParser : public LocationForecastParser
{
public:
	explicit LibxmlLocationForecastParser(LocationforecastConfiguration & configuration);
	virtual ~LibxmlLocationForecastParser();

	virtual DataList parse(std::istream & document) const;

private:

	ElementHandler::Data getParameterData_(const xmlpp::Element & parameterElement) const;
	void parseParameter_(DataElement workingData, std::vector<DataElement> & out, const xmlpp::Node * node) const;
	void parseLocation_(DataElement workingData, std::vector<DataElement> & out, const xmlpp::Node * node) const;
	void parseTime_(DataElement workingData, std::vector<DataElement> & out, const xmlpp::Node * node) const;

	LocationforecastConfiguration & configuration_;
	mutable std::map<std::string, ElementHandler::Ptr> handlers_;
};

} /* namespace locationforecast */
#endif /* LIBXMLLOCATIONFORECASTPARSER_H_ */
