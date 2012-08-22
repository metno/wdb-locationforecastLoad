liblocationforecastLoad_a_SOURCES += \
	src/locationforecast/TimeRange.h \
	src/locationforecast/DataElement.h \
	src/locationforecast/Document.h \
	src/locationforecast/Document.cpp \
	src/locationforecast/elementhandler/ElementHandler.h \
	src/locationforecast/elementhandler/SimpleElementHandler.cpp \
	src/locationforecast/elementhandler/UnitSpecializedElementHandler.cpp \
	src/locationforecast/elementhandler/UnitSpecializedElementHandler.h \
	src/locationforecast/elementhandler/ElementHandler.cpp \
	src/locationforecast/elementhandler/SimpleElementHandler.h \
	src/locationforecast/TimeRange.cpp \
	src/locationforecast/DataElement.cpp \
	src/locationforecast/LocationForecastException.h

include src/locationforecast/parser/make.mk
