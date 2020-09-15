QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network
QT  +=dbus
LIBS += -lapt-pkg
CONFIG += c++11

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
    addsource.cpp \
    clickableLabel.cpp \
    createsourcewidget.cpp \
    delayrefreshthread.cpp \
    form.cpp \
    main.cpp \
    setAlarmRepeatDialog.cpp \
    sourceinformationwidget.cpp \
    sourcemanager.cpp\
    sourceinterface.cpp \
    testingupdatethread.cpp

HEADERS += \
    addsource.h \
    clickableLabel.h \
    createsourcewidget.h \
    delayrefreshthread.h \
    form.h \
    setAlarmRepeatDialog.h \
    sourceinformationwidget.h \
    sourcemanager.h\
    sourceinterface.h \
    testingupdatethread.h

FORMS += \
    addsource.ui \
    createsourcewidget.ui \
    form.ui \
    sourceinformationwidget.ui \
    sourcemanager.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target
