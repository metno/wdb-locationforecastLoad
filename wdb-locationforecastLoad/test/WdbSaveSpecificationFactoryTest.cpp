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
#include <configuration/WdbSaveSpecificationFactory.h>

class WdbSaveSpecificationFactoryTest : public testing::Test
{
public:
	WdbSaveSpecificationFactoryTest() :
		factory("../test/etc/locationForecastLoad.conf.xml")
	{
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
	WdbSaveSpecificationFactory factory;
};


TEST_F(WdbSaveSpecificationFactoryTest, test)
{
	WdbSaveSpecification saveSpec = factory.create(dataElement);

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

	WdbSaveSpecification saveSpec = factory.create(dataElement);

	EXPECT_FLOAT_EQ(269.65, saveSpec.value());
}

TEST_F(WdbSaveSpecificationFactoryTest, setsLevelsFromConfig)
{
	dataElement.value(50);
	dataElement.parameter("mediumClouds");

	WdbSaveSpecification saveSpec = factory.create(dataElement);

	EXPECT_EQ("atmosphere sigma coordinate", saveSpec.levelParameter());
	EXPECT_FLOAT_EQ(500, saveSpec.levelFrom());
	EXPECT_FLOAT_EQ(500, saveSpec.levelTo());
}
