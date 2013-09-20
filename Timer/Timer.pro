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

SOURCES += \
    main.cpp \
    SelectAudioFile.cpp \
    AudioFilesListModel.cpp \
    WordRepeater.cpp \
    WordsListStatistic.cpp \
    TimerMainWindow.cpp \
    DialogWordStatistics.cpp
HEADERS  += \
    SelectAudioFile.h \
    AudioFilesListModel.h \
    WordRepeater.h \
    WordsListStatistic.h \
    TimerMainWindow.h \
    DialogWordStatistics.h
FORMS    += \
    SelectAudioFile.ui \
    TimerMainWindow.ui \
    DialogWordStatistics.ui

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
