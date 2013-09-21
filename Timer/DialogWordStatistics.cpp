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
        const QSize minimuSize = QSize(qMax(parentGeometry.width()/4, w->width()*6/5), w->height());
        w->setMinimumSize(minimuSize);
        //set new size and position in cent of parent widget
        w->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w->size(), parentGeometry));
        }
    else
        qWarning() << QLatin1Literal("no parent widget!");
    }

DialogWordStatistics::DialogWordStatistics(const WordRepeater& wordList, QWidget *parent) :
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

    prepareDialog (this);
    }
DialogWordStatistics::~DialogWordStatistics()
    {
    delete ui;
    }
