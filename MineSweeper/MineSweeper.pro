QMAKE = ./qt-static-5.15.2/bin/qmake.exe
QMAKE_CXXFLAGS += /utf-8
QT += core gui widgets multimedia multimediawidgets
TARGET = MineSweeper
TEMPLATE = app
SOURCES += ./src/main.cpp
SOURCES += ./src/MineSweeperGUI.cpp
SOURCES += ./src/MineSweeperCore.cpp
SOURCES += ./src/BoardCore.cpp
SOURCES += ./src/CellCore.cpp
HEADERS += ./src/MineSweeperGUI.h
HEADERS += ./src/MineSweeperCore.h
HEADERS += ./src/BoardCore.h
HEADERS += ./src/CellCore.h
CONFIG += console