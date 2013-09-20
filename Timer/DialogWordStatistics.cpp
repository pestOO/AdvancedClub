/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**      Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 20.09.2013 by Elisey Zamakhov.
****************************************************************************/
#include "DialogWordStatistics.h"
#include "ui_DialogWordStatistics.h"

DialogWordStatistics::DialogWordStatistics(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWordStatistics)
    {
    ui->setupUi(this);
    ui->buttonBox->addButton (tr("Ok"), QDialogButtonBox::RejectRole);
    }
DialogWordStatistics::~DialogWordStatistics()
    {
    delete ui;
    }
void DialogWordStatistics::setWordRepeater(const WordRepeater & wordList)
    {
    }
