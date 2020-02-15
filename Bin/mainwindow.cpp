#include "mainwindow.h"
#include <QCoreApplication>
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

    mainTree = new QTreeView(this);
    mainTree->setModel(model);
    mainTree->setRootIndex(past.first());
    mainTree->setTreePosition(5);
    mainTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mainTree,
            SIGNAL(doubleClicked(const QModelIndex&)),
            this,
            SLOT(changedList(const QModelIndex&)));

    mainList = new QListView(this);
    mainList->setModel(model);
    mainList->setRootIndex(past.first());
    mainList->setViewMode(QListView::IconMode);
    // mainList->setLayoutMode(QListView::Batched);
    mainList->setGridSize(QSize(100, 100));
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
    back->setEnabled(true);
    past.push_back(index);
    mainTree->setRootIndex(index);
    mainList->setRootIndex(index);
}

void MainWindow::pressBack()
{
    if (past.begin() != past.end() - 1) {
        past.removeLast();
        mainTree->setRootIndex(past.last());
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
    connect(bar, SIGNAL(triggered(bool)), this, SLOT(pressBar(bool)));
    connect(table, SIGNAL(triggered(bool)), this, SLOT(pressTable(bool)));

    contextMenu = new QMenu(this);
    QMenu* view = new QMenu(tr("view"));
    view->addAction(bar);
    view->addAction(table);
    contextMenu->addMenu(view);
    connect(mainTree,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(viewMenu()));
    connect(mainList,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(viewMenu()));
}

void MainWindow::pressBar(bool checked)
{
    setCentralWidget(mainList);
}

void MainWindow::pressTable(bool checked)
{
    mainTree = new QTreeView;
    mainTree->setModel(model);
    mainTree->setRootIndex(past.last());
    setCentralWidget(mainTree);
}
