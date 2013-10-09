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
#ifndef AUDIOFILESLISTMODEL_H
#define AUDIOFILESLISTMODEL_H
#include <QAbstractListModel>
#include <QtCore>
#include <QMediaPlayer>

class AudioFilesListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AudioFilesListModel(QObject *parent = 0);
    ~AudioFilesListModel();
    int rowCount (const QModelIndex &) const override;
    QVariant data (const QModelIndex &index, int role) const override;
    QString getFile(const QModelIndex &index) const;
    void playAudio(const QModelIndex &index);
    QModelIndex addFile(const QString &file);
    static QStringList getAudioFileSuufixList();
private:
    void addFileByDir(const QDir dir);
    QFileInfoList files;
    QMediaPlayer player;
};
#endif // AUDIOFILESLISTMODEL_H
