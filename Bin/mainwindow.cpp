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
    connect(leftTree,
            SIGNAL(clicked(const QModelIndex&)),
            this,
            SLOT(changedTree(const QModelIndex&)));

    mainList = new QListView(this);
    mainList->setModel(model);
    mainList->setRootIndex(past.first());
    mainList->setViewMode(QListView::IconMode);
    mainList->setLayoutMode(QListView::Batched);
    // mainList->setGridSize(QSize(100, 100));
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
    back->setIconSize(QSize(30, 30));
    connect(back, SIGNAL(released()), this, SLOT(pressBack()));
    topBar->addWidget(back);

    copy = new QPushButton(topBar);
    copy->setIcon(QIcon(QString("%1/pics/edit-copy.png").arg(MAINPATH)));
    copy->setIconSize(QSize(30, 30));
    topBar->addWidget(copy);
    connect(copy, SIGNAL(released()), this, SLOT(pressCopy()));

    move = new QPushButton(topBar);
    move->setIcon(QIcon(QString("%1/pics/edit-cut.png").arg(MAINPATH)));
    move->setIconSize(QSize(30, 30));
    topBar->addWidget(move);
    connect(move, SIGNAL(released()), this, SLOT(pressCut()));

    home = new QPushButton(topBar);
    home->setIcon(QIcon(QString("%1/pics/go-home.png").arg(MAINPATH)));
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
    copyDir = new MyDir(model->filePath(mainList->currentIndex()));
}

void MainWindow::pressCut()
{
    copyDir->copyInDir(QString(model->filePath(mainList->rootIndex())));
}

void MainWindow::copyFile(const QString& source, QString&& destination)
{
    if (QFileInfo(source).isDir()) {
        QString dirName(source.split("/").last());
        destination += QDir::separator() + dirName;
        QDir().mkdir(destination);
        foreach (
                const QString& entry,
                QDir(source).entryList(
                        QDir::AllDirs | QDir::Files | QDir::Hidden
                        | QDir::NoDotAndDotDot))
            QFile::copy(
                    source + QDir::separator() + entry,
                    destination + QDir::separator() + entry);
    } else {
        if (QFile::exists(destination)) {
            QFile::copy(source, destination);
        }
    }
}
