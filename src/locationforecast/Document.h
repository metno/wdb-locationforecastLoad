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

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include "DataElement.h"
#include "LocationForecastException.h"
#include "elementhandler/ElementHandler.h"
#include <boost/filesystem/path.hpp>
#include <iosfwd>
#include <string>
#include <exception>
#include <vector>
#include <map>


namespace xmlpp
{
class Node;
}

/**
 * Functionality for reading a locationforecast document. For more information
 * about these documents and their format, see
 * http://api.met.no/weatherapi/locationforecast/1.8/documentation
 */
namespace locationforecast
{
class LocationforecastConfiguration;

/**
 * A parsed version of a locationforecast document. Its contents may be
 * traversed via a random-access iterator. Each of this class' element is an
 * object of the class DataElement.
 *
 * Exactly what parameters to read, and how to read them is defined in the
 * configuration file, which is given to the constructors, along with the xml
 * document to read.
 */
class Document
{
	typedef std::vector<DataElement> DataList;

public:

	/**
	 * Construct document, based on data read from the given stream.
	 *
	 * @throw Document::Exception on error when reading document
	 *
	 * @param sin A locationforecast document is read from this stream
	 * @param configuration Path to a configuration file, for defining how to
	 *                      read what parameters.
	 */
	Document(std::istream & sin, LocationforecastConfiguration & configuration);

	/**
	 * Construct document, based on data read from the given URL.
	 *
	 * @throw Document::Exception on error when reading document
	 *
	 * @param sin A locationforecast document is read from this stream
	 * @param configuration Path to a configuration file, for defining how to
	 *                      read what parameters.
	 */
	Document(const std::string & url, LocationforecastConfiguration & configuration);

	/**
	 * Construct document, based on data read from the given file.
	 *
	 * @throw Document::Exception on error when reading document
	 *
	 * @param sin A locationforecast document is read from this stream
	 * @param configuration Path to a configuration file, for defining how to
	 *                      read what parameters.
	 */
	Document(const boost::filesystem::path & file, LocationforecastConfiguration & configuration);

	Document(float longitude, float latitude, LocationforecastConfiguration & configuration);

	~Document();

	typedef DataList::const_iterator const_iterator;
	typedef DataList::value_type value_type;
	typedef DataList::size_type size_type;

	const_iterator begin() const { return elements_.begin(); }
	const_iterator end() const { return elements_.end(); }

	const value_type & front() { return elements_.front(); }

	size_type size() const { return elements_.size(); }
	bool empty() const { return elements_.empty(); }

private:
	void parse_(std::istream & s);

	void parseFile_(const boost::filesystem::path & file);
	void parseUrl_(const std::string & url);

	LocationforecastConfiguration & configuration_;
	DataList elements_;
};


}

#endif /* DOCUMENT_H_ */
