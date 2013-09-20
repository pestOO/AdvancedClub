/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**		Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 27.03.2013 by Elisey Zamakhov.
****************************************************************************/
#pragma once
#ifndef TIMERMAINWINDOW_H
#define TIMERMAINWINDOW_H
#include <QMainWindow>
#include <QtCore>
#include <QMediaPlayer>
#include "WordRepeater.h"

namespace Ui {
        class TimerMainWindow;
    }
class TimerMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit TimerMainWindow(QWidget *parent = 0);
    ~TimerMainWindow();
protected:
    void enableButtons(const bool isRunning);
private slots:
    void showPlayerError(QMediaPlayer::Error err);
    void setLabelTime(const int mSecs);
    //update GUI data by micro-timer
    void updatetLabelTime();
    //timer is off - mark word as error and go next
    void tick();
    //make the next action (show word, play sound)
    void nextAction();
    void checkPlayPauseButton();
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void stopTimers(const bool reset);
    void on_actionExit_triggered();
    void on_actionSelect_audio_triggered();
    void on_actionChoode_text_file_triggered();
    void on_actionClear_sound_triggered();
    void on_actionClear_text_file_triggered();
    void on_actionChange_text_font_triggered();
    void on_buttonPause_clicked();
    void on_buttonNext_clicked();
protected:
    qreal getmSecsByBoxs() const;
    qreal getSecsByBoxs() const;
    void setAudioFile(const QString& file);
    void setLabelWord(const QString& word);
    void clearLabelWord();
    void play();
private:
    Ui::TimerMainWindow *ui;
    QTime time;
    int pause_left_msecs = 0;
    const QString soundWay = QLatin1Literal("audio.mp3");
    QMediaPlayer player;
    QTimer* taskTimer   = {new QTimer(this)};
    QTimer* updateTimer = {new QTimer(this)};
    const QLatin1Literal settingTextWay     = QLatin1Literal("TextWay");
    const QLatin1Literal settingConstSecs   = QLatin1Literal("ConstSecs");
    const QLatin1Literal settingLetterSecs  = QLatin1Literal("LetterSecs");
    const QLatin1Literal settingWordFont    = QLatin1Literal("WordFont");
    QSettings settings;
    WordRepeater wordlist;
};
#endif // TIMERMAINWINDOW_H
