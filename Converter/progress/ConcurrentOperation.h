#pragma once
#ifndef ConcurrentOperation_H
#define ConcurrentOperation_H
#include <QProgressDialog>
#include <QtConcurrent/QtConcurrentRun>
#include <QtCore>
#include <QAtomicInt>
#include <QSharedPointer>
#include <QMessageBox>

    //Class running metod in thread with GUI indication and cansel
    //provides controlling end of
    class ConcurrentOperation : public QObject {
        Q_OBJECT
        explicit ConcurrentOperation(QWidget *parent);
        ConcurrentOperation& operator=(const ConcurrentOperation&) = delete;
    public:
        //classic Qt connection
        template <class FinishResultObjectHandler>
        static QSharedPointer<ConcurrentOperation> CreateConcOperation(QWidget *parent, FinishResultObjectHandler* finishHandler, const char *finishSLot)
            {
            QSharedPointer<ConcurrentOperation> operation(new ConcurrentOperation(parent)); Q_CHECK_PTR(operation.data());
            QObject::connect(operation.data(), SIGNAL(finished()), finishHandler, finishSLot, Qt::QueuedConnection);
            return operation;
            }
        //c++0x Qt connection
        template <class FinishResultObjectHandler, class Method>
        static QSharedPointer<ConcurrentOperation> CreateConcOperation(QWidget *parent, FinishResultObjectHandler* finishHandler, const Method& finishSLot)
            {
            QSharedPointer<ConcurrentOperation> operation(new ConcurrentOperation(parent)); Q_CHECK_PTR(operation.data());
            QObject::connect(operation.data(), &ConcurrentOperation::finished, finishHandler, finishSLot, Qt::QueuedConnection);
            return operation;
            }
        ~ConcurrentOperation();
        //metode with running some @Metod(,) in thread + connect it with watcher
        template <typename RunObject, typename ... Params, typename ... Args>
        void runMethodeInThread(RunObject *object, void (RunObject::*runMethod)(IProgress* const, Params...), const Args&...args) {
            reset();
            watcher.setFuture( QtConcurrent::run(object, runMethod, progress, args...) );
            }
        //metode with running some @Metod(,) in thread + connect it with watcher
        template <typename Method, typename ... Params, typename ... Args>
        void runMethodeInThread(const Method& runMethod, const Args&...args)
            {
            reset();
            watcher.setFuture( QtConcurrent::run(runMethod, progress, args...) );
            }
        void waitForFinished();
        //if concurrent operation raise exception - return true
        bool HaveException();
        int showErrBox(const RException& err, const QString& errTitle);
        int showErrBox(const RExceptionTextFileLoad& err, const QString& errTitle);
    signals:
        //emit on close/cansel or finishing progress
        void finished();
        //emit new ProgressDialog position
        void SetPosition(int pos);
    public slots:
        //Show MessageBox
        void ShowMessageOkCansel(const QString &Caption, const QString &TextMessage);
        //catch resultReadyAt of watcher
        void ReturnResult();
        //catch finish and cansel of operation
        void handleFinished();
        //cal and send new possition of progress
        void AddStep(int step);
        //set max posssition of indication
        void SetMax (int max);
        //update dialog text and refresh it size
        void setDialogLabelText(const QString &s);
        //show error dialog with exception info
        int ShowLastErrorMessage(const QString &WindowTitle);
        void close();
    private slots:
        //set PrDialogisDestroyed
        void SetPrDialogisDestroyed();
        void SendValue();
    protected:
        void reset();
    private:
        //gui indication with user cansel button
        QProgressDialog* PrDialog;
        QTimer* dialogUpdater;
        static const int timerStepMSecs = 100;
        //IProgress implementation link
        RConcurrentProgress* progress;
        //watcher to #future
        QFutureWatcher<void> watcher;
        //timer for controllong time of operation
        QTime concurrentTime;
        //current position and devide for less call ProgressDialog
        QAtomicInt position;
        //for sending result only on cansel or result
        int    send_fin,
        //handle PrDialogis status for closeing all app
                PrDialogisNotDestroyed;
        QSharedPointer<RException> exceptionHolder;

        //widgets
        QMessageBox askMessageBox;
        RErrorSurvEditingBox* errorSurvEditingBox{nullptr};
    };
}
#endif // ConcurrentOperation_H
