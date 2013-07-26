/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**		Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 27.03.2013 by Elisey Zamakhov.
****************************************************************************/
#include "TimerMainWindow.h"
#include "ui_TimerMainWindow.h"
#include <QSound>
#include <QMessageBox>
#include <QFileDialog>
#include <QFontDialog>
#include <QHBoxLayout>

TimerMainWindow::TimerMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::TimerMainWindow),
    fileTextStream(&textFile)
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

    const qreal constSecs = ui->boxConstSeconds->value();
    ui->boxConstSeconds->setValue(settings.value(settingConstSecs, constSecs).toReal());

    const qreal letterSecs = ui->boxLetterSeconds->value();
    ui->boxLetterSeconds->setValue(settings.value(settingLetterSecs, letterSecs).toReal());

    const QFont wordFont = ui->labelWord->font();
    ui->labelWord->setFont(settings.value(settingWordFont, wordFont).value<QFont>());

    ui->labelWord->setText(QLatin1Literal(""));
    setLabelTime();
    enableButtons(false);
    adjustSize();
    updateTimer->setInterval(100);
    updateTimer->setSingleShot(false);
    mainTimer->stop ();
    if(!QFile::exists (soundWay))
        QMessageBox::warning (this, tr("No sound"),
                              tr("Coudn't find file %1%2Choose another one!").
                              arg(soundWay).arg(QChar(QChar::LineFeed)));
    connect (updateTimer,   &QTimer::timeout, this, &TimerMainWindow::setLabelTime);
    connect (mainTimer,     &QTimer::timeout, this, &TimerMainWindow::tick);
    }
TimerMainWindow::~TimerMainWindow()
    {
    settings.setValue(settingConstSecs, ui->boxConstSeconds->value());
    settings.setValue(settingLetterSecs,ui->boxLetterSeconds->value());
    settings.setValue(settingWordFont,  ui->labelWord->font());
    delete ui;
    }
void TimerMainWindow::enableButtons(const bool isRunning)
    {
    ui->actionChoode_text_file->setEnabled (!isRunning);
    ui->actionSelect_audio->setEnabled (!isRunning);
    ui->actionClear_sound->setEnabled(!isRunning);
    ui->actionClear_text_file->setEnabled(!isRunning);

    ui->buttonPause->setEnabled (isRunning);

    ui->actionStart->setVisible (!isRunning);
    ui->actionStop-> setVisible(isRunning);
    }
void TimerMainWindow::setLabelTime()
    {
    const QTime time =
            QTime(0,0).addMSecs(
                updateTimer->isActive() ? mainTimer->remainingTime() : getSecsByBoxs()*1000);
    ui->labelTime->setText(time.toString(QLatin1Literal("m:ss.zzz")));
    if(updateTimer->isActive())
        {
        ui->progressBar->setMaximum(mainTimer->interval());
        ui->progressBar->setValue(mainTimer->remainingTime());
        }
    else
        {
        ui->progressBar->setMaximum(1);
        ui->progressBar->setValue(0);
        }
    }
void TimerMainWindow::tick()
    {
    if(textFile.exists())
        {
        QString text;
        while(!fileTextStream.atEnd ())
            {
            text = fileTextStream.readLine().trimmed();
            if(!text.isEmpty ())
                break;
            }
        static QRegExp regExp("[\\r\\t]");
        const QString linedText = text.split (regExp, QString::SkipEmptyParts).join(QChar::LineFeed);
        ui->labelWord->setText(linedText);

        if(!fileTextStream.atEnd ())
            {
            if(ui->checkBoxSoundForWord->isChecked())
                QSound::play(soundWay);
            }
        else {
            on_actionStop_triggered();
            ui->labelWord->setText(tr("That's all!"));
            return;
            }
        }
    else
        QSound::play(soundWay);
    mainTimer->start(getSecsByBoxs()*1000);
    setLabelTime();
    }
void TimerMainWindow::on_actionStart_triggered()
    {
    qWarning() << "start";
    textFile.open(QFile::ReadOnly);
    tick();
    updateTimer->start();
    enableButtons(true);
    }
void TimerMainWindow::on_actionStop_triggered()
    {
    qWarning() << "stop";
    mainTimer->stop();
    updateTimer->stop();
    textFile.close();
    enableButtons(false);
    setLabelTime ();
    }
void TimerMainWindow::on_actionExit_triggered()
    {
    close();
    }
void TimerMainWindow::on_actionSelect_audio_triggered()
    {
    const QString newSoundWay =
            QFileDialog::getOpenFileName(this, tr("Open audio file"),
                                         settings.value (settingSoundWay).toString (),
                                         tr("Sound Files (%1)").arg (QLatin1Literal("*.wav *.mp3")));
    if(!newSoundWay.isEmpty())
        {
        settings.setValue (settingSoundWay, newSoundWay);
        soundWay = newSoundWay;
        }
    }

void TimerMainWindow::on_actionChoode_text_file_triggered()
    {
    const QString newTextWay =
            QFileDialog::getOpenFileName(this, tr("Open text file"),
                                         settings.value (settingTextWay).toString (),
                                         tr("Text Files (%1)").arg (QLatin1Literal("*.txt")));
    if(!newTextWay.isEmpty())
        {
        settings.setValue (settingTextWay, newTextWay);
        textFile.setFileName(newTextWay);
        ui->checkBoxSoundForWord->setEnabled(true);
        ui->boxLetterSeconds->setEnabled(true);
        }
    }
void TimerMainWindow::on_actionClear_sound_triggered()
    {
    soundWay = QLatin1Literal("left.wav");
    }
void TimerMainWindow::on_actionClear_text_file_triggered()
    {
    textFile.close();
    textFile.setFileName(QLatin1Literal(""));
    ui->checkBoxSoundForWord->setEnabled(false);
    ui->boxLetterSeconds->setEnabled(false);
    }
void TimerMainWindow::on_actionChange_text_font_triggered()
    {
    QFontDialog dialog(ui->labelWord->font(), this);
    if(dialog.exec() == QDialog::Accepted)
        ui->labelWord->setFont(dialog.selectedFont());
    }
qreal TimerMainWindow::getSecsByBoxs() const
    {
    const int letters = ui->labelWord->text().length();
    return (ui->boxConstSeconds->value() +          /* constant secs */
            ui->boxLetterSeconds->value()*letters);  /* letter time */
    }
void TimerMainWindow::on_buttonPause_clicked()
    {
    if(mainTimer->isActive ())
        {
        updateTimer->stop ();
        mainTimer->stop ();
        ui->buttonPause->setText (tr("Continue"));
        }
    else
        {
        updateTimer->start ();
        mainTimer->start ();
        ui->buttonPause->setText (tr("Pause"));
        }
    }
