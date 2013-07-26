#pragma once
#ifndef IPROGRESS_H
#define IPROGRESS_H
#include <QString>
//Interface for operating with progress dialog
//allow :
//* ask questions
//* check cansel by user/system
//* set max and add position
//* set dialog text like "working with smthg"
class IProgress {
public:
    //Метод возвращает текущее состояние процесса
    //1 - процесс отменен пользователем, 0 - процесс продолжается
    virtual int isStoped() = 0;

    //Метод информирует приложение о том, что с момента последней отправки
    //относительно максимальной позиции maxPosition был произведен шаг step
    //должен возвращать @isStoped()
    virtual int Step(const int step = 1) = 0;

    //Метод передает максимальное состояние прогресса обработки
    //относительно которого будет рассчитывается время выполнения операции
    //так же обнуляет текущее состояние прогресса.
    //При maxPosition==0 будет выводиться циклический индикатор
    virtual void SetMaxProgressPosition (const int maxPosition) = 0;

    //Выводит информацию о текущем действии
    virtual void SetProgressText(const QString &Action) = 0;

    //Метод вызова диалогового окна с запросом OK/CANSEl
    //возвращает 0 на CANSEL и 1 на OK
    virtual int ShowMessageOkCansel(const QString &Caption,const QString &TextMessage) = 0;

    virtual ~IProgress() {}
    };
#endif // PROGRESSI_H
