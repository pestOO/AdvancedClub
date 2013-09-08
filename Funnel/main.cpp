/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**		Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 17.03.2013 by pesto.
****************************************************************************/
#include <QApplication>
#include <QtCore>
#include "MainWindow.h"

int main(int argc, char *argv[])
    {
    //set global names
    QApplication::setOrganizationName   (QLatin1Literal("Advance"));
    QApplication::setApplicationName    (QLatin1Literal("Funnel"));
    QApplication::setApplicationVersion (QLatin1Literal("0.1"));
    QApplication::setOrganizationDomain (QLatin1Literal("http://www.advance-club.ru/"));
    qDebug() << QApplication::applicationName() + QLatin1Literal(" starts.");

    QApplication::addLibraryPath(QLatin1Literal("plugins"));
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon.ico"));
    QTranslator translator;
    if(!translator.load("Funnel_ru", ":/"))
        qWarning() << "no ru transaltion";
    app.installTranslator(&translator);
    MainWindow w;
    w.show();
    return app.exec();
    }
