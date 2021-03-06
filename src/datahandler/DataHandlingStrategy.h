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


#ifndef DATAHANDLINGSTRATEGY_H_
#define DATAHANDLINGSTRATEGY_H_

#include <string>
#include <stdexcept>

namespace locationforecast
{
class Document;
}

/**
 * A strategy for what to do with a locationforecast document, such as storing
 * it to a database, or printing it to stdout.
 */
class DataHandlingStrategy
{
public:
	virtual ~DataHandlingStrategy() {}

	/**
	 * Handle data according to the strategy.
	 *
	 * @param document The data to process.
	 */
	virtual void handle(const locationforecast::Document & document) =0;

	struct Position
	{
		float longitude;
		float latitude;
	};

	virtual Position getPosition(const std::string & placeName)
	{
		throw std::logic_error("Getting location is not supported");
	}
};


#endif /* DATAHANDLINGSTRATEGY_H_ */
