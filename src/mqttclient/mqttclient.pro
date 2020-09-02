TARGET = mqttclient

QT += widgets
QT += core
QT += network
QT += mqtt

CONFIG += c++11

TEMPLATE = lib
DEFINES += LIBMQTTCLIENT_LIBRARY

SOURCES += \
    $$PWD/customerwidgetitem.cpp \
    $$PWD/libmqttclient.cpp

HEADERS += \
    $$PWD/customerwidgetitem.h \
    $$PWD/libmqttclient_global.h \
    $$PWD/libmqttclient.h \
    $$PWD/ui_libmqttclient.h

INCLUDEPATH += ../mqtt/
DEPENDPATH +=  ../mqtt/



BEFORE_LINK_CMD_LINE =echo Start build mqttclient.
QMAKE_PRE_LINK += $$quote($$BEFORE_LINK_CMD_LINE)

## linux
unix:!macx: AFTER_LINK_CMD_LINE_COPY =cp -a release/** ../../lib/

# windows
win32: AFTER_LINK_CMD_LINE_COPY =copy .\\release\\* ..\\..\\lib\\

QMAKE_POST_LINK += $$quote($$AFTER_LINK_CMD_LINE_COPY)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
