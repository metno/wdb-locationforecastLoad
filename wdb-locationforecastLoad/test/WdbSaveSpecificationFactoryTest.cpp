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
#include <wdb_access/WdbSaveSpecificationFactory.h>
#include <configuration/LoaderConfiguration.h>

namespace
{
boost::shared_ptr<locationforecast::LoaderConfiguration>  getConfiguration(int argc, const char ** argv)
{
	boost::shared_ptr<locationforecast::LoaderConfiguration> ret(new locationforecast::LoaderConfiguration);
	ret->parse(argc, const_cast<char **>(argv));
	return ret;
}
boost::shared_ptr<locationforecast::LoaderConfiguration>  getConfiguration()
{
	const char * options = "program_name";
	return getConfiguration(1, & options);
}
}

class WdbSaveSpecificationFactoryTest : public testing::Test
{
public:

	void setConfiguration(int argc, const char ** argv)
	{
		loaderConfiguration = getConfiguration(argc, argv);
		factory = boost::shared_ptr<WdbSaveSpecificationFactory>(
				new WdbSaveSpecificationFactory(* loaderConfiguration));
	}

	WdbSaveSpecificationFactoryTest() :
		loaderConfiguration(getConfiguration())
	{
		factory = boost::shared_ptr<WdbSaveSpecificationFactory>(
				new WdbSaveSpecificationFactory(* loaderConfiguration));

		// Default dataElement setup
		dataElement.value(10);
		dataElement.parameter("precipitation");
		dataElement.location("POINT(1 2)");
		dataElement.referenceTime("2012-01-26T00:00:00Z");
		dataElement.validFrom("2012-01-26T12:00:00Z");
		dataElement.validTo("2012-01-26T18:00:00Z");

		if ( not dataElement.complete() )
			throw std::logic_error("Test prerequirement error");
	}

	locationforecast::DataElement dataElement;
	boost::shared_ptr<WdbSaveSpecificationFactory> factory;
private:
	boost::shared_ptr<locationforecast::LoaderConfiguration> loaderConfiguration;
};


TEST_F(WdbSaveSpecificationFactoryTest, test)
{
	std::vector<WdbSaveSpecification> saveElements;
	factory->create(saveElements, dataElement);
	ASSERT_EQ(1, saveElements.size());
	const WdbSaveSpecification & saveSpec = saveElements[0];

	EXPECT_FLOAT_EQ(10, saveSpec.value());
	EXPECT_EQ("POINT(1 2)", saveSpec.location());
	EXPECT_EQ("2012-01-26T00:00:00Z", saveSpec.referenceTime());
	EXPECT_EQ("2012-01-26T12:00:00Z", saveSpec.validFrom());
	EXPECT_EQ("2012-01-26T18:00:00Z", saveSpec.validTo());

	EXPECT_EQ("lwe thickness of precipitation amount", saveSpec.valueParameter());

	// From config
	EXPECT_EQ("height above ground", saveSpec.levelParameter());
	EXPECT_FLOAT_EQ(0, saveSpec.levelFrom());
	EXPECT_FLOAT_EQ(0, saveSpec.levelTo());
}


TEST_F(WdbSaveSpecificationFactoryTest, convertsValues)
{
	dataElement.value(-3.5);
	dataElement.parameter("temperature");

	std::vector<WdbSaveSpecification> saveElements;
	factory->create(saveElements, dataElement);
	ASSERT_EQ(1, saveElements.size());
	const WdbSaveSpecification & saveSpec = saveElements[0];

	EXPECT_FLOAT_EQ(269.65, saveSpec.value());
}

TEST_F(WdbSaveSpecificationFactoryTest, setsLevelsFromConfig)
{
	dataElement.value(50);
	dataElement.parameter("mediumClouds");

	std::vector<WdbSaveSpecification> saveElements;
	factory->create(saveElements, dataElement);
	ASSERT_EQ(1, saveElements.size());
	const WdbSaveSpecification & saveSpec = saveElements[0];

	EXPECT_EQ("atmosphere sigma coordinate", saveSpec.levelParameter());
	EXPECT_FLOAT_EQ(500, saveSpec.levelFrom());
	EXPECT_FLOAT_EQ(500, saveSpec.levelTo());
}

TEST_F(WdbSaveSpecificationFactoryTest, noEffectOnDataProviderOverride)
{
	// Just to have all options
	const char * options[2] = {"program_name", "--dataprovider=someone"};
	setConfiguration(2, options);
	std::vector<WdbSaveSpecification> saveElements;
	factory->create(saveElements, dataElement);
	ASSERT_EQ(1, saveElements.size());
	const WdbSaveSpecification & saveSpec = saveElements[0];
}

TEST_F(WdbSaveSpecificationFactoryTest, noEffectOnPlaceName)
{
	// Just to have all options
	const char * options[2] = {"program_name", "--placename=somewhere"};
	setConfiguration(2, options);
	std::vector<WdbSaveSpecification> saveElements;
	factory->create(saveElements, dataElement);
	ASSERT_EQ(1, saveElements.size());
	const WdbSaveSpecification & saveSpec = saveElements[0];
}


TEST_F(WdbSaveSpecificationFactoryTest, respectsReferenceTimeOverride)
{
	const char * options[2] = {"program_name", "--referencetime=today"};
	setConfiguration(2, options);

	std::vector<WdbSaveSpecification> saveElements;
	factory->create(saveElements, dataElement);
	ASSERT_EQ(1, saveElements.size());
	const WdbSaveSpecification & saveSpec = saveElements[0];
	EXPECT_EQ("today", saveSpec.referenceTime());
}

TEST_F(WdbSaveSpecificationFactoryTest, errorOnValueParameterOverride)
{
	const char * options[2] = {"program_name", "--valueparameter=rain"};
	setConfiguration(2, options);

	std::vector<WdbSaveSpecification> saveElements;
	EXPECT_THROW(factory->create(saveElements, dataElement), std::runtime_error);
}

TEST_F(WdbSaveSpecificationFactoryTest, errorOnLevelParameterOverride)
{
	const char * options[2] = {"program_name", "--levelparameter=highupsomewhere"};
	setConfiguration(2, options);

	std::vector<WdbSaveSpecification> saveElements;
	EXPECT_THROW(factory->create(saveElements, dataElement), std::runtime_error);
}

TEST_F(WdbSaveSpecificationFactoryTest, errorOnDataVersionOverride)
{
	const char * options[2] = {"program_name", "--dataversion=42"};
	setConfiguration(2, options);

	std::vector<WdbSaveSpecification> saveElements;
	EXPECT_THROW(factory->create(saveElements, dataElement), std::runtime_error);
}

TEST_F(WdbSaveSpecificationFactoryTest, errorOnconfidenceOverride)
{
	const char * options[2] = {"program_name", "--confidencecode=42"};
	setConfiguration(2, options);

	std::vector<WdbSaveSpecification> saveElements;
	EXPECT_THROW(factory->create(saveElements, dataElement), std::runtime_error);
}
