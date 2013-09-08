/****************************************************************************
**
** Copyright(c) 2013 Advance Club
** All rights reserved.
** Authors contacts:
**		Elisey Zamakhov (EliseyZamahov@gmail.com)
**
** Created 08.09.2013 by Elisey Zamakhov.
****************************************************************************/
#include "AudioFilesListModel.h"

AudioFilesListModel::AudioFilesListModel(QObject *parent) :
    QAbstractListModel(parent)
    {
    const QString windowsWay = qgetenv("WINDIR");
    addFileByDir(QDir());
    QDir dir(windowsWay);
    if(dir.cd("Media"))
       addFileByDir(dir);
    }
int AudioFilesListModel::rowCount(const QModelIndex & ) const
    {
    return files.size ();
    }
QVariant AudioFilesListModel::data(const QModelIndex & index, int role) const
    {
    if(Qt::DisplayRole == role)
        return files.at (index.row ()).fileName ();
    if(Qt::ToolTipRole == role)
        return files.at (index.row ()).absoluteFilePath ();
    return QVariant();
    }
QString AudioFilesListModel::getFile(const QModelIndex & index) const
    {
    if(index.isValid ())
        return files.at (index.row ()).absoluteFilePath ();
    return QString();
    }
void AudioFilesListModel::playAudio(const QModelIndex & index)
    {
    if(index.isValid ())
        {
        player.stop ();
        const QString fileWay = files.at (index.row ()).absoluteFilePath ();
        player.setMedia(QUrl::fromLocalFile(fileWay));
        player.play ();
        }
    }
QModelIndex AudioFilesListModel::addFile(const QString & file)
    {
    const QFileInfo newFileInfo(file);
    if(!files.contains (newFileInfo))
        files << newFileInfo;
    layoutChanged ();
    return createIndex (files.indexOf (newFileInfo), 0);
    }
void AudioFilesListModel::addFileByDir(const QDir dir)
    {
    const QStringList filters{"*.mp3","*.wav"};
    const QFileInfoList newFiles =
            dir.entryInfoList (filters, QDir::Files | QDir::NoDotAndDotDot);
    for(const QFileInfo& info : newFiles)
        if(!files.contains (info))
            files << info;
    }
