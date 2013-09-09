/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**		Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 05.07.2013 by Zamakhov_EY.
****************************************************************************/
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QtWidgets>
#include <QFileDialog>
#include <QProcess>
#include <QSound>
#include <QListWidget>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
    {
    ui->setupUi(this);
    setStatusBar(nullptr);
    const int newWidth = qApp->desktop()->screenGeometry(this).width () / 3;
    resize (newWidth, height());

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
    connect (this, &MainWindow::signal_log,
             this, &MainWindow::add_Text_to_Log, Qt::QueuedConnection);

    ui->listWidgetLog->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidgetLog, &QWidget::customContextMenuRequested,
            [=](const QPoint& pos) {
                    const QPoint globalPos = ui->listWidgetLog->mapToGlobal(pos);
                    QMenu myMenu;
                    myMenu.addAction(tr("Clear log"));
                    if (myMenu.exec(globalPos))
                        ui->listWidgetLog->clear ();
                });
    }
MainWindow::~MainWindow()
    {
    delete ui;
    }
void MainWindow::add_Text_to_Log(const QString & text, const bool err)
    {
    QListWidgetItem *item = new QListWidgetItem(text);
    ui->listWidgetLog->addItem(item);
    ui->listWidgetLog->setCurrentItem(item);
    if(err)
        {
        item->setForeground (Qt::red);
        QFile logFile("converter_errors.txt");
        logFile.open (QFile::Text | QFile::Append | QFile::WriteOnly);
        QTextStream logFileStream(&logFile);
        logFileStream << text << endl;
        QToolTip::showText (QCursor::pos(), text, this);
        }
    }
void MainWindow::setEnableButtons(const bool enable)
    {
    ui->actionInpute_audio_folder->setEnabled (enable);
    ui->actionOutpute_audio_folder->setEnabled (enable);
    ui->actionAudio_list_file->setEnabled (enable);
    ui->actionConvert->setEnabled (enable);
    ui->actionGenerateAudioList->setEnabled (enable);
    ui->actionSplitFileList->setEnabled (enable);
    ui->boxSplitor->setEnabled (enable);
    ui->pushChangeTime->setEnabled (enable);
    }
void MainWindow::on_actionGenerateAudioList_triggered()
    {
    on_actionInpute_audio_folder_triggered();
    if(!inputeAudioDirWay.isEmpty ())
        {
        const QDir audioDir(inputeAudioDirWay);
        const QStringList filter{ "*.mp3", "*.wav"};
        const QStringList list = audioDir.entryList(filter, QDir::NoDot | QDir::Files, QDir::Name);
        if(list.isEmpty ())
            {
            addErrorToLog (tr("Empty folder \"%1\"").arg (audioDir.absolutePath ()));
            return;
            }

        const QString listFileWay = audioDir.absoluteFilePath ("list.txt");
        QFile txtFile(listFileWay);
        if(!txtFile.open (QFile::WriteOnly | QFile::Text))
            {
            addErrorToLog (tr("Couldn't open open file \"%1\"").arg (listFileWay));
            return;
            }
        QTextStream txtFileStream(&txtFile);

        addSilenseFromUI (txtFileStream);
        for(const QString& str : list)
            txtFileStream << str << endl;
        const QString outFile = QFileInfo(txtFile.fileName()).filePath();
        addInfoToLog(tr("Generated file \"%1\"").arg (outFile));
        setNewAudioListFile( QStringList(outFile) );
        }
    }
void MainWindow::on_actionSplitFileList_triggered()
    {
    const QString dir = textFiles.isEmpty () ? "" : textFiles.first ();
    const QString bigTxt = QFileDialog::getOpenFileName(this, tr("Open text list"), dir,
                                         tr("Text Files (%1)").arg (QLatin1Literal("*.txt")));
    const QFileInfo bigTxtFileInfo(bigTxt);
    if(!bigTxt.isEmpty() && bigTxtFileInfo.exists())
        {
        QFile bigTxtFile(bigTxt);
        if(!bigTxtFile.open (QFile::Text | QFile::ReadOnly))
            {
            addErrorToLog (tr("Couldn't open open file \"%1\"").arg(bigTxtFile.fileName()));
            return;
            }
        QTextStream bigTxtStream(&bigTxtFile);
        //skip old timings
        loadSilense(bigTxtStream);
        const int linesInFile = ui->boxSplitor->value();
        Q_ASSERT(linesInFile>0);

        int countLines = 0;
        const QString splitedTxtFileName =
                bigTxtFileInfo.absolutePath() + QLatin1Char('/') +
                bigTxtFileInfo.completeBaseName () + QLatin1Literal("%1.txt");
        int fileCounter = 0;
        QFile splitedTxtFile;
        QTextStream splitedTxtStream(&splitedTxtFile);
        textFiles.clear();
        while(!bigTxtStream.atEnd ())
            {
            if( --countLines <= 0)
                {
                const QString fileName = splitedTxtFileName.
                                         arg(++fileCounter, 3, 10, QLatin1Char('0'));
                textFiles << fileName;
                splitedTxtFile.close ();
                splitedTxtFile.setFileName(fileName);
                if(!splitedTxtFile.open (QFile::Text | QFile::WriteOnly))
                    {
                    addErrorToLog (tr("Couldn't open open file \"%1\"").arg(splitedTxtFile.fileName()));
                    return;
                    }
                countLines = linesInFile;
                addSilenseFromUI (splitedTxtStream);
                }
            else
                splitedTxtStream << endl;
            splitedTxtStream << bigTxtStream.readLine ();
            }
        splitedTxtFile.close ();
        addInfoToLog(tr("Splited file \"%1\"").arg (bigTxt));
        }
    }
