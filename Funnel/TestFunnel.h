/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**		Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 17.03.2013 by pesto.
****************************************************************************/
#pragma once
#ifndef TESTFUNNEL_H
#define TESTFUNNEL_H
#include <QtCore>
#include <set>

struct Task {
    QString question;
    QString answer;
    QString hint;
};
class TestFunnel
{
    typedef std::set<int> IndexSetType;
public:
    explicit TestFunnel() {}
    void OpenTest(const QString& fileName) throw(QString);
    int countTasks() const;
    int countErrors() const;
    int isLastTask() const;
    void addCurrentAsMistake();
    const Task taskAt(const int i) const;
    const Task currentTask() const;
    const Task nextTask();

    void nextRound();
protected:
    const IndexSetType& currentTasksSet() const;
    const IndexSetType& lastErorrsSet() const;
    static const Task getTask(const QJsonObject taskObject);
private:
    QJsonArray tests;
    QJsonArray::Iterator currentTaskIterator;
    QList<IndexSetType> errorsList;
    IndexSetType::const_iterator testTaskIndexIterrator;
};
QDebug operator<<(QDebug dbg, const std::set<int> &set);
#endif // TESTFUNNEL_H
