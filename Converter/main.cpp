/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**		Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 05.07.2013 by Zamakhov_EY.
****************************************************************************/
#include <QApplication>
#include <QtCore>
#include "MainWindow.h"

int main(int argc, char *argv[])
    {
    //set global names
    QApplication::setOrganizationName   (QLatin1Literal("Advance"));
    QApplication::setApplicationName    (QLatin1Literal("Converter"));
    QApplication::setApplicationVersion (QLatin1Literal("0.1"));
    QApplication::setOrganizationDomain (QLatin1Literal("http://www.advance-club.ru/"));
    qDebug() << QApplication::applicationName() + QLatin1Literal(" starts.");


    QApplication::addLibraryPath(QLatin1Literal("plugins"));
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon.ico"));
    QTranslator translator_qt_ru;
    if(!translator_qt_ru.load("qt_ru", ":/"))
        qWarning() << "no qt_ru transaltion";
    app.installTranslator(&translator_qt_ru);
    QTranslator translator_own;
    if(!translator_own.load("Player_ru", ":/"))
        qWarning() << "no ru transaltion";
    app.installTranslator(&translator_own);
    MainWindow w;
    w.show();
    return app.exec();
    }
