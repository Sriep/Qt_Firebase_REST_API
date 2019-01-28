QT       += core network firebasedb

TARGET = firebaseapi
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    firebaseexamples.cpp

HEADERS += \
    firebaseexamples.h

target.path = $$[QT_INSTALL_EXAMPLES]/firebasedb/simple
INSTALLS += target
