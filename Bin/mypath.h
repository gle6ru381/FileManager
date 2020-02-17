#ifndef MYDIR_H
#define MYDIR_H

#include <QDir>
#include <QFileSystemModel>

class MyPath : public QDir {
private:
    QString path;

public:
    MyPath(const QString& path = QString());
    void copyInDir(QString&& destination);
    void moveInDir(QString&& destination);
};

#endif // MYDIR_H
