/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**      Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 20.09.2013 by Elisey Zamakhov.
****************************************************************************/
#include "WordRepeater.h"

void WordRepeater::load(const QString & filename)
    {
    QFile textFile(filename);
    textFile.open (QFile::ReadOnly);
    QTextStream fileTextStream(&textFile);

    WordsListStatistic newWordList;
    while(!fileTextStream.atEnd ())
        {
        const QString text = fileTextStream.readLine().trimmed();
        if(text.isEmpty ())
            continue;
        newWordList.addWord (text);
        }

    originalWordList = newWordList;
    wordListsStatistics.clear ();
    }
int WordRepeater::countWords() const
    {
    if(wordListsStatistics.isEmpty ())
        return originalWordList.countWords ();
    return wordListsStatistics.last ().countWords ();
    }
bool WordRepeater::hasNext()
    {
    Q_ASSERT(!wordListsStatistics.isEmpty ());
    if(wordListsStatistics.isEmpty ())
        return false;
    return wordListsStatistics.last ().hasNext ();
    }
QString WordRepeater::word() const
    {
    Q_ASSERT(!wordListsStatistics.isEmpty ());
    if(wordListsStatistics.isEmpty ())
        return QString();
    return wordListsStatistics.last ().word ();
    }
void WordRepeater::setErrorState(const bool isCorrect)
    {
    Q_ASSERT(!wordListsStatistics.isEmpty ());
    if(!wordListsStatistics.isEmpty ())
        wordListsStatistics.last ().setErrorState (isCorrect);
    }
bool WordRepeater::isEmpty() const
    {
    return originalWordList.isEmpty ();
    }
bool WordRepeater::hasNextRound() const
    {
    if(wordListsStatistics.isEmpty ())
        return originalWordList.hasNextRound ();
    return wordListsStatistics.last ().hasNextRound ();
    }
void WordRepeater::startNewRound()
    {
    if(wordListsStatistics.isEmpty ())
        wordListsStatistics.append (originalWordList);
    else
        {
        WordsListStatistic newWordList(wordListsStatistics.last ().getWordsError ());
        wordListsStatistics.append (newWordList);
        }
    }
void WordRepeater::resetRound()
    {
    wordListsStatistics.removeLast ();
    startNewRound();
    }
void WordRepeater::clear()
    {
    wordListsStatistics.clear ();
    }
