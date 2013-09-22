/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**		Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 08.09.2013 by Elisey Zamakhov.
****************************************************************************/
#include "SelectAudioFile.h"
#include "ui_SelectAudioFile.h"
#include <QFileDialog>
#include <QPushButton>

SelectAudioFile::SelectAudioFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectAudioFile)
    {
    ui->setupUi(this);
    ui->listView->setModel (filesModel = new AudioFilesListModel(this));

    ui->buttonBox->addButton (tr("Cansel"), QDialogButtonBox::RejectRole);
    ui->buttonBox->addButton (tr("Select"), QDialogButtonBox::AcceptRole);
    QPushButton * selectOtherButton =
            ui->buttonBox->addButton (tr("Select other"), QDialogButtonBox::ActionRole);
    connect (selectOtherButton, &QPushButton::clicked, this, &SelectAudioFile::selectOtherAudio );
    connect (ui->listView, &QListView::clicked, filesModel, &AudioFilesListModel::playAudio);
    }
SelectAudioFile::~SelectAudioFile()
    {
    delete ui;
    }
QString SelectAudioFile::getAudioFile(Widget * parent)
    {
    SelectAudioFile dialog(parent);
    dialog.exec ();
    return dialog.getSelectedAudioFile ();
    }
QString SelectAudioFile::getSelectedAudioFile() const
    {
    return filesModel->getFile (ui->listView->currentIndex ());
    }
void SelectAudioFile::selectOtherAudio(bool)
    {
    QSettings settings;
    const QLatin1Literal settingSoundWay("SoundWay");
    const QString newSoundWay =
            QFileDialog::getOpenFileName(this, windowTitle (),
                                         settings.value (settingSoundWay).toString (),
                                         tr("Sound Files (%1)").arg (QLatin1Literal("*.wav *.mp3")));
    if(!newSoundWay.isEmpty())
        {
        settings.setValue (settingSoundWay, newSoundWay);
        const QModelIndex newIndex = filesModel->addFile (newSoundWay);
        ui->listView->setCurrentIndex (newIndex);
        filesModel->playAudio (newIndex);
        }
    }


