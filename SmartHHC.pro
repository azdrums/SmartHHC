#-------------------------------------------------
#
# Project created by QtCreator 2018-11-21T15:58:47
#
#-------------------------------------------------

QT += core
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG  += c++11 # mobility
TARGET   = smarthhc
TEMPLATE = app
# MOBILITY =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src src/ui src/ui/qsw

SOURCES += src/main.cpp \
           src/ui/qsw/menustyle.cpp \
           src/ui/qsw/actionbar.cpp \
           src/ui/qsw/levelbar.cpp \
           src/ui/qsw/spinbox.cpp \
           src/ui/qsw/tablayout.cpp \
           src/device.cpp \
           src/ui/CurveWidget.cpp \
           src/ui/CurveEditor.cpp \
           src/ui/CurvePage.cpp \
           src/ui/CurveToolBar.cpp \
           src/ui/PageAbout.cpp \
           src/ui/PageConnection.cpp \
           src/ui/PageHome.cpp \
           src/ui/PageTerminal.cpp \
           src/ui/MainWindow.cpp
HEADERS += src/ui/qsw/menustyle.h \
           src/ui/qsw/actionbar.h \
           src/ui/qsw/levelbar.h \
           src/ui/qsw/spinbox.h \
           src/ui/qsw/tablayout.h \
           src/ui/qsw/utils.h \
           src/settings.h \
           src/device.h \
           src/ui/CurveWidget.h \
           src/ui/CurveEditor.h \
           src/ui/CurveToolBar.h \
           src/ui/CurvePage.h \
           src/ui/PageAbout.h \
           src/ui/PageConnection.h \
           src/ui/PageHome.h \
           src/ui/PageTerminal.h \
           src/ui/MainWindow.h \
           src/setup.h
FORMS +=   src/ui/PageAbout.ui \
           src/ui/PageConnection.ui \
           src/ui/PageHome.ui \
           src/ui/PageTerminal.ui

RESOURCES += resources/resources.qrc

TRANSLATIONS += resources/translations/smarthhc_it.ts

DISTFILES += android/AndroidManifest.xml \
             android/res/values/libs.xml \
             android/build.gradle

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

linux:android {
    QT += bluetooth

    HEADERS += src/bluetooth.h
    SOURCES += src/bluetooth.cpp
} else {
    QT += serialport

    HEADERS += src/serial.h
    SOURCES += src/serial.cpp
}
linux:!android {
    desktop.path = /usr/share/applications/
    desktop.files = deploy/bundle/linux/smarthhc.desktop

    icon.path = /usr/share/$${TARGET}/icons/
    icon.files = resources/icons/icon.png

    translations.path = /usr/share/$${TARGET}/translations/
    translations.files = resources/translations/*.qm

    target.files = $${TARGET}
    target.path = /usr/bin

    INSTALLS += desktop icon translations target
}
win32 {
    RC_FILE = deploy/bundle/windows/icon.rc
}
mac {
    CONFIG += app_bundle
    ICON    = deploy/bundle/osx/icon.icns
}
