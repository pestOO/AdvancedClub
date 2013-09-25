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
#include <QFileDialog>
#include <QMessageBox>
#include <QFontDialog>
#include <QInputDialog>
#include <QHBoxLayout>
#include "SelectAudioFile.h"
#include "DialogWordStatistics.h"

TimerMainWindow::TimerMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::TimerMainWindow)
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

    wordlist.setMaxRounds (settings.value(settingRoundsCount, WordRepeater::maxRoundsDefault).toInt ());

    clearLabelWord ();
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
    ui->buttonNext->setEnabled (isRunning);

    ui->actionStart->setVisible (!isRunning);
    ui->actionStop-> setVisible(isRunning);
    }
bool TimerMainWindow::isTextMode() const
    {
    return !wordlist.isEmpty();
    }
bool TimerMainWindow::isSoundMode() const
    {
    return wordlist.isEmpty();
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
    else{
        ui->progressBar->setMaximum(taskTimer->interval());
        ui->progressBar->setValue(taskTimer->remainingTime());
        }
    }
void TimerMainWindow::tick()
    {
    if(isTextMode ())
        wordlist.setErrorState (false);
    nextAction();
    }
void TimerMainWindow::nextAction()
    {
    if(isSoundMode ())
        play();
    //text mode
    else
        {
        if(wordlist.hasNext ())
            {
            setLabelWord(wordlist.word ());
            if(ui->checkBoxSoundForWord->isChecked())
                play();
            }
        else
            {
            stopTimers(false);
            setLabelWord(tr("That's all!"));
            showStatistics();
            return;
            }
        }
    taskTimer->start(getmSecsByBoxs());
    updatetLabelTime();
    }
void TimerMainWindow::checkPlayPauseButton()
    {
    //on pause - start
    if(!taskTimer->isActive ())
        on_buttonPause_clicked ();
    }
void TimerMainWindow::on_actionStart_triggered()
    {
    if(isTextMode())
        {
        if(wordlist.hasNextRound ())
            wordlist.startNewRound ();
        else
            {
            showStatistics();
            return;
            }
        }
    qWarning() << "start";
    nextAction();
    updateTimer->start();
    enableButtons(true);
    ui->buttonNext->setFocus ();
    }
void TimerMainWindow::on_actionStop_triggered()
    {
    stopTimers(true);
    }
void TimerMainWindow::stopTimers(const bool reset)
    {
    //on pause - start
    checkPlayPauseButton ();
    taskTimer->stop();
    updateTimer->stop();
    player.stop ();
    enableButtons(false);
    updatetLabelTime ();
    setLabelTime(getmSecsByBoxs());
    if(isTextMode())
        {
        //clear current rounf data on stop
        if(reset)
            wordlist.resetRound ();
        else
            //add time for next round
            if(wordlist.hasNextRound ())\
                {
                ui->boxConstSeconds->setValue (ui->boxConstSeconds->value () * 1.5);
                ui->boxLetterSeconds->setValue(ui->boxLetterSeconds->value() * 1.5);
                }
        }
    qWarning() << "stop";
    }
void TimerMainWindow::on_actionExit_triggered()
    {
    close();
    }
void TimerMainWindow::on_actionSelect_audio_triggered()
    {
    const QString newSoundWay = SelectAudioFile::getAudioFile (this);
    if(!newSoundWay.isEmpty())
        setAudioFile(newSoundWay);
    }

void TimerMainWindow::on_actionChoode_text_file_triggered()
    {
    const auto getOldWay = [&]()
        {
        const QString textWay =  settings.value (settingTextWay).toString ();
        if(!QFile::exists(textWay))
            {
            const QString txtFolder = "TEXTFILES";
            const QDir dir;
            if(dir.exists (txtFolder))
                return dir.absoluteFilePath (txtFolder);
            }
        return textWay;
        };
    const QString oldTextWay = getOldWay();
    const QString newTextWay =
            QFileDialog::getOpenFileName(this, tr("Open text file"), oldTextWay,
                                         tr("Text Files (%1)").arg (QLatin1Literal("*.txt")));
    if(!newTextWay.isEmpty())
        {
        wordlist.load (newTextWay);
        if(!wordlist.isEmpty ())
            {
            settings.setValue (settingTextWay, newTextWay);
            ui->checkBoxSoundForWord->setEnabled(true);
            ui->boxLetterSeconds->setEnabled(true);
            }
        }
    }
void TimerMainWindow::on_actionClear_sound_triggered()
    {
    setAudioFile (soundWay);
    }
void TimerMainWindow::on_actionClear_text_file_triggered()
    {
    wordlist.clear ();
    clearLabelWord ();
    ui->checkBoxSoundForWord->setEnabled(false);
    ui->boxLetterSeconds->setEnabled(false);
    }
void TimerMainWindow::on_actionChange_text_font_triggered()
    {
    QFontDialog dialog(ui->labelWord->font(), this);
    dialog.setWindowTitle (tr("Select font"));
    if(dialog.exec() == QDialog::Accepted)
        ui->labelWord->setFont(dialog.selectedFont());
    }
void TimerMainWindow::on_actionChange_count_rounds_triggered()
    {
    bool ok;
    const int rounds = QInputDialog::getInt(this, tr("Select count rounds"),
                                         tr("Count rounds:"), 3, 2, 100, 1, &ok);
    if (ok)
        {
        settings.setValue (settingRoundsCount, rounds);
        wordlist.setMaxRounds (rounds);
        }
    }
qreal TimerMainWindow::getmSecsByBoxs() const
    {
    return getSecsByBoxs()*1000;
    }
qreal TimerMainWindow::getSecsByBoxs() const
    {
    const int letters = ui->labelWord->text().length();
    return (ui->boxConstSeconds->value() +          /* constant secs */
            ui->boxLetterSeconds->value()*letters); /* letter time */
    }
void TimerMainWindow::setAudioFile(const QString & file)
    {
    player.setMedia(QUrl::fromLocalFile(file));
    }

void TimerMainWindow::setLabelWord(const QString & word)
    {
    static QRegExp regExp("[\\r\\t]");
    const QString linedText = word.split (regExp, QString::SkipEmptyParts).join(QChar::LineFeed);
    ui->labelWord->setText(linedText);
    }
void TimerMainWindow::clearLabelWord()
    {
    ui->labelWord->setText(QLatin1Literal(""));
    }
void TimerMainWindow::showStatistics()
    {
    DialogWordStatistics dialog(wordlist, this);
    dialog.exec ();
    }
void TimerMainWindow::play()
    {
    player.stop ();
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
void TimerMainWindow::on_buttonNext_clicked()
    {
    checkPlayPauseButton ();
    if(isTextMode ())
        wordlist.setErrorState (true);
    player.stop ();
    taskTimer->stop();
    updatetLabelTime ();
    nextAction();
    }
