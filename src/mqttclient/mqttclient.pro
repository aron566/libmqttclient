TARGET = mqttclient

QT += widgets
QT += core
QT += network
QT += mqtt

CONFIG += c++11

TEMPLATE = lib
DEFINES += LIBMQTTCLIENT_LIBRARY

SOURCES += \
    customerwidgetitem.cpp \
    libmqttclient.cpp

HEADERS += \
    customerwidgetitem.h \
    libmqttclient_global.h \
    libmqttclient.h \
    ui_libmqttclient.h

INCLUDEPATH += ../mqtt/
DEPENDPATH +=  ../mqtt/

# must be exist
#load(qt_module)
