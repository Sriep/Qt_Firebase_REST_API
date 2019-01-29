QT       += core network firebasedb

TARGET = mocking
TEMPLATE = app

CONFIG += exceptions

SOURCES += \
    main.cpp

INCLUDEPATH += $$PWD/3rdparty

target.path = $$[QT_INSTALL_EXAMPLES]/firebasedb/mocking
INSTALLS += target
