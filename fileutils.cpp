#include "fileutils.h"

#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

QList<QString> FileUtils::enumeratesAllFilesInTheDir(const QString &dirPath)
{
    QList<QString> result;
    QDir dir(dirPath);
    QFileInfoList infoList = dir.entryInfoList();
    for(const QFileInfo& iter : infoList){
        if(iter.isFile()){
            result.append(iter.absoluteFilePath());
        }else if(iter.isDir() &&
                 !iter.fileName().endsWith(".") &&
                 !iter.fileName().endsWith("..")){
            QList<QString> tmp = enumeratesAllFilesInTheDir(iter.absoluteFilePath());
            result.append(tmp);
        }
    }
    return result;
}
