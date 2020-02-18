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

    createLeftTree();
    createMainList();
    createTopBar();
    createLeftBar();
    createContextMenu();

    setCentralWidget(mainList);
    setGeometry(0, 30, 800, 800);
}

void MainWindow::createLeftTree()
{
    leftTree = new QTreeView(this);
    leftTree->setModel(model);
    leftTree->setRootIndex(past.first());
    leftTree->hideColumn(1);
    leftTree->hideColumn(2);
    leftTree->hideColumn(3);
    leftTree->setHeaderHidden(true);
    connect(leftTree,
            SIGNAL(clicked(const QModelIndex&)),
            this,
            SLOT(changedTree(const QModelIndex&)));
}

void MainWindow::createMainList()
{
    mainList = new MyListView(this);
    mainList->setModel(model);
    mainList->setRootIndex(past.first());
    mainList->setViewMode(QListView::IconMode);
    mainList->setLayoutMode(QListView::Batched);
    mainList->setUniformItemSizes(true);
    mainList->setWordWrap(true);
    mainList->setResizeMode(QListView::Adjust);
    mainList->setIconSize(QSize(80, 70));
    mainList->setEditTriggers(QAbstractItemView::SelectedClicked);
    mainList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(mainList,
            SIGNAL(doubleClicked(const QModelIndex&)),
            this,
            SLOT(changedList(const QModelIndex&)));
    mainList->setContextMenuPolicy(Qt::CustomContextMenu);
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
    back->setToolTip("Назад");
    back->setIconSize(QSize(30, 30));
    connect(back, SIGNAL(released()), this, SLOT(pressBack()));
    topBar->addWidget(back);

    copy = new QPushButton(topBar);
    copy->setIcon(QIcon(QString("%1/pics/edit-copy.png").arg(MAINPATH)));
    copy->setToolTip(tr("Копировать"));
    copy->setIconSize(QSize(30, 30));
    topBar->addWidget(copy);
    connect(copy, SIGNAL(released()), this, SLOT(pressCopy()));

    cut = new QPushButton(topBar);
    cut->setIcon(QIcon(QString("%1/pics/edit-cut.png").arg(MAINPATH)));
    cut->setToolTip(tr("Вырезать"));
    cut->setIconSize(QSize(30, 30));
    topBar->addWidget(cut);
    connect(cut, SIGNAL(released()), this, SLOT(pressCut()));

    paste = new QPushButton(topBar);
    paste->setIcon(QIcon(QString("%1/pics/edit-paste.png").arg(MAINPATH)));
    paste->setToolTip(tr("Вставить"));
    paste->setIconSize(QSize(30, 30));
    paste->setEnabled(false);
    topBar->addWidget(paste);
    connect(paste, SIGNAL(released()), this, SLOT(pressPaste()));

    home = new QPushButton(topBar);
    home->setIcon(QIcon(QString("%1/pics/go-home.png").arg(MAINPATH)));
    home->setToolTip(tr("Домой"));
    home->setIconSize(QSize(30, 30));
    connect(home, SIGNAL(released()), this, SLOT(pressHome()));
    topBar->addWidget(home);

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
    bar = new QAction(tr("Плитка"));
    table = new QAction(tr("Таблица"));
    connect(bar, SIGNAL(triggered(bool)), this, SLOT(pressBar()));
    connect(table, SIGNAL(triggered(bool)), this, SLOT(pressTable()));

    contextMenu = new QMenu(this);
    QMenu* view = new QMenu(tr("Вид"));
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
    mainList->setIconSize(QSize(80, 70));
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

void MainWindow::changedTree(const QModelIndex& index)
{
    if (model->isDir(index))
        changedList(index);
}

void MainWindow::pressHome()
{
    changedList(model->index(""));
}

void MainWindow::pressCopy()
{
    if (!mainList->getSelectedIndexes().isEmpty()) {
        paste->setEnabled(true);
        type = MyPath::Copy;
        QModelIndexList temp = mainList->getSelectedIndexes();
        selectedPaths = new MyPath;
        selectedPaths->clearPaths();
        foreach (QModelIndex path, temp)
            selectedPaths->pushBack(model->filePath(path));
    }
}

void MainWindow::pressPaste()
{
    if (type == MyPath::Move) {
        selectedPaths->moveInDir(
                QString(model->filePath(mainList->rootIndex())));
        paste->setEnabled(false);
    } else if (type == MyPath::Copy)
        selectedPaths->copyInDir(
                QString(model->filePath(mainList->rootIndex())));
}

void MainWindow::pressCut()
{
    if (!mainList->getSelectedIndexes().isEmpty()) {
        paste->setEnabled(true);
        type = MyPath::Move;
        QModelIndexList temp = mainList->getSelectedIndexes();
        selectedPaths = new MyPath;
        selectedPaths->clearPaths();
        foreach (QModelIndex path, temp)
            selectedPaths->pushBack(model->filePath(path));
    }
}
