#-------------------------------------------------
#
# Project created by QtCreator 2019-12-30T09:25:30
#
#-------------------------------------------------

QT       -= core gui

TARGET = ImageProc
TEMPLATE = lib

DEFINES += IMAGEPROC_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


win32 {
    contains(QT_ARCH, i386) {
        CONFIG(release, debug|release) {
            DESTDIR += ../bin/x86/release
            LIBS += -L$$PWD/../opencv/x86/lib/ -lopencv_core2413 -lopencv_highgui2413 -lopencv_imgproc2413
        }
        CONFIG(debug, debug|release) {
            DESTDIR += ../bin/x86/debug
            LIBS += -L$$PWD/../opencv/x86/lib/ -lopencv_core2413d -lopencv_highgui2413d -lopencv_imgproc2413d
        }
    } else {
        CONFIG(release, debug|release) {
            DESTDIR += ../bin/x64/release
            LIBS += -L$$PWD/../opencv/x64/lib/ -lopencv_core2413 -lopencv_highgui2413 -lopencv_imgproc2413
        }
        CONFIG(debug, debug|release) {
            DESTDIR += ../bin/x64/debug
            LIBS += -L$$PWD/../opencv/x64/lib/ -lopencv_core2413d -lopencv_highgui2413d -lopencv_imgproc2413d
        }
    }
}

INCLUDEPATH += $$PWD/../opencv/include


SOURCES += \
    ImageApply.cpp \
    ImageApplyAdjustColors.cpp \
    ImageApplyAutoCrop.cpp \
    ImageApplyBWBinaray.cpp \
    ImageApplyChannel.cpp \
    ImageApplyCrop.cpp \
    ImageApplyDiscardBlank.cpp \
    ImageApplyOutHole.cpp \
    ImageApplyResize.cpp \
    ImageApplyRotation.cpp \
    ImageApplySharpen.cpp \
    ImageApplyTextOrientation.cpp \
    ImageApplyThreshold.cpp \
    ImageProcess_Public.cpp

HEADERS += \
    ImageApply.h \
    ImageApplyAdjustColors.h \
    ImageApplyAutoCrop.h \
    ImageApplyBWBinaray.h \
    ImageApplyChannel.h \
    ImageApplyCrop.h \
    ImageApplyDiscardBlank.h \
    ImageApplyHeaders.h \
    ImageApplyOutHole.h \
    ImageApplyResize.h \
    ImageApplyRotation.h \
    ImageApplySharpen.h \
    ImageApplyTextOrientation.h \
    ImageApplyThreshold.h \
    ImageProcess_Public.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}



