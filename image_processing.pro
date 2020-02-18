#-------------------------------------------------
#
# Project created by QtCreator 2020-02-17T14:11:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = image_processing
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/include
INCLUDEPATH += /usr/local/include/opencv2

LIBS += -L/usr/lib/x86_64-linux-gnu \
-lopencv_core \
-lopencv_imgproc \
-lopencv_imgcodecs \
-lopencv_highgui \
-lopencv_ml \
-lopencv_video \
-lopencv_features2d \
-lopencv_calib3d \
-lopencv_objdetect \
-lopencv_contrib \
-lopencv_legacy \
-lopencv_flann \
-lopencv_gpu \
-lopencv_photo \
-lopencv_stitching \
-lopencv_ts \
-lopencv_videostab



