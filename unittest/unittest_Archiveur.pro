QT +=   testlib \
        sql

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

VPATH += \
        $$PWD/../utilitary/ \
        $$PWD/../gui/

INCLUDEPATH += \
        $$PWD/../utilitary/ \
        $$PWD/../gui/

SOURCES +=  tst_unittest_archiveur.cpp \
            c_dbmanager.cpp \
            c_document.cpp \
            c_tag.cpp

HEADERS +=  c_dbmanager.h \
            c_document.h \
            c_tag.h
