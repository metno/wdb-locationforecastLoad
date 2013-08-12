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
#include <configuration/LocationforecastConfiguration.h>
#include <boost/filesystem/path.hpp>
#include <iterator>


using namespace locationforecast;


namespace
{
const boost::filesystem::path testFiles = SRCDIR"/test/etc";
//const boost::filesystem::path configuration = SRCDIR"/etc/locationforecastLoad.conf.xml";
LoaderConfiguration configuration()
{
	LoaderConfiguration ret;
	ret.translation().translationConfiguration = SRCDIR"/etc/locationforecastLoad.conf.xml";
	return ret;
}
}

TEST(DocumentTest, itWorks)
{
	LocationforecastConfiguration conf(configuration());
	Document doc(testFiles/"real_example.xml", conf);

	Document::size_type expectedSize = 1290;

	EXPECT_FALSE(doc.empty());

	ASSERT_EQ(expectedSize, doc.size());

	Document::const_iterator it = doc.begin();
	std::advance(it, expectedSize);
	EXPECT_TRUE(it == doc.end());
}

TEST(DocumentTest, failOnCorruptDocument)
{
	LocationforecastConfiguration conf(configuration());
	ASSERT_THROW(Document(testFiles/"corrupt_document.xml", conf), ParseException);
}

TEST(DocumentTest, failOnNonExistingDocument)
{
	LocationforecastConfiguration conf(configuration());
	ASSERT_THROW(Document(testFiles/"no_such_file.xml", conf), NoSuchFile);
}

TEST(DocumentTest, failOnDirectoryAsDocument)
{
	LocationforecastConfiguration conf(configuration());
	ASSERT_THROW(Document(boost::filesystem::path("."), conf), FileIsDirectory);
}

TEST(DocumentTest, emptyDocument)
{
	LocationforecastConfiguration conf(configuration());
	Document doc(testFiles/"empty_document.xml", conf);

	EXPECT_TRUE(doc.empty());
	EXPECT_EQ(0u, doc.size());
	EXPECT_TRUE(doc.begin() == doc.end());
}
TEST(DocumentTest, oneElementDocument)
{
	LocationforecastConfiguration conf(configuration());
	Document doc(testFiles/"one_element_document.xml", conf);

	EXPECT_FALSE(doc.empty());
	ASSERT_EQ(1u, doc.size());

	const locationforecast::DataElement & dataElement = doc.front();
	EXPECT_FLOAT_EQ(-2.4, dataElement.value());
	EXPECT_EQ("temperature", dataElement.parameter());
	EXPECT_EQ("POINT(9.58 60.1)", dataElement.location());
	EXPECT_EQ("2012-01-19T10:00:00Z", dataElement.validFrom());
	EXPECT_EQ("2012-01-19T10:00:00Z", dataElement.validTo());
}

TEST(DocumentTest, setsReferenceTime)
{
	LocationforecastConfiguration conf(configuration());
	Document doc(testFiles/"real_example.xml", conf);

	Document::size_type expectedSize = 1290;
	ASSERT_EQ(expectedSize, doc.size());

	Document::const_iterator element = doc.begin();
	EXPECT_EQ("2012-01-19T00:00:00Z", element->referenceTime());

	std::advance(element, 1200);
	EXPECT_EQ("2012-01-26T00:00:00Z", element->validFrom());
	EXPECT_EQ("2012-01-26T00:00:00Z", element->validTo());
	EXPECT_EQ("2012-01-18T12:00:00Z", element->referenceTime());
}

TEST(DocumentTest, onlyOneModel_a)
{
	LoaderConfiguration config;
	const char * options[3] = {
			"<program name>",
			"--translation="SRCDIR"/etc/locationforecastLoad.conf.xml",
			"-mYR"
	};
	config.parse(3, (char **) options);
	LocationforecastConfiguration conf(config);
	Document doc(testFiles/"small_document.xml", conf);

	EXPECT_EQ(1, doc.size());
}

TEST(DocumentTest, onlyOneModel_b)
{
	LoaderConfiguration config;
	const char * options[3] = {
			"<program name>",
			"--translation="SRCDIR"/etc/locationforecastLoad.conf.xml",
			"-mEPS"
	};
	config.parse(3, (char **) options);
	LocationforecastConfiguration conf(config);
	Document doc(testFiles/"small_document.xml", conf);

	EXPECT_EQ(1, doc.size());
}

TEST(DocumentTest, twoModels)
{
	LoaderConfiguration config;
	const char * options[4] = {
			"<program name>",
			"--translation="SRCDIR"/etc/locationforecastLoad.conf.xml",
			"-mEPS",
			"-mYR"
	};
	config.parse(4, (char **) options);
	LocationforecastConfiguration conf(config);
	Document doc(testFiles/"small_document.xml", conf);

	EXPECT_EQ(2, doc.size());
}

TEST(DocumentTest, nonexistingModel)
{
	LoaderConfiguration config;
	const char * options[3] = {
			"<program name>",
			"--translation="SRCDIR"/etc/locationforecastLoad.conf.xml",
			"-mFOO"
	};
	config.parse(3, (char **) options);
	LocationforecastConfiguration conf(config);
	Document doc(testFiles/"small_document.xml", conf);

	EXPECT_TRUE(doc.empty());
}

TEST(DocumentTest, realDocument_a)
{
	LoaderConfiguration config;
	const char * options[3] = {
			"<program name>",
			"--translation="SRCDIR"/etc/locationforecastLoad.conf.xml",
			"-mYR"
	};
	config.parse(3, (char **) options);
	LocationforecastConfiguration conf(config);
	Document doc(testFiles/"real_example.xml", conf);

	EXPECT_EQ(1056, doc.size());
}

TEST(DocumentTest, realDocument_b)
{
	LoaderConfiguration config;
	const char * options[3] = {
			"<program name>",
			"--translation="SRCDIR"/etc/locationforecastLoad.conf.xml",
			"-mEPS"
	};
	config.parse(3, (char **) options);
	LocationforecastConfiguration conf(config);
	Document doc(testFiles/"real_example.xml", conf);

	EXPECT_EQ(234, doc.size());
}
