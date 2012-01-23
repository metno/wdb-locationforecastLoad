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
#include <locationforecast/DataElement.h>

using locationforecast::DataElement;

TEST(DataElementTest, setValues)
{
	DataElement de;
	de.value(0.4);
	de.parameter("precipitation");
	de.location("POINT(9 63)");
	de.validFrom("2012-01-23T06:00:00Z");
	de.validTo("2012-01-23T12:00:00Z");

	EXPECT_TRUE(de.complete());
	EXPECT_FLOAT_EQ(0.4, de.value());
	EXPECT_EQ("precipitation", de.parameter());
	EXPECT_EQ("POINT(9 63)", de.location());
	EXPECT_EQ("2012-01-23T06:00:00Z", de.validFrom());
	EXPECT_EQ("2012-01-23T12:00:00Z", de.validTo());
}

TEST(DataElement, incompleteOnMissingValidFrom)
{
	DataElement de;
	de.value(2.9);
	de.parameter("precipitation");
	de.location("POINT(9 63)");
	de.validTo("2012-01-23T12:00:00Z");

	EXPECT_FALSE(de.complete());
}

TEST(DataElement, incompleteOnMissingValidTo)
{
	DataElement de;
	de.value(2.9);
	de.parameter("precipitation");
	de.location("POINT(9 63)");
	de.validFrom("2012-01-23T06:00:00Z");

	EXPECT_FALSE(de.complete());
}

TEST(DataElement, incompleteOnMissingParameter)
{
	DataElement de;
	de.value(2.9);
	de.location("POINT(9 63)");
	de.validFrom("2012-01-23T06:00:00Z");
	de.validTo("2012-01-23T12:00:00Z");

	EXPECT_FALSE(de.complete());
}

TEST(DataElement, incompleteOnMissingValue)
{
	DataElement de;
	de.parameter("precipitation");
	de.location("POINT(9 63)");
	de.validFrom("2012-01-23T06:00:00Z");
	de.validTo("2012-01-23T12:00:00Z");

	EXPECT_FALSE(de.complete());
}

TEST(DataElement, incompleteOnMissingLocation)
{
	DataElement de;
	de.value(2.9);
	de.parameter("precipitation");
	de.validFrom("2012-01-23T06:00:00Z");
	de.validTo("2012-01-23T12:00:00Z");

	EXPECT_FALSE(de.complete());
}
