#-------------------------------------------------
#
# Project created by QtCreator 2019-12-30T09:29:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ui
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

CONFIG += c++11


win32 {
    contains(QT_ARCH, i386) {
        CONFIG(release, debug|release) {
            DESTDIR += ../bin/x86/release
            LIBS += -L$$PWD/../bin/x86/release/ -lImageProc
            LIBS += -L$$PWD/../opencv/x86/lib/-lopencv_core2413 -lopencv_highgui2413 -lopencv_imgproc2413
        }
        CONFIG(debug, debug|release) {
            DESTDIR += ../bin/x86/debug
            LIBS += -L$$PWD/../bin/x86/debug/ -lImageProc
            LIBS += -L$$PWD/../opencv/x86/lib/ -lopencv_core2413d -lopencv_highgui2413d -lopencv_imgproc2413d
        }
    } else {
        CONFIG(release, debug|release) {
            DESTDIR += ../bin/x64/release
            LIBS += -L$$PWD/../bin/x64/release/ -lImageProc
            LIBS += -L$$PWD/../opencv/x64/lib/ -lopencv_core2413 -lopencv_highgui2413 -lopencv_imgproc2413
        }
        CONFIG(debug, debug|release) {
            DESTDIR += ../bin/x64/debug
            LIBS += -L$$PWD/../bin/x64/debug/ -lImageProc
            LIBS += -L$$PWD/../opencv/x64/lib/ -lopencv_core2413d -lopencv_highgui2413d -lopencv_imgproc2413d
        }
    }
}

INCLUDEPATH += $$PWD/../ImageProc
INCLUDEPATH += $$PWD/../opencv/include




SOURCES += \
        main.cpp \
        procthread.cpp \
        widget.cpp

HEADERS += \
        StopWatch.h \
        filetools.h \
        procthread.h \
        widget.h

FORMS += \
        widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
