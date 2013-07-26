#include "ConcurrentProgress.h"
#include <QWaitCondition>
#include <QMessageBox>
using namespace RNS;

RConcurrentProgress::RConcurrentProgress(QObject*parent) : QObject(parent)
    {
    //add interapting on Exit Application
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(Interapte()), Qt::QueuedConnection);
    }
int RConcurrentProgress::isStoped()
    {
    //return interapted by user/system/app close
    return is_interapted || qApp->closingDown();
    }
int RConcurrentProgress::Step(int step)
    {
    emit(SendStep(step));
    return isStoped();
    }
int RConcurrentProgress::ShowMessageOkCansel(const QString &Caption, const QString &Text)
    {
    //lock mutex for waiting user answer
    mutexMessageBox.lock();
    //ask user
    emit(ShowMessage(Caption, Text));
    //wait while mutex'll be unlocked after user answer
    QMutexLocker locker(&mutexMessageBox);
    //unlock it imidiatly for next messages
    //return user answer
    return ((ShowMessageResult==QMessageBox::Ok) ? 1 : 0);
    }
void RConcurrentProgress::SetMaxProgressPosition(int maxPosition)
    {
    emit(SendMax(maxPosition));
    }
void RConcurrentProgress::SetProgressText(const QString &Action)
    {
    emit(setLabelText(Action));
    }
void RConcurrentProgress::Interapte()
    {
    is_interapted = true;
    }
void RConcurrentProgress::reset()
    {
    is_interapted = false;
    }
void RConcurrentProgress::SetShowMessageResult(int result)
    {
    //get user answer
    ShowMessageResult = result;
    //unlock mutex for return answer in @ShowMessageOkCansel()
    mutexMessageBox.unlock();
    }
