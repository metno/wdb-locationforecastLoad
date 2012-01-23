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

#include "Document.h"
#include "parse_document.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>

namespace locationforecast
{

Document::Document()
{
	parse_document(std::cin, elements_);
}

Document::Document(const boost::filesystem::path & file)
{
	if ( ! exists(file) )
		throw NoSuchFile(file.string() + "does not exist");
	if ( is_directory(file) )
		throw FileIsDirectory(file.string() + " is a directory");

	boost::filesystem::ifstream xmlStream(file);

	parse_document(xmlStream, elements_);
}

Document::~Document()
{
}

}
