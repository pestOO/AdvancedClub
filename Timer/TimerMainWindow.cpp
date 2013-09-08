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
    updatetLabelTime();
    enableButtons(false);
    adjustSize();
    updateTimer->setInterval(100);
    updateTimer->setSingleShot(false);
    taskTimer->stop ();
    setAudioFile(soundWay);
    if(!QFile::exists (soundWay))
        QMessageBox::warning (this, tr("No sound"),
                              tr("Coudn't find file %1%2Choose another one!").
                              arg(soundWay).arg(QChar(QChar::LineFeed)));
    connect (updateTimer,   &QTimer::timeout, this, &TimerMainWindow::updatetLabelTime);
    connect (taskTimer,     &QTimer::timeout, this, &TimerMainWindow::tick);
    connect (&player,   SIGNAL(error(QMediaPlayer::Error)), this, SLOT(showPlayerError(QMediaPlayer::Error)));
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
void TimerMainWindow::showPlayerError(QMediaPlayer::Error err)
    {
    qWarning() << err << player.errorString();
    }
void TimerMainWindow::setLabelTime(const int mSecs)
    {
    const QTime time = QTime(0,0).addMSecs(mSecs );
    ui->labelTime->setText(time.toString(QLatin1Literal("m:ss.zzz")));
    }
void TimerMainWindow::updatetLabelTime()
    {
    //for active timer
    if(taskTimer->isActive())
        //update label with secs
        setLabelTime(taskTimer->remainingTime());
    if(!updateTimer->isActive())
        {
        ui->progressBar->setMaximum(1);
        ui->progressBar->setValue(0);
        }
    else
        ui->progressBar->setValue(taskTimer->remainingTime());
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
                play();
            }
        else {
            on_actionStop_triggered();
            ui->labelWord->setText(tr("That's all!"));
            return;
            }
        }
    else
        play();
    taskTimer->start(getmSecsByBoxs());
    updatetLabelTime();
    }
void TimerMainWindow::on_actionStart_triggered()
    {
    qWarning() << "start";
    textFile.open(QFile::ReadOnly);
    tick();
    ui->progressBar->setMaximum(taskTimer->interval());
    updateTimer->start();
    enableButtons(true);
    }
void TimerMainWindow::on_actionStop_triggered()
    {
    //on pause - start
    if(!taskTimer->isActive ())
        on_buttonPause_clicked ();
    taskTimer->stop();
    updateTimer->stop();
    textFile.close();
    player.stop ();
    enableButtons(false);
    updatetLabelTime ();
    setLabelTime(getmSecsByBoxs());
    qWarning() << "stop";
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
        setAudioFile(newSoundWay);
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
    setAudioFile (soundWay);
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
qreal TimerMainWindow::getmSecsByBoxs() const
    {
    return getSecsByBoxs()*1000;
    }
qreal TimerMainWindow::getSecsByBoxs() const
    {
    const int letters = ui->labelWord->text().length();
    return (ui->boxConstSeconds->value() +          /* constant secs */
            ui->boxLetterSeconds->value()*letters);  /* letter time */
    }

void TimerMainWindow::setAudioFile(const QString & file)
    {
    player.setMedia(QUrl::fromLocalFile(file));
    }
void TimerMainWindow::play()
    {
    player.play ();
    }
void TimerMainWindow::on_buttonPause_clicked()
    {
    if(taskTimer->isActive ())
        {
        pause_left_msecs = taskTimer->remainingTime();
        taskTimer->stop ();
        ui->buttonPause->setText (tr("Continue"));
        }
    else
        {
        taskTimer->start (pause_left_msecs);
        ui->buttonPause->setText (tr("Pause"));
        }
    }
