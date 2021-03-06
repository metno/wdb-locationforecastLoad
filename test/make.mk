TESTS = locationforecastLoadTest

check_PROGRAMS = locationforecastLoadTest

locationforecastLoadTest_SOURCES = \
	test/DataElementTest.cpp \
	test/DocumentTest.cpp \
	test/WdbSaveSpecificationFactoryTest.cpp

locationforecastLoadTest_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	$(gtest_CFLAGS)

locationforecastLoadTest_CXXFLAGS = \
	$(AM_CXXFLAGS) \
	-DSRCDIR=\"$(top_srcdir)\"


locationforecastLoadTest_LDADD = \
	$(locationforecastLoad_LDADD) \
	$(gtest_LIBS) -lgtest_main

include test/etc/make.mk
