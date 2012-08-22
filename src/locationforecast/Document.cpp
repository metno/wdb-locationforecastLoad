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
#include "TimeRange.h"
#ifdef BOOST_XML_PARSE
#include "parser/BoostPropertyTreeLocationForecastParser.h"
#else
#include "parser/LibxmlLocationForecastParser.h"
#endif
#include <types/Point.h>
#include <wdbLogHandler.h>
#include <libxml++/libxml++.h>
#include <curl/curl.h>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

namespace locationforecast
{

Document::Document(std::istream & sin, const boost::filesystem::path & configuration) :
		configuration_(configuration)
{
	parse_(sin);
}

Document::Document(const std::string & url, const boost::filesystem::path & configuration) :
		configuration_(configuration)
{
	parseUrl_(url);
}

Document::Document(const boost::filesystem::path & file, const boost::filesystem::path & configuration) :
		configuration_(configuration)
{
	parseFile_(file);
}

namespace
{
std::string str(float f)
{
	std::ostringstream s;
	s.precision(4);
	s << std::fixed << f;
	return s.str();
}
}

Document::Document(float longitude, float latitude, const boost::filesystem::path & configuration) :
		configuration_(configuration)
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.locationforecastLoad.xmlparse" );

	std::string url = configuration_.baseUrl();
	boost::replace_all(url, "$LATITUDE$", str(latitude));
	boost::replace_all(url, "$LONGITUDE$", str(longitude));

	log.infoStream() << "Getting data from " << url;

	parseUrl_(url);
}


Document::~Document()
{
}

void Document::parse_(std::istream & s)
{
#ifdef BOOST_XML_PARSE
	BoostPropertyTreeLocationForecastParser parser(configuration_);
#else
	LibxmlLocationForecastParser parser(configuration_);
#endif
	elements_ = parser.parse(s);

	std::cout << "size: " << elements_.size() << std::endl;
}


void Document::parseFile_(const boost::filesystem::path & file)
{
	if ( ! exists(file) )
		throw NoSuchFile(file.string() + "does not exist");
	if ( is_directory(file) )
		throw FileIsDirectory(file.string() + " is a directory");

	boost::filesystem::ifstream xmlStream(file);
	parse_(xmlStream);
}

namespace
{
size_t writeToStream(char * ptr, size_t size, size_t nmemb, void *userdata)
{
	std::ostream * s = (std::ostream *) userdata;

	for ( int i = 0; i < size * nmemb; ++ i )
		(*s) << ptr[i];

	return size * nmemb;
}
}

void Document::parseUrl_(const std::string & url)
{
	WDB_LOG & log = WDB_LOG::getInstance("wdb.locationforecastLoad");

	std::stringstream data;

	curl_global_init(CURL_GLOBAL_ALL);
	CURL * curl = curl_easy_init();
	if ( ! curl )
		throw std::runtime_error("Unable to initialize web handler");

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToStream);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) & data);

	curl_easy_setopt(curl, CURLOPT_USERAGENT, "wdb-locationforecastLoad/"VERSION);

	char error_buffer[CURL_ERROR_SIZE];
	error_buffer[0] = '\0';
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

	CURLcode error = curl_easy_perform(curl);

	long http_response_code;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, & http_response_code);

	curl_easy_cleanup(curl);

	switch ( http_response_code )
	{
	case 400:
		throw HttpException("400 Bad Request: " + url);
	case 404:
		throw HttpException("404 Not found: " + url);
	case  203:
		log.warn("api.met.no version is deprecated");
		break;
	case 200:
		break;
	default:
		std::ostringstream error;
		error << "Error when fetching document: " << http_response_code;
		throw HttpException(error.str());
	}

	if ( error )
		throw HttpException(error_buffer);

	parse_(data);
}

}
