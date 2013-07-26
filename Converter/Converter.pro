#-------------------------------------------------
#
# Project created by QtCreator 2013-07-05T20:37:08
#
#-------------------------------------------------

TARGET   = Converter
QT      += core gui widgets multimedia concurrent
CONFIG  += exceptions c++11
TEMPLATE = app

TRANSLATIONS = Converter_ru.ts
RC_FILE   += res/advance_icon.rc

SOURCES += main.cpp\
        MainWindow.cpp \
    DialogTiming.cpp \
    ModelTiming.cpp

HEADERS  += MainWindow.h \
    DialogTiming.h \
    ModelTiming.h

FORMS    += MainWindow.ui \
    DialogTiming.ui

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

RESOURCES += \
    Converter.qrc
