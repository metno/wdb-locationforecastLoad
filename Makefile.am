ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST =

AM_CPPFLAGS = \
	-I$(top_srcdir)/src \
	$(libwdbLoad_CFLAGS) \
	$(libxmlpp_CFLAGS)

AM_LDADD = \
	$(libwdbLoad_LIBS) \
	$(libxmlpp_LIBS)


noinst_LIBRARIES = liblocationforecastLoad.a

bin_PROGRAMS = locationforecastLoad
locationforecastLoad_LDADD = liblocationforecastLoad.a $(AM_LDADD)

include src/make.mk
include etc/make.mk
include test/make.mk
