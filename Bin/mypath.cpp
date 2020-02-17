#include "mypath.h"

MyPath::MyPath(const QString& Path) : QDir(Path)
{
}

void MyPath::copyInDir(QString&& destination)
{
    foreach (QString path, paths)
        if (QFileInfo(path).isDir()) {
            QString dirName(path.split("/").last());
            destination += QDir::separator() + dirName;
            QDir().mkdir(destination);
            foreach (
                    const QString& entry,
                    QDir(path).entryList(
                            QDir::AllDirs | QDir::Files | QDir::Hidden
                            | QDir::NoDotAndDotDot))
                QFile::copy(
                        path + QDir::separator() + entry,
                        destination + QDir::separator() + entry);
        } else if (QFileInfo(path).isFile()) {
            QString entry(path.split("/").last());
            QFile::copy(path, destination + QDir::separator() + entry);
        }
}

void MyPath::pushBack(QString path)
{
    paths.push_back(path);
}

void MyPath::clearPaths()
{
    paths.clear();
}