void MainWindow::sox(const QStringList & args) const
    {
    //TODO: try "−−multi−threaded"
    QFile logFile(sox_log_name);
    logFile.open (QFile::Text | QFile::Append | QFile::WriteOnly);
    QTextStream logFileStream(&logFile);

    const QString soxApp("sox/sox.exe");
    if(!QFile::exists (soxApp))
        {
        const QString err = "no sox app";
        logFileStream << err;
        qWarning() << err;
        return;
        }
    if(args.isEmpty ())
        {
        const QString err = "empty args";
        logFileStream << err;
        qWarning() << err;
        return;
        }
    QProcess soxProcess;
    if(args.size () > 1)
        soxProcess.start(soxApp, args);
    else
        {
        soxProcess.setProgram (soxApp);
        soxProcess.setNativeArguments (args.first ());
        soxProcess.start ();
        }
    while(!soxProcess.waitForFinished (100))
        qApp->processEvents ();
    {
    const QString soxOut = soxProcess.readAllStandardOutput();
    if(!soxOut.isEmpty ())
        logFileStream << soxOut;
    }
    {
    const QString soxOutErr = soxProcess.readAllStandardError();
    if(!soxOutErr.isEmpty ())
        logFileStream << soxOutErr;
    }
    }
QString MainWindow::createSilentFile(const qreal duration) const
    {
    const int decSecs = duration*10;
    const QString durationText = QString("%1").arg(decSecs, 3, 10, QLatin1Char('0'));
    QDir dir;
    auto constexpr folder = "pause_audio";
    dir.mkdir (folder);
    dir.cd (folder);
    const QString silenceFile = dir.filePath (QString("silence%1.mp3").arg(durationText));
    const QString argument = QString("-b 16 -c 2 -n -r 48000 %1 trim 0.0 %2").
                             arg(silenceFile).arg(duration, 0, 'f', 1);
    static QMutex mutex;
    QMutexLocker locker(&mutex); Q_UNUSED(locker);
    if(!QFile::exists(silenceFile))
        {
        sox(QStringList(argument));
        if(!QFile::exists(silenceFile))
            addErrorToLog (tr("Silence file don't generated"));
        }
    return silenceFile;
    }
void MainWindow::on_actionAudio_list_file_triggered()
    {
    const QStringList newTxtList = QFileDialog::getOpenFileNames(this, tr("Open text list"), "",
                                         tr("Text Files (%1)").arg (QLatin1Literal("*.txt")));
    if(!newTxtList.isEmpty ())
        {
        setNewAudioListFile(newTxtList);
        addInfoToLog(tr("Selected text lists"));
        }
    }
void MainWindow::on_actionInpute_audio_folder_triggered()
    {
    const QString newDirectory =
            QFileDialog::getExistingDirectory(this, tr("Open Directory"), "",
                                              QFileDialog::ReadOnly
                                              | QFileDialog::DontResolveSymlinks);
    if(!newDirectory.isEmpty ())
        {
        inputeAudioDirWay = newDirectory;
        addInfoToLog(tr("Selected inpute directory \"%1\"").arg (newDirectory));
        }
    }
void MainWindow::on_actionOutpute_audio_folder_triggered()
    {
    const QString newDirectory =
            QFileDialog::getExistingDirectory(this, tr("Open Directory"), "",
                                              QFileDialog::ReadOnly
                                              | QFileDialog::DontResolveSymlinks);
    if(!newDirectory.isEmpty ())
        {
        outputeAudioDirWay = newDirectory;
        addInfoToLog(tr("Selected outpute directory \"%1\"").arg (newDirectory));
        }
    }
void MainWindow::on_actionConvert_triggered()
    {
    if(textFiles.isEmpty())
        {
        addErrorToLog(tr("No text list"));
        return;
        }
    setEnableButtons (false);
    const QDir inputeDir = !inputeAudioDirWay.isEmpty() ? QDir(inputeAudioDirWay) : QFileInfo(textFiles.first()).dir();
    const QDir outputeDir= !outputeAudioDirWay.isEmpty()? QDir(outputeAudioDirWay): inputeDir;

    const auto oldMaxThreads = QThreadPool::globalInstance()->maxThreadCount ();
    QThreadPool::globalInstance()->setMaxThreadCount (qMax(2, QThread::idealThreadCount () + 1));

    QFutureWatcher<void> *watcher  = new QFutureWatcher<void>();
    QObject::connect(watcher, &QFutureWatcher<void>::finished,
                     [=]() {
                        setEnableButtons (true);
                        QThreadPool::globalInstance()->setMaxThreadCount (oldMaxThreads);
                        });
    QFuture<void> future = QtConcurrent::run(
                         [=]()
            {
            QFutureSynchronizer<void> sync;
            for(const auto file : textFiles)
                sync.addFuture ( QtConcurrent::run(this, &MainWindow::prepareConvertAudio,
                                                   inputeDir, file, outputeDir));
            sync.waitForFinished ();
            addInfoToLog(tr("All audio converted"));
            });
    watcher->setFuture(future);
    }
