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
    void updatetLabelTime();
    void tick();
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_actionExit_triggered();
    void on_actionSelect_audio_triggered();
    void on_actionChoode_text_file_triggered();
    void on_actionClear_sound_triggered();
    void on_actionClear_text_file_triggered();
    void on_actionChange_text_font_triggered();
    void on_buttonPause_clicked();

protected:
    qreal getmSecsByBoxs() const;
    qreal getSecsByBoxs() const;
    void setAudioFile(const QString& file);
    void play();
private:
    Ui::TimerMainWindow *ui;
    QTime time;
    int pause_left_msecs = 0;
    const QString soundWay = QLatin1Literal("audio.wav");
    QMediaPlayer player;
    QTimer* taskTimer   = {new QTimer(this)};
    QTimer* updateTimer = {new QTimer(this)};
    const QLatin1Literal settingTextWay     = QLatin1Literal("TextWay");
    const QLatin1Literal settingConstSecs   = QLatin1Literal("ConstSecs");
    const QLatin1Literal settingLetterSecs  = QLatin1Literal("LetterSecs");
    const QLatin1Literal settingWordFont    = QLatin1Literal("WordFont");
    QFile textFile;
    QTextStream fileTextStream;
    QSettings settings;
};
#endif // TIMERMAINWINDOW_H
