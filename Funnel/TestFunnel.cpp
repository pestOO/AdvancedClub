/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**		Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** This file is part of the Rakurs-4 application.
**
** Created 17.03.2013 by pesto.
****************************************************************************/
#include "TestFunnel.h"

void TestFunnel::OpenTest(const QString &fileName)  throw(QString)
    {
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
        {
        QJsonParseError err;
        const  QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &err);
        if(err.error != QJsonParseError::NoError)
            throw err.errorString();
        if(!jsonDoc.isArray() || jsonDoc.isEmpty())
            throw qApp->translate("TestFunnel", "Wrong file format");
        tests = jsonDoc.array();
        errorsList.clear();
        errorsList.append(IndexSetType());
        IndexSetType& set = errorsList.last();
        for (int i = 0; i < tests.size(); ++i)
            set.insert(i);
        nextRound();
        //FIXME: add checking all data
        };
    }
int TestFunnel::countTasks() const
    {
    return lastErorrsSet().size();
    }
int TestFunnel::countErrors() const
    {
    return currentTasksSet().size();
    }
int TestFunnel::isLastTask() const
    {
    return testTaskIndexIterrator == --lastErorrsSet().end();
    }
void TestFunnel::addCurrentAsMistake()
    {
    errorsList.last().insert(*testTaskIndexIterrator);
    }
const Task TestFunnel::taskAt(const int i) const
    {
    return getTask(tests.at(i).toObject());
    }
const Task TestFunnel::currentTask() const
    {
    return getTask(tests.at(*testTaskIndexIterrator).toObject());
    }
const Task TestFunnel::nextTask()
    {
    if(isLastTask())
        throw qApp->translate("TestFunnel", "No tasks");
    return getTask(tests.at(*++testTaskIndexIterrator).toObject());
    }
void TestFunnel::nextRound()
    {
//    qDebug() << errorsList.last();
    errorsList.append(IndexSetType());
    testTaskIndexIterrator = lastErorrsSet().begin();
    }
const TestFunnel::IndexSetType &TestFunnel::currentTasksSet() const
    {
    return errorsList.last();
    }
const TestFunnel::IndexSetType &TestFunnel::lastErorrsSet() const
    {
    return errorsList.at(errorsList.size()-2);
    }
const Task TestFunnel::getTask(const QJsonObject taskObject)
    {
    const QJsonValue question = taskObject.value(QLatin1Literal("Q"));
    const QJsonValue answer   = taskObject.value(QLatin1Literal("A"));
    const QJsonValue hint     = taskObject.value(QLatin1Literal("H"));
    return Task{question.toString(), answer.toString(), hint.toString()};
    }
QDebug operator<<(QDebug dbg, const std::set<int> &set)
    {
    dbg.nospace() << "{ ";
    const auto end = --set.end();
    for(auto it=set.begin(); it!=end; ++it)
        dbg.nospace() << *it << ", ";
    return dbg.nospace() << *set.end() << " }";
    }
