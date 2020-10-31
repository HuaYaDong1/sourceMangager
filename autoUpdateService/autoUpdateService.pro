QT -= gui

CONFIG += c++11 console link_pkgconfig
CONFIG -= app_bundle

INCLUDEPATH = /usr/include/qapt

LIBS += -lQApt

PKGCONFIG += glib-2.0

TARGET = autoUpdate

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        autoupdateservice.cpp \
        main.cpp

HEADERS += \
    autoupdateservice.h

unix {
    target.path = /usr/bin/
    INSTALLS += target

    servicefile.path = /usr/lib/systemd/system/
    servicefile.files += data/autoUpdate.service
    INSTALLS += servicefile

    conffile.path = /var/lib/sourcemanager/
    conffile.files += data/updateTime.conf
    INSTALLS += conffile
}