void MainWindow::on_pushChangeTime_clicked()
    {
    dialogTiming.exec ();
    }
void MainWindow::prepareConvertAudio(const QDir& inputeDir, const QString & file, const QDir& outputeDir)
    {
    QFile txtFile(file);
    txtFile.open (QFile::ReadOnly | QFile::Text);
    QTextStream txtFileStream(&txtFile);
    const QSet<Timing> timings = loadSilense(txtFileStream);
    const QFileInfo fileInfo(file);
    if(timings.isEmpty ()){
        addInfoToLog(tr("Couldn't load timings from file \"%1\"").arg (fileInfo.fileName()));
        return;
        }
    QStringList files;
    while(!txtFileStream.atEnd ())
        files << txtFileStream.readLine ();

    const QString baseFileName = fileInfo.completeBaseName();
    for(const Timing& timing : timings)
        convertAudio(inputeDir, files, outputeDir, baseFileName, timing);
    addInfoToLog(tr("All audio for list \"%1\" converted").arg (baseFileName));
    }
void MainWindow::convertAudio(const QDir& inputeDir, const QStringList & files,
                                    const QDir& outputeDir,const QString& outBaseName, const Timing & t)
    {
    static constexpr QLatin1Literal wav{".wav"};
    static constexpr QLatin1Literal mp3{".mp3"};
    QStringList arguments;
    const QString majorSilence = createSilentFile(t.majorSeconds);
    const QString minorSilence = createSilentFile(t.minorSeconds);
    int step = 0;
    for(const QString& fileName : files)
        {
        if(!fileName.isEmpty ())
            {
            const QString audioFile = fileName.trimmed();
            const QString audioFileAbs = inputeDir.absoluteFilePath(audioFile);
            if(QFile::exists (audioFileAbs))
                arguments << audioFileAbs;
            else
                {
                const QString fileWav = audioFileAbs + wav;
                if(QFile::exists (fileWav))
                    arguments << fileWav;
                else
                    {
                    const QString fileMp3 = audioFileAbs + mp3;
                    if(QFile::exists (fileMp3))
                        arguments << fileMp3;
                    else
                        {
                        addErrorToLog(tr("No file \"%1\"").arg (audioFileAbs) + "(wav/mp3).");
                        return;
                        }
                    }
                }
            if(++step % t.minorQuant == 0)
                arguments << minorSilence;
            else
                arguments << majorSilence;
            }
        }
    //remove last silense
    arguments.removeLast ();
    const constexpr auto ch = QLatin1Char('_');
    const QString outName = outBaseName + ch + t.toString (ch) + mp3;
    const QString outNameAbsolute = outputeDir.absoluteFilePath(outName);
    arguments << outNameAbsolute;
    QFile outFile(outNameAbsolute);
    if(outFile.exists ())
        if(!outFile.remove ()){
            addInfoToLog(tr("Couldn't remove old file \"%1\"").arg (outName));
            return;
            }
    sox(arguments);
    if(!QFile::exists(outNameAbsolute))
        addErrorToLog (tr("Outpute file \"%1\" don't generated. Details in log-file \"%2\"")
                  .arg (outName).arg (sox_log_name));
    else
        addInfoToLog(tr("Outpute file \"%1\" converted.").arg (outName));
    }
void MainWindow::setNewAudioListFile(const QStringList & newFiles)
    {
    textFiles = newFiles;
    inputeAudioDirWay = outputeAudioDirWay = QString();
    ui->actionConvert->setEnabled (true);
    }
void MainWindow::addSilenseFromUI(QTextStream & stream) const
    {
    const QSet<Timing> timings = dialogTiming.getTimingList ();
    if(timings.isEmpty ())
        {
        addErrorToLog (tr("Timings are epmty"));
        return;
        }
    for(const Timing t : timings)
        stream << t.toString() << endl;
    }
QSet<Timing> MainWindow::loadSilense(QTextStream & stream)
    {
    QSet<Timing> timings;
    Timing tempTiming;
    do {
        const auto streamPos = stream.pos ();
        const QString line = stream.readLine ();
        if(tempTiming.fromString (line))
            timings <<tempTiming;
        else
            {
            stream.seek(streamPos);
            break;
            }
        }
    while(!stream.atEnd ());
    return timings;
    }
void MainWindow::addErrorToLog(const QString & text) const
    {
    emit signal_log(QTime::currentTime ().toString("HH:mm:ss - %1").arg(text), true);
    }
void MainWindow::addInfoToLog(const QString & text) const
    {
    emit signal_log(QTime::currentTime ().toString("HH:mm:ss - %1").arg(text), false);
    }
