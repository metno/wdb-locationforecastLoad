noinst_LIBARRIES = liblocationforecastLoad.a

liblocationforecastLoad_a_SOURCES = \
	src/configuration/LoaderConfiguration.h
	src/configuration/LoaderConfiguration.cpp
	src/datahandler/list/PrintingDataHandlingStrategy.h
	src/datahandler/list/PrintingDataHandlingStrategy.cpp
	src/datahandler/save/SaveDataTransactor.h
	src/datahandler/save/SavingDataHandlingStrategy.cpp
	src/datahandler/save/SavingDataHandlingStrategy.h
	src/datahandler/save/SaveDataTransactor.cpp
	src/datahandler/DataHandlingStrategy.h
	src/wdb_access/element/IgnoreConfigurationElement.h
	src/wdb_access/element/ConfigurationElement.h
	src/wdb_access/element/ConfigurationElement.cpp
	src/wdb_access/element/NormalConfigurationElement.cpp
	src/wdb_access/element/NormalConfigurationElement.h
	src/wdb_access/WdbSaveSpecification.cpp
	src/wdb_access/xmlutil.h
	src/wdb_access/xmlutil.cpp
	src/wdb_access/WdbSaveSpecificationFactory.cpp
	src/wdb_access/WdbSaveSpecificationFactory.h
	src/wdb_access/WdbSaveSpecification.h
	src/locationforecast/TimeRange.h
	src/locationforecast/DataElement.h
	src/locationforecast/Document.h
	src/locationforecast/Document.cpp
	src/locationforecast/elementhandler/ElementHandler.h
	src/locationforecast/elementhandler/SimpleElementHandler.cpp
	src/locationforecast/elementhandler/UnitSpecializedElementHandler.cpp
	src/locationforecast/elementhandler/UnitSpecializedElementHandler.h
	src/locationforecast/elementhandler/ElementHandler.cpp
	src/locationforecast/elementhandler/SimpleElementHandler.h
	src/locationforecast/TimeRange.cpp
	src/locationforecast/DataElement.cpp


bin_PROGRAMS = locationforecastLoad
locationforecastLoad_SOURCES = src/main.cpp
locationforecastLoad_LIBS = liblocationforecastLoad.a
