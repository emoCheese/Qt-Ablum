#ifndef PRELISTITEM_H
#define PRELISTITEM_H

#include <QListWidget>

#include <QIcon>


class PreListItem : public QListWidgetItem
{
public:
    explicit PreListItem(const QIcon &icon, const QString &text, const int& index,
                         QListWidget *listview = nullptr, int type = Type);
    int GetIndex() const;
    QString GetPath() const;
private:
    QString _path;
    int _index;
};

#endif // PRELISTITEM_H
