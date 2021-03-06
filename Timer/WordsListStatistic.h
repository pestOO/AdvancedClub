/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**      Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 20.09.2013 by Elisey Zamakhov.
****************************************************************************/
#pragma once
#ifndef WORDSLISTSTATISTIC_H
#define WORDSLISTSTATISTIC_H
#include <QtCore>
#include <QLinkedList>
//Class provides iteration on word list with marking them correct/error
//Load all word by @addWord or create by previous errors
class WordsListStatistic {
public:
    typedef QList<QString> StringList;
    typedef QPair<qreal,qreal> PairTiming;
    WordsListStatistic();
    WordsListStatistic(const StringList & newWords);
    void addWord(const QString & word);
    //count of all words (correct, uncorrect, unmarked)
    int countWords() const;
    int countErrors() const;
    int countCorrects() const;
    //has one more unmarked word
    bool hasWord();
    //return current unmared word
    QString word() const;
    //set word un/correct and change current
    void gotoNextWord(const bool currentIsCorrect = false, const qreal time = 0.0);
    //no words was loaded
    bool isEmpty() const;

    //has errors for next round
    bool hasNextRound() const;
    //return words for next round
    StringList getWordsError() const;

    void setTiming(PairTiming time);
    PairTiming getTiming() const;

    qreal meanTimeCorrect() const;
private:
    StringList words;
    StringList wordsError;
    StringList wordsCorrect;
    PairTiming timing;
    QList<qreal> correctTimes;
};
#endif // WORDSLISTSTATISTIC_H
