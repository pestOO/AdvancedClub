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
