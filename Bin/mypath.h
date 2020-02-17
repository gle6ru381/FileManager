#ifndef MYDIR_H
#define MYDIR_H

#include <QDir>
#include <QFileSystemModel>

class MyPath : public QDir {
private:
    QStringList paths;

public:
    MyPath(const QString& path = QString());
    void copyInDir(QString&& destination);
    void moveInDir(QString&& destination);
    void pushBack(QString path);
    void clearPaths();
};

#endif // MYDIR_H
