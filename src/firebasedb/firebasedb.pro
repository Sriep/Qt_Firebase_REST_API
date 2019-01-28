TARGET = QtFirebaseDb
MODULE = firebasedb

QT += core network

#DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

HEADERS += \
    firebase.h

SOURCES += \
    firebase.cpp

load(qt_module)
