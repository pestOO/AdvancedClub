/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**		Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 08.09.2013 by Elisey Zamakhov.
****************************************************************************/
#pragma once
#ifndef SELECTAUDIOFILE_H
#define SELECTAUDIOFILE_H
#include <QDialog>
#include "AudioFilesListModel.h"

namespace Ui {
        class SelectAudioFile;
    }
class SelectAudioFile : public QDialog
{
    Q_OBJECT
    explicit SelectAudioFile(QWidget *parent = 0);
public:
    static QString getAudioFile ();
    ~SelectAudioFile();
protected:
    QString getSelectedAudioFile() const;
private:
    Ui::SelectAudioFile *ui;
    AudioFilesListModel* filesModel;
private slots:
    void selectOtherAudio(bool);
};

#endif // SELECTAUDIOFILE_H
