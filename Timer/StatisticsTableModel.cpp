/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**      Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 21.09.2013 by Elisey Zamakhov.
****************************************************************************/
#include "StatisticsTableModel.h"

StatisticsTableModel::StatisticsTableModel(const WordRepeater & wordList, QObject *parent) :
    QAbstractTableModel(parent), wordStatistics(wordList)
    {
    }
int StatisticsTableModel::rowCount(const QModelIndex &) const
    {
    return wordStatistics.roundCounts ();
    }
int StatisticsTableModel::columnCount(const QModelIndex &) const
    {
    return 3;
    }
QVariant StatisticsTableModel::data(const QModelIndex & index, int role) const
    {
    if(index.isValid ())
        {
        //set data Alignment
        if(role == Qt::TextAlignmentRole)
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        if(role == Qt::DisplayRole)
            {
            const WordsListStatistic &roundInfo = wordStatistics.getRound (index.row ());
            const int all = roundInfo.countWords ();
            switch (index.column ()) {
                case 0:
                    {
                    const int err = roundInfo.countErrors ();
                    return  QString("%1 (%2%)").arg (err).arg (qreal(err)*100/all, 4, 'f', 1);
                    }
                case 1:
                    {
                    const int corr = roundInfo.countCorrects ();
                    return  QString("%1 (%2%)").arg (corr).arg (qreal(corr)*100/all, 4, 'f', 1);
                    }
                case 2:
                    return  QString("%1").arg (all);
                default:
                    Q_UNIMPLEMENTED();
                    break;
                }
            }
        }
    return QVariant();
    }
QVariant StatisticsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
    if(role == Qt::DisplayRole && Qt::Horizontal == orientation)
        switch (section) {
            case 0:
                return tr("Error");
            case 1:
                return tr("Correct");
            case 2:
                return tr("All");
            default:
                Q_UNIMPLEMENTED();
                break;
        }
    return QAbstractTableModel::headerData (section, orientation, role);
    }
