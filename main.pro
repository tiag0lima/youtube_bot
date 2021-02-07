#####################################################
#                                                   #
#                                                   #
#   this document it's a test from tiago lima       #
#                                                   #
#                                                   #
#####################################################

TEMPLATE = app
QT = core gui widgets quickwidgets quick webview network
HEADERS = include/widget.h include/VideoWatcher.h  include/VideoEditor.h
SOURCES = src/widget.cpp src/VideoWatcher.cpp src/VideoEditor.cpp src/main.cpp
RESOURCES = resource.qrc
TARGET = main

INCLUDEPATH += include
MOC_DIR = build
OBJECTS_DIR = build
