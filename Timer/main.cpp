/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**		Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 27.03.2013 by Elisey Zamakhov.
****************************************************************************/
#include <QApplication>
#include <QtCore>
#include "TimerMainWindow.h"

int main(int argc, char *argv[])
    {
    //set global names
    QApplication::setOrganizationName   (QLatin1Literal("Advance"));
    QApplication::setApplicationName    (QLatin1Literal("Timer"));
    QApplication::setApplicationVersion (QLatin1Literal("0.1"));
    QApplication::setOrganizationDomain (QLatin1Literal("http://www.advance-club.ru/"));
    qDebug() << QApplication::applicationName() + QLatin1Literal(" starts.");

    QApplication::addLibraryPath(QLatin1Literal("plugins"));
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon.ico"));
    QTranslator ru_translation;
    if(!ru_translation.load("Timer_ru", ":/"))
        qWarning() << "no ru transaltion";
    app.installTranslator(&ru_translation);
    TimerMainWindow w;
    w.showMaximized ();
    return app.exec();
    }
