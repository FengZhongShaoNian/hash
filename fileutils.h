#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QtCore>

class FileUtils
{
public:

    // 枚举dirPath下的所有文件及文件夹，将所有的文件（不包括文件夹）的绝对路径放到QList中并返回
    static QList<QString> enumeratesAllFilesInTheDir(const QString& dirPath);
};

#endif // FILEUTILS_H
