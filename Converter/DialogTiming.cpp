#include "DialogTiming.h"
#include "ui_DialogTiming.h"
#include <QMenu>

DialogTiming::DialogTiming(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTiming)
    {
    ui->setupUi(this);
    ui->listTiming->setModel (model = new ModelTiming());

    ui->listTiming->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listTiming, &QWidget::customContextMenuRequested,
            [=](const QPoint& pos) {
                    const QPoint globalPos = ui->listTiming->mapToGlobal(pos);
                    QMenu myMenu;
                    myMenu.addAction(tr("Clear list"));
                    if (myMenu.exec(globalPos))
                        model->clear ();
                });
    }
DialogTiming::~DialogTiming()
    {
    delete ui;
    }
QSet<Timing> DialogTiming::getTimingList() const
    {
    Q_ASSERT(model);
    return model->getTimingList ();
    }
void DialogTiming::on_pushButton_clicked()
    {
    Timing timing;
    timing.majorSeconds = ui->boxMajorSeconds->value();
    timing.minorQuant = ui->boxMinorQuant->value();
    timing.minorSeconds = ui->boxMinorSeconds->value();
    Q_ASSERT(model);
    model->addTiming(timing);
    }
