/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**		Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 17.03.2013 by pesto.
****************************************************************************/
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QWidget>
#include <QDialog>
#include <QAbstractButton>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
    {
    ui->setupUi(this);
    setStatusBar(nullptr);
    QAction * actionAbout = ui->menuBar->addAction (qApp->translate("About", "About"));
    connect (actionAbout, &QAction::triggered,
             [=]() {
                QDialog dialog(this);
                dialog.setWindowTitle(actionAbout->text());
                dialog.setLayout (new QHBoxLayout());
                const QString text =
                        qApp->translate("About", "Made by %1 for training club %2.").
                        arg("<a href='mailto:EliseyZamahov@gmail.com'>Elisey Zamahov</a>").
                        arg("Advance");
                dialog.layout()->addWidget (new QLabel(text, &dialog));
                dialog.exec();
                });

    ui->questionText->setPlainText(tr("Open test file."));
    connect (ui->buttonAnswer,  &QPushButton::clicked, this, &MainWindow::answerClicked);
    connect (ui->buttonNotSure, &QPushButton::clicked, this, &MainWindow::notSureClicked);
    connect (ui->buttonYes,     &QPushButton::clicked, this, &MainWindow::yesClicked);
    connect (ui->buttonNo,      &QPushButton::clicked, this, &MainWindow::noClicked);
    setButtomState(false, false);
    }
MainWindow::~MainWindow()
    {
    delete ui;
    }
void MainWindow::on_actionOpen_test_triggered()
    {
    const QString testWay =
            QFileDialog::getOpenFileName(
                this,	tr("Select test"), QString(),
                qApp->translate("FileExtentions", "Test files (%1)").arg("*.txt"), 0,
                QFileDialog::DontResolveSymlinks | QFileDialog::DontConfirmOverwrite |  QFileDialog::ReadOnly);
    if(!testWay.isEmpty())
        try {
            test.OpenTest(testWay);
            setCurrentTask();
            setButtomState(true, true);
            }
        catch(const QString& error)
            {
            QMessageBox msgBox;
            msgBox.setText(error);
            msgBox.exec();
            }
    }
void MainWindow::setCurrentTask()
    {
    const Task task = test.currentTask();
    ui->questionText->setPlainText(task.question);
    }
void MainWindow::setNextTask()
    {
    const Task task = test.nextTask();
    ui->questionText->setPlainText(task.question);
    ui->answerText->clear();
    }
void MainWindow::setAnswer()
    {
    const Task task = test.currentTask();
    ui->questionText->setPlainText(task.question);
    ui->answerText->setPlainText(task.answer + '\n' + task.hint);
    }
void MainWindow::answerClicked()
    {
    setAnswer();
    setButtomState(false, true);
    }
void MainWindow::notSureClicked()
    {
    test.addCurrentAsMistake();
    answerClicked();
    }
void MainWindow::yesClicked()
    {
    if(test.isLastTask())
        {
        if(test.countErrors()==0)
            {
            QMessageBox::information(
                        this, windowTitle(), tr("You have no mistakes!"));
            setButtomState(false, false);
            return;
            };
        if (QMessageBox::Yes == QMessageBox::question(this, windowTitle (),
                              tr("You result is %1 mistakes from %2.\nNext round?").
                              arg(test.countErrors()).arg(test.countTasks())) )
            {
            test.nextRound();
            setCurrentTask();
            }
        else
            {
            setButtomState(false, false);
            return;
            }
        }
    else
        setNextTask();
    setButtomState(true, true);
    }
void MainWindow::noClicked()
    {
    test.addCurrentAsMistake();
    yesClicked();
    }
void MainWindow::setButtomState(const bool isTask, const bool isTestLoaded)
    {
//    const bool fisrtButtons  = state != startState && state == taskState;
//    const bool secondButtons = state != startState && state != taskState;
//    ui->buttonAnswer->setEnabled(fisrtButtons);
//    ui->buttonNotSure->setEnabled(fisrtButtons);
//    ui->buttonYes->setEnabled(secondButtons);
//    ui->buttonNo->setEnabled(secondButtons);
    ui->buttonAnswer->setVisible(isTask);
    ui->buttonNotSure->setVisible(isTask);
    ui->buttonAnswer->setEnabled(isTestLoaded);
    ui->buttonNotSure->setEnabled(isTestLoaded);
    ui->buttonYes->setVisible(!isTask);
    ui->buttonNo->setVisible(!isTask);
    ui->buttonYes->setEnabled(isTestLoaded);
    ui->buttonNo->setEnabled(isTestLoaded);
    }
