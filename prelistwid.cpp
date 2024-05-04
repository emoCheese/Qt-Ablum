#include "prelistwid.h"
#include <QDebug>
#include <QPainter>
#include "protreeitem.h"
#include "const.h"
#include "prelistitem.h"
#include <QGuiApplication>

PreListWid::PreListWid(QWidget *parent)
    : QListWidget(parent), _global(0), _last_index(17)
{
    this->setViewMode(QListWidget::IconMode);
    this->setIconSize(QSize(PREICON_SIZE, PREICON_SIZE));
    this->setSpacing(5);

    connect(this, &PreListWid::itemPressed, this, &PreListWid::SlotItemPressed);
}

PreListWid::~PreListWid()
{

}

void PreListWid::AddListItem(const QString &path)
{
    QPixmap src_pixmap(path);
    src_pixmap = src_pixmap.scaled(PREICON_SIZE, PREICON_SIZE, Qt::KeepAspectRatio);
    QPixmap dist_pixmap(QSize(PREICON_SIZE, PREICON_SIZE));

    dist_pixmap.fill(QColor(220, 220, 220, 50));
    QPainter painter(&dist_pixmap);
    int src_width = src_pixmap.width();
    int src_heigth = src_pixmap.height();

    int dist_width = dist_pixmap.width();
    int dist_heigth = dist_pixmap.height();
    auto x = (dist_width - src_width) / 2;
    auto y = (dist_heigth - src_heigth) / 2;
    painter.drawPixmap(x, y, src_pixmap);
    _global++;
    PreListItem * pItem = new PreListItem(QIcon(dist_pixmap), path, _global, this);
    pItem->setSizeHint(QSize(PREICON_SIZE, PREICON_SIZE));
    this->addItem(pItem);
    _set_items[path] = pItem;
    if(_global == 1)
    {
        _pos_origin = this->pos();
    }
}

void PreListWid::SlotUpPreList(QTreeWidgetItem *tree_item)
{
    if(!tree_item)
    {
        qDebug() << "tree_item is empty" << Qt::endl;
        return;
    }

    auto * pro_item = dynamic_cast<ProTreeItem*>(tree_item);
    QString path = pro_item->GetPath();
    auto iter = _set_items.find(path);
    if(iter != _set_items.end())
        return;
    AddListItem(path);
}

void PreListWid::SlotSelectItem(QTreeWidgetItem *item)
{
    if(!item)
        return;
    auto * pro_item = dynamic_cast<ProTreeItem*>(item);
    QString path = pro_item->GetPath();
    auto iter = _set_items.find(path);
    if(iter == _set_items.end())
        return;
    auto * list_item = dynamic_cast<PreListItem*>(iter.value());
    int index = list_item->GetIndex();
    // 一页显示17项
    if(index > 17)
    {
        auto pos_cur = this->pos();
        this->move(pos_cur.x() - (index-_last_index)*100, pos_cur.y());
    }
    else
    {
        this->move(_pos_origin);
        _last_index = 17;
    }
    this->setCurrentItem(iter.value());
}

void PreListWid::SlotItemPressed(QListWidgetItem *item)
{
    if(QGuiApplication::mouseButtons() != Qt::LeftButton)
        return;
    auto * list_item = dynamic_cast<PreListItem*>(item);
    auto cur_index = list_item->GetIndex();
    QString path = list_item->GetPath();
    this->setCurrentItem(list_item);
    emit SigUpSelectShow(path);
}

