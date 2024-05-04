#ifndef PRELISTWID_H
#define PRELISTWID_H

#include <QListWidget>
#include <QTreeWidgetItem>
#include <QPoint>

class PreListWid : public QListWidget
{
    Q_OBJECT
public:
    explicit PreListWid(QWidget *parent = nullptr);
    ~PreListWid();

private:
    void AddListItem(const QString& path);

    QMap<QString, QListWidgetItem*> _set_items;
    int _global;
    QPoint _pos_origin;
    int _last_index;

signals:
    void SigUpSelectShow(const QString& path);

public slots:
    void SlotUpPreList(QTreeWidgetItem* tree_item);
    void SlotSelectItem(QTreeWidgetItem* item);
    void SlotItemPressed(QListWidgetItem * item);
};

#endif // PRELISTWID_H
