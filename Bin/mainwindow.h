#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
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
    QAction* bar;
    QAction* table;
    QMenu* contextMenu;
    QToolBar* topBar;
    QToolBar* leftBar;
    QVector<QModelIndex> past;
    QPushButton* back;
    QTreeView* mainTree;
private slots:
    void changedList(const QModelIndex& index);
    void pressBack();
    void viewMenu();
};
#endif // MAINWINDOW_H
