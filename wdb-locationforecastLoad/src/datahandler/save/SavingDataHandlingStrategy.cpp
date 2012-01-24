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

#include "SavingDataHandlingStrategy.h"
#include <locationforecast/Document.h>
#include <wdb/LoaderConfiguration.h>
#include <pqxx/transactor.hxx>
#include <boost/foreach.hpp>
#include <iostream>

SavingDataHandlingStrategy::SavingDataHandlingStrategy(const wdb::load::LoaderConfiguration & conf) :
		connection_(conf.database().pqDatabaseConnection()),
		wciUser_(conf.database().user)
{
}

SavingDataHandlingStrategy::~SavingDataHandlingStrategy()
{
}

void foo(int i, int j)
{
	std::cout << i << ", " << j << std::endl;
}

namespace
{
	class Escaper
	{
	public:
		explicit Escaper(const pqxx::work & transaction) : transaction_(transaction) {}

		std::string operator () (const std::string & s) const
		{
			return transaction_.esc(s);
		}

	private:
		const pqxx::work & transaction_;
	};
}

void SavingDataHandlingStrategy::handle(const locationforecast::Document & document)
{
	pqxx::work transaction(connection_);
	Escaper escape(transaction);

	std::cout << "SELECT wci.begin('" << escape(wciUser_) << "')";
	std::cout << ";\n";

	BOOST_FOREACH(const locationforecast::Document::value_type & element, document)
	{
		if ( specificationFactory_.hasTranslationFor(element) )
		{
			const WdbSaveSpecification & spec = specificationFactory_.create(element);
			std::cout << spec.getReadQuery(escape) << ";\n";
		}
	}
	std::cout << std::flush;
}
