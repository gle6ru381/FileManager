#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QFileSystemModel>
#include <QListView>
#include <QMainWindow>
#include <QMenu>
#include <QPushButton>
#include <QToolBar>
#include <QTreeView>
#include <QVector>

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
    QTreeView* leftTree;
    QListView* mainList;

    void createTopBar();
    void createLeftBar();
    void createContextMenu();
private slots:
    void changedList(const QModelIndex& index);
    void pressBack();
    void viewMenu();
    void pressBar();
    void pressTable();
};
#endif // MAINWINDOW_H
