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
