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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include "TestFunnel.h"

namespace Ui {
    class MainWindow;
    }
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    enum buttonStates {
        startState,
        taskState,   //yes/not_sure buttons enable
        resultState  //i'm right/i was wrong buttons enable
    };
signals:
    void testLoaded();
public slots:
    void setButtomState(const bool isTask, const bool isTestLoaded);
private slots:
    void on_actionOpen_test_triggered();
private:
    void setCurrentTask();
    void setNextTask();
    void setAnswer();

    void answerClicked();
    void notSureClicked();
    void yesClicked();
    void noClicked();

    Ui::MainWindow *ui;
    TestFunnel test;
};
#endif // MAINWINDOW_H
