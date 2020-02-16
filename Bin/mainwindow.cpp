#include "mainwindow.h"
#include <QCoreApplication>
#include <QFileIconProvider>
#include <QProcess>
#include <QString>
#include <QTableView>

#define MAINPATH "D:/reposQt/FileManager"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    model = new QFileSystemModel(this);
    model->setRootPath("");
    model->setReadOnly(false);
    past.push_back(model->index(""));

    leftTree = new QTreeView(this);
    leftTree->setModel(model);
    leftTree->setRootIndex(past.first());
    leftTree->hideColumn(1);
    leftTree->hideColumn(2);
    leftTree->hideColumn(3);
    leftTree->setHeaderHidden(true);

    mainList = new QListView(this);
    mainList->setModel(model);
    mainList->setRootIndex(past.first());
    mainList->setViewMode(QListView::IconMode);
    mainList->setLayoutMode(QListView::Batched);
    // mainList->setGridSize(QSize(100, 100));
    mainList->setUniformItemSizes(true);
    mainList->setWordWrap(true);
    mainList->setResizeMode(QListView::Adjust);
    mainList->setIconSize(QSize(80, 80));
    mainList->setEditTriggers(QAbstractItemView::SelectedClicked);
    connect(mainList,
            SIGNAL(doubleClicked(const QModelIndex&)),
            this,
            SLOT(changedList(const QModelIndex&)));
    mainList->setContextMenuPolicy(Qt::CustomContextMenu);

    createTopBar();
    createLeftBar();
    createContextMenu();

    setCentralWidget(mainList);
    setGeometry(0, 30, 800, 800);
}

void MainWindow::changedList(const QModelIndex& index)
{
    if (model->isDir(index)) {
        back->setEnabled(true);
        past.push_back(index);
        mainList->setRootIndex(index);
    } else {
        fileRun(model->filePath(index));
    }
}

void MainWindow::pressBack()
{
    if (past.begin() != past.end() - 1) {
        past.removeLast();
        mainList->setRootIndex(past.last());
        if (past.first() == past.last())
            back->setEnabled(false);
    } else
        back->setEnabled(false);
}

void MainWindow::viewMenu()
{
    contextMenu->exec(QCursor::pos());
}

void MainWindow::createTopBar()
{
    topBar = new QToolBar(this);
    topBar->setAllowedAreas(Qt::TopToolBarArea);
    topBar->setMovable(false);
    topBar->setFloatable(false);
    back = new QPushButton(topBar);
    back->setEnabled(false);
    back->setIcon(QIcon(QString("%1/pics/go-previous.png").arg(MAINPATH)));
    back->setIconSize(QSize(40, 40));
    connect(back, SIGNAL(released()), this, SLOT(pressBack()));
    topBar->addWidget(back);
    addToolBar(Qt::TopToolBarArea, topBar);
}

void MainWindow::createLeftBar()
{
    leftBar = new QToolBar(this);
    leftBar->setOrientation(Qt::Vertical);
    leftBar->setAllowedAreas(Qt::LeftToolBarArea);
    leftBar->setMaximumSize(180, 500);
    leftBar->addWidget(leftTree);
    leftBar->setMovable(false);
    leftBar->setFloatable(false);
    addToolBar(Qt::LeftToolBarArea, leftBar);
}

void MainWindow::createContextMenu()
{
    bar = new QAction(tr("bar"));
    table = new QAction(tr("table"));
    connect(bar, SIGNAL(triggered(bool)), this, SLOT(pressBar()));
    connect(table, SIGNAL(triggered(bool)), this, SLOT(pressTable()));

    contextMenu = new QMenu(this);
    QMenu* view = new QMenu(tr("view"));
    view->addAction(bar);
    view->addAction(table);
    contextMenu->addMenu(view);
    connect(mainList,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(viewMenu()));
}

void MainWindow::pressBar()
{
    mainList->setIconSize(QSize(80, 80));
    mainList->setViewMode(QListView::IconMode);
}

void MainWindow::pressTable()
{
    mainList->setIconSize(QSize(25, 25));
    mainList->setViewMode(QListView::ListMode);
}

void MainWindow::fileRun(QString filePath)
{
    QProcess* process = new QProcess(this);
    QString extension = filePath.split("/").last().split(".").last();
    if (extension == "txt" || extension == "log")
        process->start(QString("notepad %1").arg(filePath));
    else if (extension == "exe")
        process->startDetached(filePath);
}
