TARGET = QtFirebaseDb
MODULE = firebasedb

QT += core core-private network

#DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

HEADERS += \
    firebaseinterface.h \
    firebase.h

SOURCES += \
    firebase.cpp

load(qt_module)
