#ifndef MYDIR_H
#define MYDIR_H

#include <QDir>
#include <QFileSystemModel>

class MyPath : public QDir {
private:
    QStringList paths;

public:
    enum TypeMove { Move = 0, Copy };
    MyPath(const QString& path = QString());
    void copyInDir(QString&& destination);
    void moveInDir(QString&& destination);
    void pushBack(QString path);
    void remove();
    void clearPaths();
};

#endif // MYDIR_H
