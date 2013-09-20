/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**		Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 20.09.2013 by Elisey Zamakhov.
****************************************************************************/
#pragma once
#ifndef WORDREPEATER_H
#define WORDREPEATER_H
#include <QtCore>
#include "WordsListStatistic.h"

class WordRepeater
{
public:
    WordRepeater() {}
    void load(const QString& filename);

    //count of all words (correct, uncorrect, unmarked)
    //on current step
    int countWords() const;
    //has one more unmarked word
    bool hasNext();
    //return current unmared word
    QString word() const;
    //set word un/correct and change current
    void setErrorState(const bool isCorrect);
    //no words was loaded
    bool isEmpty() const;

    //has errors for next round
    bool hasNextRound() const;
    //start new round
    void startNewRound();
    void resetRound();

    //clear all data;
    void clear();
    //resert to orogonal state
private:
    Q_DISABLE_COPY(WordRepeater)
    WordsListStatistic originalWordList;
    QList<WordsListStatistic> wordListsStatistics;
};
#endif // WORDREPEATER_H
