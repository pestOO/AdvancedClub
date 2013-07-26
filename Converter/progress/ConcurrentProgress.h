#pragma once
#ifndef RConcurrentProgress_H
#define RConcurrentProgress_H
#include "IProgress.h"
#include <QtCore>
namespace RNS {

    //implementation of Iprogress interface
    class RConcurrentProgress : public QObject,  public IProgress
    {
        Q_OBJECT
    public:
        explicit RConcurrentProgress(QObject *parent);
        //Show Dialog with OK/CANSEL buttons
        //return 1 on OK or 0 on CANSEL
        int ShowMessageOkCansel(const QString &Caption, const QString &TextMessage) override;
        //Return current states of interapted
        int isStoped() override;
        //Add #count steps to graphic progress
        //return @isStoped() state
        int Step(int count) override;
        //Set max Progress Position and set current step to zero
        void SetMaxProgressPosition (int maxPosition) override;
        //Set ProgressText label by QString value
        void SetProgressText(const QString &Action) override;
    public slots:
        //signal for catching interapting
        void Interapte();
        void reset();
        //get result from @ShowMessage
        void SetShowMessageResult(int result);
    signals:
        //emit to ask smtg
        void ShowMessage(QString Caption, QString TextMessage) const;
        //Emit new steps
        void SendStep(int step) const;
        //emit new max possition
        void SendMax (int maxPosition) const;
        //emit new label text
        void setLabelText(const QString& text) const;
    private:
        //is operation interapted by getting @Interapte()
        bool is_interapted={false};
        //locker for waiting result from MessageBox
        QMutex  mutexMessageBox;
        //result of MessageBox
        int ShowMessageResult;
    };
}       // RNS
#endif  // RConcurrentProgress_H
