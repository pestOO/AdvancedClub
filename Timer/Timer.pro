#-------------------------------------------------
#
# Project created by QtCreator 2013-03-27T21:24:52
#
#-------------------------------------------------

TARGET   = Timer
QT      += core gui widgets multimedia
CONFIG  += exceptions c++11
TEMPLATE = app

TRANSLATIONS = Timer_ru.ts
RC_FILE   += res/advance_icon.rc

SOURCES += main.cpp\
        TimerMainWindow.cpp \
    SelectAudioFile.cpp \
    AudioFilesListModel.cpp
HEADERS  += TimerMainWindow.h \
    SelectAudioFile.h \
    AudioFilesListModel.h
FORMS    += TimerMainWindow.ui \
    SelectAudioFile.ui

RESOURCES += \
    sounds.qrc

QMAKE_CXXFLAGS += \
    -Winit-self \
    -Wextra \
    -Wnon-virtual-dtor  \ #Must be - check virtual destructors for classes
#    -Wold-style-cast    \ #old cast is bad style
    -Woverloaded-virtual\
    -pedantic \
    -Wunreachable-code \
#    -Wctor-dtor-privacy \
#    -Weffc++ \	#a lot of warn on QObjects
#    -Wconversion -Wsign-conversion \  #try check Qt libs
    -Wall
