#ifndef MODELTIMING_H
#define MODELTIMING_H
#include <QAbstractListModel>
#include <QSet>

struct Timing {
    qreal majorSeconds{2.0};
    qreal minorSeconds{2.0};
    int minorQuant{1};
    QString toString(const QChar & space = QChar::Space) const;
    bool fromString(const QString& t);
};
inline bool operator==(const Timing &t1, const Timing &t2)
    {
    return  qFuzzyCompare (t1.majorSeconds, t2.majorSeconds) &&
            qFuzzyCompare (t1.minorSeconds, t2.minorSeconds) &&
            (t1.minorQuant == t2.minorQuant);
    }
inline uint qHash(const Timing &t, uint seed)
    {
    return qHash(t.toString(), seed);
    }

class ModelTiming : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ModelTiming(QObject *parent = 0);
    QSet<Timing> getTimingList() const;
    void addTiming(const Timing& t);
    int rowCount (const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    void clear();
private:
    QList<Timing> timingList;
};
#endif // MODELTIMING_H
