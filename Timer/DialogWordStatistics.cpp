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
#include "StatisticsTableModel.h"
#include <QAbstractButton>
#include <QApplication>
#include <QFont>

void prepareDialog(QWidget* const w)
    {
    if(w->parentWidget()) {
        const QRect& parentGeometry = w->parentWidget()->geometry();
        w->setMinimumSize(0,0);
        w->adjustSize();
        const QSize minimuSize = QSize(qMax(parentGeometry.width()/3, w->width()*6/5), w->height());
        w->setMinimumSize(minimuSize);
        //set new size and position in cent of parent widget
        w->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w->size(), parentGeometry));
        }
    else
        qWarning() << QLatin1Literal("no parent widget!");
    }

DialogWordStatistics::DialogWordStatistics(const WordRepeater& wordList, const bool showTimeInfo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWordStatistics)
    {
    ui->setupUi(this);
    ui->buttonBox->addButton (tr("Ok"), QDialogButtonBox::RejectRole);

    QAbstractButton* cornerButton = ui->tableView->findChild<QAbstractButton*>();
    Q_ASSERT(cornerButton);
    cornerButton->setEnabled(false);
    cornerButton->setText(tr(" Round "));
    cornerButton->installEventFilter(this);

    ui->tableView->setModel( new StatisticsTableModel(wordList));
    ui->tableView->clearSelection ();

    QFont appFont = qApp->font();
    appFont.setFamily ("Lucida Console");
    ui->tableView->setFont (appFont);

    const int roundCurrent = wordList.currentRound ();
    const int roundMax = wordList.getMaxRounds ();
    ui->labelRound->setText (QString("%1 / %2").arg (roundCurrent).arg (roundMax));
    if(roundCurrent >= roundMax)
        {
        QFont font = ui->labelRound->font ();
        font.setBold (true);
        ui->labelRound->setFont (font);
        }
    if(!showTimeInfo)
        ui->labelInfo->clear ();
    prepareDialog (this);
    }
DialogWordStatistics::~DialogWordStatistics()
    {
    delete ui;
    }
