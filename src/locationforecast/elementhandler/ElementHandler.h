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


#ifndef ELEMENTHANDLER_H_
#define ELEMENTHANDLER_H_

#include <boost/shared_ptr.hpp>
#include <string>
#include <stdexcept>

#ifdef BOOST_XML_PARSE
#include <boost/property_tree/ptree_fwd.hpp>
#else
namespace xmlpp
{
class Element;
}
#endif

namespace locationforecast
{

class ElementHandler
{
public:
	virtual ~ElementHandler() {}

	typedef boost::shared_ptr<ElementHandler> Ptr;

	static Ptr get(const std::string & parameter, const std::string & unit = std::string());


	struct Data
	{
		std::string parameter;
		float value;
		std::string unit;
	};

#ifdef BOOST_XML_PARSE
	/**
	 * may throw ExtractionError if unable to parse element
	 */
	virtual Data extract(const boost::property_tree::ptree & element) const =0;
#else
	/**
	 * may throw ExtractionError if unable to parse element
	 */
	virtual Data extract(const xmlpp::Element & element) const =0;
#endif

	class ExtractionError : public std::runtime_error
	{
	public:
		ExtractionError(const std::string & msg) :
			std::runtime_error(msg)
		{}
	};
};

}

#endif /* ELEMENTHANDLER_H_ */
