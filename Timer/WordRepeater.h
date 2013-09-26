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
    int countErrors() const;
    int countCorrects() const;
    //has one more unmarked word
    bool hasNext();
    //return current unmared word
    QString word() const;
    //set word un/correct and change current
    void setErrorState(const bool isCorrect);
    //no words was loaded
    bool isEmpty() const;

    //set/get maximum count of rounds
    void setMaxRounds(const int rounds);
    int getMaxRounds() const;

    //has errors for next round
    bool hasNextRound() const;
    //start new round
    void startNewRound();
    void setRoundTimings(const QPair<qreal, qreal> timing);
    void resetRound();
    //return count of started rounds
    int currentRound() const;

    //statistics getters
    int countsRounds() const;
    const WordsListStatistic& getRound(int i) const;

    //clear all data;
    void clear();

    static const int maxRoundsDefault {3};
private:
    Q_DISABLE_COPY(WordRepeater)
    WordsListStatistic originalWordList;
    QList<WordsListStatistic> wordListsStatistics;
    WordsListStatistic& currentList();
    const WordsListStatistic& currentList() const;
    int maxRounds{maxRoundsDefault};
};
#endif // WORDREPEATER_H
