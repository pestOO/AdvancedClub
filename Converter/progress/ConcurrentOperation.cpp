#include "ConcurrentOperation.h"
#include <QStyle>
#include <QApplication>
#include <QTextEdit>
#include <QSharedPointer>
#include <QDesktopWidget>
#include <QErrorMessage>
#include <QtGui>

ConcurrentOperation::ConcurrentOperation(QWidget *parent) :
    QObject(parent), PrDialog(new QProgressDialog(parent)), dialogUpdater (new QTimer(this)),
    progress (new RConcurrentProgress(this)),
    concurrentTime(QTime::currentTime()), position(0), send_fin(false),
    PrDialogisNotDestroyed(true), errorSurvEditingBox(nullptr)
    {
    Q_CHECK_PTR(PrDialog); Q_CHECK_PTR(dialogUpdater); Q_CHECK_PTR(progress);
    PrDialog->setWindowTitle(tr("Wait..."));
    PrDialog->setModal(true);
    PrDialog->setWindowModality(Qt::WindowModal);
    PrDialog->setAutoClose(false);
    PrDialog->setAutoReset(false);
    PrDialog->setSizeGripEnabled(false);
    setDialogLabelText(tr("Operation in progress."));
    PrDialog->show();
    //On cansel Dialog call interapting in RConcurrentProgress
    connect(PrDialog,   SIGNAL(canceled()),         progress,   SLOT(Interapte()),  Qt::QueuedConnection);
    //connect indication of Dialog to this realisation
    //on finish in #watcher handle finish (and Interapte progress)
    connect(&watcher,   SIGNAL(finished()),         this,       SLOT(handleFinished()));

    connect(dialogUpdater, SIGNAL(timeout()), this, SLOT(SendValue()));
    dialogUpdater->start(timerStepMSecs);

    //connect $RConcurrentProgress signals
    connect(progress, SIGNAL(SendStep(int)),		this, SLOT(AddStep(int)),   Qt::QueuedConnection);  /*Qt::QueuedConnection*/
    connect(progress, SIGNAL(SendMax (int)),		this, SLOT(SetMax(int)),    Qt::BlockingQueuedConnection);
    connect(progress, SIGNAL(setLabelText(QString)),this, SLOT(setDialogLabelText(QString)), Qt::BlockingQueuedConnection);
    connect(progress, SIGNAL(ShowMessage(QString,QString)),
            this,     SLOT(ShowMessageOkCansel(QString,QString)), Qt::QueuedConnection);
    //save PrDialogisDestroyed status for correct destoying
    connect(PrDialog, SIGNAL(destroyed()), this, SLOT(SetPrDialogisDestroyed()));
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(SetPrDialogisDestroyed()));
    }
void ConcurrentOperation::SetPrDialogisDestroyed()
    {
    PrDialogisNotDestroyed = false;
    }
ConcurrentOperation::~ConcurrentOperation()
    {
    PrDialog->close();
    if(PrDialogisNotDestroyed || !(PrDialog->testAttribute(Qt::WA_DeleteOnClose)) )
        PrDialog->deleteLater();
    }
void ConcurrentOperation::ReturnResult() {
    try {//wait result (actual for canseled thread with exception)
        watcher.waitForFinished();
        }
    catch (const RException &err) {
        exceptionHolder = QSharedPointer<RException>(err.clone());
    } catch (...) {
        exceptionHolder = QSharedPointer<RException>(new RException(err_strange, fullPlace));
        R_UNIMPLEMENTED();
    }
    //check is finished emited (actialy for canseling)
    if(send_fin++)
        qCritical() << QString("Logic err - 2 times emited finish");
    //NOTE: setMax or not?
    //PrDialog->setMaximum(PrDialog->maximum());
    emit(finished());
    }
