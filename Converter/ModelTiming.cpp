#include "ModelTiming.h"
#include <QTextStream>

QString Timing::toString(const QChar& space) const
    {
    return QString("%2%1%3%1%4").arg (space).
            arg (majorSeconds, 4, 'f', 1, QLatin1Char('0')).
            arg (minorQuant,   3,  10,    QLatin1Char('0')).
            arg (minorSeconds, 4, 'f', 1, QLatin1Char('0'));
    }
bool Timing::fromString(const QString & string)
    {
    QString str = string;
    majorSeconds = minorQuant = minorSeconds = -1;
    QTextStream stream(&str, QIODevice::ReadOnly);
    stream >> majorSeconds >> minorQuant >> minorSeconds;
    return (stream.status () == QTextStream::Ok &&
            majorSeconds > 0 &&
            minorQuant   > 0 &&
            minorSeconds > 0 );
    }

ModelTiming::ModelTiming(QObject *parent) :
    QAbstractListModel(parent)
    {
    }
QSet<Timing> ModelTiming::getTimingList() const
    {
    return timingList.toSet ();
    }
void ModelTiming::addTiming(const Timing & t)
    {
    if(!timingList.contains (t))
        {
        timingList << t;
        emit layoutChanged ();
        }
    }
int ModelTiming::rowCount(const QModelIndex & ) const
    {
    return timingList.size ();
    }
QVariant ModelTiming::data(const QModelIndex & index, int role) const
    {
    if(index.isValid () && role == Qt::DisplayRole)
        {
        Q_ASSERT(index.row () < timingList.size ());
        if(index.row () < timingList.size ())
            return timingList[index.row ()].toString ();
        }
    return QVariant();
    }
void ModelTiming::clear()
    {
    timingList.clear ();
    emit layoutChanged ();
    }
