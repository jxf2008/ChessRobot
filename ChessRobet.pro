#-------------------------------------------------
#
# Project created by QtCreator 2019-12-10T15:52:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChessRebot
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

QT += sql

SOURCES += \
        AI/AnalysisFunction.cpp \
        AI/AIThread.cpp \
        GUI/ChessButton.cpp \
        GUI/ChessMapWidget.cpp \
        GUI/ChoosePlayerDialog.cpp \
        GUI/GameReplayDialog.cpp \
        Model/MissionDataModel.cpp \
        GUI/SaveGameDataDialog.cpp \
        main.cpp \
        GUI/ChessRobert.cpp \
        Init/InitBuilder.cpp

HEADERS += \
        AI/AnalysisFunction.h \
        AI/AIThread.h \
        GUI/ChessButton.h \
        GUI/ChessMapWidget.h \
        GUI/ChessRobert.h \
        GUI/ChoosePlayerDialog.h \
        ConstData.h \
        GUI/GameReplayDialog.h \
        Init/ConstData.h \
        Init/InitBuilder.h \
        Init/ConstData.h \
        Model/MissionDataModel.h \
        GUI/SaveGameDataDialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = images/MyExe.ico

RESOURCES += \
    source.qrc