void ConcurrentOperation::handleFinished()
    {
    dialogUpdater->stop();
    qDebug() << "RConcurrent operation finished at "
             << QTime(0,0).addMSecs(concurrentTime.elapsed()).toString("mm:ss.zzz");
    //if operation is not finished (it's canseled)
    if(!watcher.isFinished())
        //try cansel watcher
        watcher.cancel();
    //call waiting result and
    ReturnResult();
    }
void ConcurrentOperation::SetMax(int max)
    {
    //for more 2+ operations save time of current
    if (position.load())
        qDebug() << "prev operation finished at "
                 << QTime(0,0).addMSecs(concurrentTime.elapsed()).toString("mm:ss.zzz");
    //reset position of progress
    position = 0;
    //prepare GUI
    PrDialog->show();
    PrDialog->setRange(0, max);
    }
void ConcurrentOperation::setDialogLabelText(const QString &text)
    {
    PrDialog->setLabelText(text);
    prepareDialog(PrDialog);
    }
void ConcurrentOperation::AddStep(int step)
    {
    position.fetchAndAddRelease(step);
    }
void ConcurrentOperation::SendValue()
    {
#if defined(RakursMain) || defined(RakursUnitTesting)
    if(NULL!=sender())
        PrDialog->setValue(position.load());
    else
        qWarning() << "I'p! =)";
#endif
    }
void ConcurrentOperation::reset()
    {
    position = send_fin = 0;
    concurrentTime = QTime::currentTime();
    progress->reset();
    PrDialog->reset();
    dialogUpdater->start(timerStepMSecs);
    exceptionHolder.clear();
    }
bool ConcurrentOperation::HaveException()
    {
    return exceptionHolder.data();
    }
const RException *ConcurrentOperation::GetLastException()
    {
    //check user logics - NULL referense is bad +)
#ifdef RakursUnitTesting
    QTest::qVerify(!exceptionHolder.data(), "(RException*)exception", "not NULL", __FILE__, __LINE__);
#else
    Q_ASSERT(exceptionHolder.data());
#endif
    return exceptionHolder.data();
    }
int ConcurrentOperation::ShowLastErrorMessage(const QString &WindowTitle)
    {
    PrDialog->close();
    const RException*exception = GetLastException();
    const RExceptionTextFileLoad* fileFormatException = dynamic_cast<const RExceptionTextFileLoad*>(exception);
    if(fileFormatException)
        return showErrBox(*fileFormatException, WindowTitle);
    return showErrBox(*exception, WindowTitle);
    }
void ConcurrentOperation::close()
    {
    dialogUpdater->stop();
    PrDialog->close();
    }
void ConcurrentOperation::ShowMessageOkCansel(const QString &Caption, const QString &TextMessage)
    {
    //prepare #msgBox texts and buttons
    askMessageBox.setText(Caption);
    askMessageBox.setInformativeText(TextMessage);
    askMessageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    //run #MsgBox
    const int result = askMessageBox.exec();
    //set user answer to #progress
    progress->SetShowMessageResult(result);
    }
void ConcurrentOperation::waitForFinished()
    {
    watcher.waitForFinished();
    }
int ConcurrentOperation::showErrBox(const RException &err, const QString& errTitle)
    {
    RErrorMessageBox errBox(errTitle, &err, PrDialog->parentWidget());
    errBox.exec();
    return QDialog::Rejected;
    }
int ConcurrentOperation::showErrBox(const RExceptionTextFileLoad &err, const QString& errTitle)
    {
    if(nullptr == errorSurvEditingBox)
        errorSurvEditingBox = new RErrorSurvEditingBox(PrDialog->parentWidget());
    errorSurvEditingBox->setException(err);
    errorSurvEditingBox->setWindowTitle(errTitle);
    errorSurvEditingBox->showMaximized();
    switch (errorSurvEditingBox->exec()) {
        case QDialog::Accepted:
            return QDialog::Accepted;
            break;
        default: //QDialog::Rejected
            DELETE_PTR(errorSurvEditingBox);
            return QDialog::Rejected;
            break;
        };
    }
