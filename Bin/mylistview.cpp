#include "mylistview.h"

MyListView::MyListView(QWidget* parent) : QListView(parent)
{
}

QModelIndexList MyListView::getSelectedIndexes()
{
    return selectedIndexes();
}
