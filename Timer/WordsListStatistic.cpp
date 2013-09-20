/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**      Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 20.09.2013 by Elisey Zamakhov.
****************************************************************************/
#include "WordsListStatistic.h"

WordsListStatistic::WordsListStatistic()
    {
    }
WordsListStatistic::WordsListStatistic(const WordsListStatistic::StringList & newWords) : words(newWords)
    {
    }
void WordsListStatistic::addWord(const QString & word)
    {
    words.append (word);
    }
int WordsListStatistic::countWords() const
    {
    return words.count () + wordsCorrect.count () + wordsError.count ();
    }
int WordsListStatistic::countErrors() const
    {
    return wordsError.count ();
    }
int WordsListStatistic::countCorrects() const
    {
    return wordsCorrect.count ();
    }
bool WordsListStatistic::hasNext()
    {
    return !words.isEmpty ();
    }
QString WordsListStatistic::word() const
    {
    Q_ASSERT(!words.isEmpty());
    return words.first ();
    }
void WordsListStatistic::setErrorState(const bool isCorrect)
    {
    Q_ASSERT(!words.isEmpty());
    if(words.isEmpty())
        return;
    if(isCorrect)
        wordsCorrect.append (words.takeFirst ());
    else
        wordsError.append (words.takeFirst ());
    }
bool WordsListStatistic::isEmpty() const
    {
    return countWords() <= 0;
    }
bool WordsListStatistic::hasNextRound() const
    {
    return !wordsError.isEmpty () || !words.isEmpty ();
    }
WordsListStatistic::StringList WordsListStatistic::getWordsError() const
    {
    Q_ASSERT(words.isEmpty());
    Q_ASSERT(!wordsError.isEmpty());
    return wordsError;
    }
