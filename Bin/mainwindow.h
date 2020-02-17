#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mypath.h"
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
    QPushButton* cut;
    QTreeView* leftTree;
    QListView* mainList;
    MyPath* copyDir;

    void createTopBar();
    void createLeftBar();
    void createLeftTree();
    void createMainList();
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
    void pressCut();
};
#endif // MAINWINDOW_H
