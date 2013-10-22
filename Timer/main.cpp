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

    QApplication app(argc, argv);
#ifdef Q_OS_WIN
    QApplication::addLibraryPath(QLatin1Literal("plugins"));
#elif defined Q_OS_MAC
    QDir dir(QApplication::applicationDirPath());
    dir.cdUp();
    dir.cd("Plugins");
    QApplication::addLibraryPath(dir.absolutePath());
#else
    Q_STATIC_ASSERT("not implemented for this platform");
#endif
    app.setWindowIcon(QIcon(":/icon.ico"));
    QTranslator ru_translation;
    if(!ru_translation.load("Timer_ru", ":/"))
        qWarning() << "no ru transaltion";
    app.installTranslator(&ru_translation);
    TimerMainWindow w;
    w.showMaximized ();
    return app.exec();
    }
