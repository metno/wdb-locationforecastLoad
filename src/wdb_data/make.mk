liblocationforecastLoad_a_SOURCES += \
	src/wdb_data/element/IgnoreConfigurationElement.h \
	src/wdb_data/element/ConfigurationElement.h \
	src/wdb_data/element/ConfigurationElement.cpp \
	src/wdb_data/element/NormalConfigurationElement.cpp \
	src/wdb_data/element/NormalConfigurationElement.h \
	src/wdb_data/WdbSaveSpecification.cpp \
	src/wdb_data/WdbSaveSpecification.h

if BOOST_XML_PARSE
else
liblocationforecastLoad_a_SOURCES += \
	src/wdb_data/xmlutil.h \
	src/wdb_data/xmlutil.cpp
endif
