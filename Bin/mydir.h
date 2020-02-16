#ifndef MYDIR_H
#define MYDIR_H

#include <QDir>
#include <QFileSystemModel>

class MyDir : public QDir {
private:
    QString path;

public:
    MyDir(const QString& path = QString());
    void copyInDir(QString&& destination);
    void moveInDir(QString&& destination);
};

#endif // MYDIR_H
