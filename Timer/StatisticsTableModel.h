/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**      Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 21.09.2013 by Elisey Zamakhov.
****************************************************************************/
#pragma once
#ifndef STATISTICSTABLEMODEL_H
#define STATISTICSTABLEMODEL_H
#include <QAbstractTableModel>
#include "WordRepeater.h"
class StatisticsTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    StatisticsTableModel(const WordRepeater& wordList, QObject *parent = 0);

    int rowCount (const QModelIndex &) const override;
    int columnCount (const QModelIndex &) const override;
    QVariant data (const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
private:
    const WordRepeater& wordStatistics;
};
#endif // STATISTICSTABLEMODEL_H
