#-------------------------------------------------
#
# Project created by QtCreator 2013-03-17T12:22:15
#
#-------------------------------------------------

TARGET   = Funnel
QT      += core gui widgets
CONFIG  += exceptions c++11
TEMPLATE = app

TRANSLATIONS = Funnel_ru.ts
RC_FILE   += res/advance_icon.rc

SOURCES +=  main.cpp\
            MainWindow.cpp \
            TestFunnel.cpp
HEADERS  += MainWindow.h \
            TestFunnel.h
FORMS    += MainWindow.ui
RESOURCES+= resources.qrc

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
