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
#include <boost/filesystem/path.hpp>
#include <iosfwd>
#include <string>
#include <exception>
#include <vector>


namespace locationforecast
{

#define DOCUMENT_EXCEPTION(ExceptionName) \
class ExceptionName : public Exception { \
public: \
	ExceptionName(const std::string & msg) : 	Document::Exception(msg) {} \
}


class Document
{
	typedef std::vector<DataElement> DataList;

public:
	Document();
	Document(const boost::filesystem::path & file);
	~Document();

	typedef DataList::const_iterator const_iterator;
	typedef DataList::value_type value_type;
	typedef DataList::size_type size_type;

	const_iterator begin() const { return elements_.begin(); }
	const_iterator end() const { return elements_.end(); }

	const value_type & front() { return elements_.front(); }

	size_type size() const { return elements_.size(); }
	bool empty() const { return elements_.empty(); }

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

	DOCUMENT_EXCEPTION(NoSuchFile);
	DOCUMENT_EXCEPTION(FileIsDirectory);
	DOCUMENT_EXCEPTION(ParseException);

private:
	DataList elements_;
};


}

#endif /* DOCUMENT_H_ */
