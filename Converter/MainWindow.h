/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**		Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 05.07.2013 by Zamakhov_EY.
****************************************************************************/
#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtCore>
#include <QMessageBox>
#include "DialogTiming.h"
namespace Ui {
        class MainWindow;
    }
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void signal_log(const QString& text, const bool err) const;
private slots:
    void add_Text_to_Log(const QString & text, const bool err);
    void setEnableButtons(const bool enable);

    void on_actionGenerateAudioList_triggered();
    void on_actionSplitFileList_triggered();
    void on_actionAudio_list_file_triggered();
    void on_actionInpute_audio_folder_triggered();
    void on_actionOutpute_audio_folder_triggered();
    void on_actionConvert_triggered();
    void on_pushChangeTime_clicked();
protected:
    void prepareConvertAudio(const QDir& inputeDir, const QString & file, const QDir & outputeDir);
    void convertAudio(const QDir & inputeDir, const QStringList & files, const QDir & outputeDir, const QString & outBaseName, const Timing& t);
    void setNewAudioListFile(const QStringList& newFiles);
    void addSilenseFromUI(QTextStream& stream) const;
    QSet<Timing> loadSilense(QTextStream & stream);
private:
    Ui::MainWindow *ui;
    QStringList textFiles;
    QString inputeAudioDirWay, outputeAudioDirWay;
    void sox(const QStringList & args) const;
    QString createSilentFile(const qreal duration) const;
    void addErrorToLog(const QString& text) const;
    void addInfoToLog(const QString& text) const;
    static constexpr auto sox_log_name = "sox_log.txt";
    DialogTiming dialogTiming;
};
#endif // PLAYERMAINWINDOW_H
