ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST =

AM_CPPFLAGS = \
	-I$(top_srcdir)/src \
	$(libwdbLoad_CFLAGS) \
	$(libcurl_CFLAGS)

if BOOST_XML_PARSE
AM_CPPFLAGS += -DBOOST_XML_PARSE
else
AM_CPPFLAGS += $(libxmlpp_CFLAGS)
endif

AM_CXXFLAGS = \
	-DSYSCONFDIR=\"$(sysconfdir)\" \
	-DVERSION=\"$(VERSION)\"

AM_LDADD = \
	$(libwdbLoad_LIBS) \
	$(libcurl_LIBS)

if BOOST_XML_PARSE
else
AM_LDADD += $(libxmlpp_LIBS)
endif

noinst_LIBRARIES = liblocationforecastLoad.a

bin_PROGRAMS = locationforecastLoad
locationforecastLoad_LDADD = liblocationforecastLoad.a $(AM_LDADD)

man_MANS = locationforecastLoad.1
locationforecastLoad.1 : locationforecastLoad
	$(HELP2MAN) \
	-n"load locationforecast data from api.met.no int a wdb database" \
	-N \
	./$< > $@


CLEANFILES = $(man_MANS)


include src/make.mk
include etc/make.mk
include test/make.mk
include debian.mk

