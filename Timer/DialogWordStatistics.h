/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**      Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 20.09.2013 by Elisey Zamakhov.
****************************************************************************/
#pragma once
#ifndef DIALOGWORDSTATISTICS_H
#define DIALOGWORDSTATISTICS_H
#include <QDialog>
#include "WordRepeater.h"
namespace Ui {
        class DialogWordStatistics;
    }
class DialogWordStatistics : public QDialog
{
    Q_OBJECT
public:
    explicit DialogWordStatistics(const WordRepeater& wordList, const bool showTimeInfo, QWidget *parent = 0);
    ~DialogWordStatistics();
private:
    Ui::DialogWordStatistics *ui;
};
#endif // DIALOGWORDSTATISTICS_H
