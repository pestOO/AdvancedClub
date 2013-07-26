#ifndef DIALOGTIMING_H
#define DIALOGTIMING_H
#include <QDialog>
#include "ModelTiming.h"
namespace Ui {
        class DialogTiming;
    }
class DialogTiming : public QDialog
{
    Q_OBJECT
public:
    explicit DialogTiming(QWidget *parent = 0);
    ~DialogTiming();
    QSet<Timing> getTimingList() const;
private slots:
    void on_pushButton_clicked();
private:
    Ui::DialogTiming *ui;
    ModelTiming* model{nullptr};
};
#endif // DIALOGTIMING_H
