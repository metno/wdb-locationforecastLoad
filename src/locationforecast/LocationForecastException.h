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


#ifndef LOCATIONFORECASTEXCEPTION_H_
#define LOCATIONFORECASTEXCEPTION_H_

#include <exception>
#include <string>

namespace locationforecast
{
#define LOCATIONFORECAST_EXCEPTION(ExceptionName) \
class ExceptionName : public Exception { \
public: \
	ExceptionName(const std::string & msg) : Exception(msg) {} \
}



/**
 * Base class for exceptions thrown by classes in this namespace
 */
class Exception : public std::exception
{
public:
	Exception(const std::string & reason) :
		reason_(reason)
	{}
	virtual ~Exception() throw()
	{}
	virtual const char * what() const throw()
	{
		return reason_.c_str();
	}
private:
	std::string reason_;
};

LOCATIONFORECAST_EXCEPTION(NoSuchFile);
LOCATIONFORECAST_EXCEPTION(FileIsDirectory);
LOCATIONFORECAST_EXCEPTION(ParseException);
LOCATIONFORECAST_EXCEPTION(HttpException);
}



#endif /* LOCATIONFORECASTEXCEPTION_H_ */
