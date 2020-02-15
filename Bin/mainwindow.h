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
    QVector<QModelIndex> past;
    QPushButton* back;
    QTreeView* leftTree;
    QTreeView* mainTree;
    QListView* mainList;

    void createTopBar();
    void createLeftBar();
    void createContextMenu();
private slots:
    void changedList(const QModelIndex& index);
    void pressBack();
    void viewMenu();
    void pressBar(bool checked);
    void pressTable(bool checked);
};
#endif // MAINWINDOW_H
