#-------------------------------------------------
#
# Project created by QtCreator 2018-10-18T11:21:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HumanFace
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    tps.cpp \
    interpolation.cpp \
    decomposition.cpp

HEADERS  += mainwindow.h \
    tps.h \
    interpolation.h \
    decomposition.h

FORMS    += mainwindow.ui

#INCLUDEPATH += -I  C:\Python27\include
#LIBS += -L C:\Python27\libs\ -lpython27
#INCLUDEPATH += -I  C:\Program Files\Python36\include
#LIBS += -L C:\Program Files\Python36\libs\ -lpython36

#INCLUDEPATH += -I D:\program\Anaconda3\include
#LIBS += -L D:\program\Anaconda3\libs\ -lpython36

#INCLUDEPATH += d:\program\opencv\build\include\
#INCLUDEPATH += d:\program\opencv\build\include\opencv\
#INCLUDEPATH += d:\program\opencv\build\include\opencv2\
#LIBS += D:\program\opencv\build\x64\vc14\lib\*.lib
#CONFIG(debug,debug|release) {
#LIBS += -Ld:\program\opencv\build\x64\vc12\lib \
#    -lopencv_core2410d \
#    -lopencv_highgui2410d \
#    -lopencv_imgproc2410d \
#    -lopencv_features2d2410d \
#    -lopencv_calib3d2410d
#} else {
#LIBS += -Ld:\program\opencv\build\x64\vc12\lib \
#    -lopencv_core2410 \
#    -lopencv_highgui2410 \
#    -lopencv_imgproc2410 \
#    -lopencv_features2d2410 \
#    -lopencv_calib3d2410
#}

RESOURCES += \
    ico.qrc
RC_FILE=icon.rc
