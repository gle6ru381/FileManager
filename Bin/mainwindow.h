#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mydir.h"
#include <QAction>
#include <QDir>
#include <QFileSystemModel>
#include <QListView>
#include <QMainWindow>
#include <QMenu>
#include <QPushButton>
#include <QToolBar>
#include <QTreeView>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:
    QFileSystemModel* model;
    QAction* bar;
    QAction* table;
    QMenu* contextMenu;
    QToolBar* topBar;
    QToolBar* leftBar;
    QModelIndexList past;
    QPushButton* back;
    QPushButton* copy;
    QPushButton* paste;
    QPushButton* home;
    QTreeView* leftTree;
    QListView* mainList;
    MyDir* copyDir;

    void createTopBar();
    void createLeftBar();
    void createContextMenu();
    void fileRun(QString);
private slots:
    void changedList(const QModelIndex& index);
    void changedTree(const QModelIndex& index);
    void pressBack();
    void viewMenu();
    void pressBar();
    void pressTable();
    void pressHome();
    void pressCopy();
    void pressPaste();
};
#endif // MAINWINDOW_H
