#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListView>
#include <QMainWindow>
#include <QPushButton>
#include <QToolBar>
#include <QTreeView>
#include <QVector>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:
    QToolBar* topBar;
    QToolBar* leftBar;
    QVector<QModelIndex> past;
    QPushButton* back;
    QTreeView* mainTree;
private slots:
    void changedList(const QModelIndex& index);
    void pressBack();
};
#endif // MAINWINDOW_H
