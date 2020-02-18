#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mylistview.h"
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
    MyListView* mainList;
    MyPath* selectedPaths;
    MyPath::TypeMove type;

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
