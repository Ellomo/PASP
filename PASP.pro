#-------------------------------------------------
#
# Project created by QtCreator 2019-08-10T17:40:27
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PASP
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += \c++11 \
        qaxcontainer

SOURCES += \
        customtabstyle.cpp \
        main.cpp \
        mainwindow.cpp \
        object.cpp \
        organization.cpp \
        qcustomplot.cpp \
        testgiswindow.cpp \
        testispdnwindow.cpp \
        testzokiiwindow.cpp \
        widgetforcategory.cpp

HEADERS += \
        customtabstyle.h \
        mainwindow.h \
        object.h \
        organization.h \
        qcustomplot.h \
        testgiswindow.h \
        testispdnwindow.h \
        testzokiiwindow.h \
        widgetforcategory.h

FORMS += \
        mainwindow.ui \
        testgiswindow.ui \
        testispdnwindow.ui \
        testzokiiwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc



