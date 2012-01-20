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


#include <gtest/gtest.h>
#include <locationforecast/Document.h>
#include <boost/filesystem/path.hpp>
#include <iterator>


using locationforecast::Document;


namespace
{
const boost::filesystem::path testFiles = "../test/locationforecast";
}

TEST(DocumentTest, itWorks)
{
	Document doc(testFiles/"real_example.xml");

	Document::size_type expectedSize = 260;

	EXPECT_FALSE(doc.empty());
	ASSERT_EQ(expectedSize, doc.size());

	Document::const_iterator it = doc.begin();
	std::advance(it, expectedSize);
	EXPECT_TRUE(it == doc.end());
}

TEST(DocumentTest, failOnNonExistingDocument)
{
	ASSERT_THROW(Document(testFiles/"no_such_file.xml"), Document::NoSuchFile);
}

TEST(DocumentTest, failOnDirectoryAsDocument)
{
	ASSERT_THROW(Document("."), Document::FileIsDirectory);
}

TEST(DocumentTest, emptyDocument)
{
	Document doc(testFiles/"empty_document.xml");

	EXPECT_TRUE(doc.empty());
	EXPECT_EQ(0u, doc.size());
	EXPECT_TRUE(doc.begin() == doc.end());
}
TEST(DocumentTest, oneElementDocument)
{
	Document doc(testFiles/"one_element_document.xml");

	EXPECT_FALSE(doc.empty());
	EXPECT_EQ(1u, doc.size());
}
