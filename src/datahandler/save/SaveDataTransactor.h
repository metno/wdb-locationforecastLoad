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

#ifndef SAVEDATATRANSACTOR_H_
#define SAVEDATATRANSACTOR_H_

#include "types/Point.h"
#include <pqxx/transactor>
#include <string>
#include <map>

namespace locationforecast
{
class LoaderConfiguration;
class Document;
}

/**
 * Functor for saving data to a wdb database. See pqxx documentation for details about how this works.
 */
class SaveDataTransactor : public pqxx::transactor<>
{
public:


	/**
	 * Initialize transactor.
	 *
	 * @param conf configuration for loading
	 * @param document the document to load
	 */
	SaveDataTransactor(const locationforecast::LoaderConfiguration & conf, const locationforecast::Document & document);
	~SaveDataTransactor();

	void operator()(pqxx::work & transaction);

private:
	const std::string & getPlaceName_(pqxx::work & transaction, const type::Point & geometryPoint, const std::string & referencetime);
	const std::string & getCustomPlaceName_(pqxx::work & transaction, const type::Point & geometryPoint);
	void createPlacePoint(pqxx::work & transaction, const std::string & name, const type::Point & geometryPoint);

	std::map<type::Point, std::string> nameFromGeometry_;

	const locationforecast::LoaderConfiguration & conf_;
	const locationforecast::Document & document_;
};

#endif /* SAVEDATATRANSACTOR_H_ */
