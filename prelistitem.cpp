#include "prelistitem.h"
#include <QListWidgetItem>

PreListItem::PreListItem(const QIcon &icon, const QString &text, const int& index,
                         QListWidget *listview, int type)
    : QListWidgetItem(icon, "", listview, type), _path(text), _index(index)
{

}

int PreListItem::GetIndex() const
{
    return _index;
}

QString PreListItem::GetPath() const
{
    return _path;
}
