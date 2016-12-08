TEMPLATE = app

QT += qml quick
CONFIG += c++11

CONFIG += warn_on qmltestcase

SOURCES += main.cpp \
    serversetup.cpp \
    clientsetup.cpp

RESOURCES += qml.qrc

#dependencies
VEIN_DEP_EVENT = 1
VEIN_DEP_COMP = 1
VEIN_DEP_HASH = 1
VEIN_DEP_PROTOBUF = 1
VEIN_DEP_NET2 = 1
VEIN_DEP_TCP2 = 1
VEIN_DEP_HELPER = 1
VEIN_DEP_QML = 1

exists( ../../vein-framework.pri ) {
  include(../../vein-framework.pri)
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

unix {
    target.path = /usr/bin
}

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    serversetup.h \
    clientsetup.h
