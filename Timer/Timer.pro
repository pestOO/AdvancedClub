#-------------------------------------------------
#
# Project created by QtCreator 2013-03-27T21:24:52
#
#-------------------------------------------------

TARGET   = Timer
QT      += core gui widgets multimedia xml
CONFIG  += exceptions c++11
TEMPLATE = app

TRANSLATIONS = Timer_ru.ts
RC_FILE   += res\advance_icon.rc

ICON = res/advance.icns

SOURCES += \
    main.cpp \
    SelectAudioFile.cpp \
    AudioFilesListModel.cpp \
    WordRepeater.cpp \
    WordsListStatistic.cpp \
    DialogWordStatistics.cpp \
    StatisticsTableModel.cpp \
    TimerMainWindow.cpp
HEADERS  += \
    SelectAudioFile.h \
    AudioFilesListModel.h \
    WordRepeater.h \
    WordsListStatistic.h \
    DialogWordStatistics.h \
    StatisticsTableModel.h \
    TimerMainWindow.h
FORMS    += \
    SelectAudioFile.ui \
    DialogWordStatistics.ui \
    TimerMainWindow.ui

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

OTHER_FILES += \
    CreateDMG.sh

macx:warning(Don\'t forget add build step \"bash %{sourceDir}/CreateDMG.sh Timer.app\");
