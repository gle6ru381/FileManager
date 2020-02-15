#include "mainwindow.h"
#include <QComboBox>
#include <QCoreApplication>
#include <QFileSystemModel>
#include <QString>

#define MAINPATH "D:/reposQt/FileManager"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
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

    QFileSystemModel* model = new QFileSystemModel(this);
    model->setRootPath("");
    model->setReadOnly(false);
    past.push_back(model->index(""));

    QTreeView* tree = new QTreeView(this);
    tree->setModel(model);
    tree->setRootIndex(past.first());
    tree->hideColumn(1);
    tree->hideColumn(2);
    tree->hideColumn(3);

    leftBar = new QToolBar(this);
    leftBar->setOrientation(Qt::Vertical);
    leftBar->setAllowedAreas(Qt::LeftToolBarArea);
    leftBar->setMaximumSize(150, 500);
    leftBar->addWidget(tree);
    leftBar->setMovable(false);
    leftBar->setFloatable(false);
    addToolBar(Qt::LeftToolBarArea, leftBar);

    mainTree = new QTreeView(this);
    mainTree->setModel(model);
    mainTree->setRootIndex(past.first());
    mainTree->setTreePosition(5);
    mainTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mainTree,
            SIGNAL(doubleClicked(const QModelIndex&)),
            this,
            SLOT(changedList(const QModelIndex&)));

    bar = new QAction(tr("bar"));
    table = new QAction(tr("table"));

    contextMenu = new QMenu(this);
    QMenu* view = new QMenu(tr("view"));
    view->addAction(bar);
    view->addAction(table);
    contextMenu->addMenu(view);
    connect(mainTree,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(viewMenu()));

    setCentralWidget(mainTree);
    setGeometry(0, 30, 800, 800);
}

void MainWindow::changedList(const QModelIndex& index)
{
    back->setEnabled(true);
    past.push_back(index);
    mainTree->setRootIndex(index);
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
