#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDir>
#include <QAction>
#include <QProgressDialog>
#include <QtMultimedia/QMediaPlayer>
#include "protreethread.h"
#include "opentreethread.h"

class SlideShowDlg;

class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget * parent = nullptr);
    void AddProToTree(const QString& name, const QString& path);

private:
    QSet<QString> _set_path;               // 判断重名
    QTreeWidgetItem * _right_click_item;   // 获取被右键的item
    QTreeWidgetItem * _left_click_item;
    QTreeWidgetItem * _active_item;        // 保存被激活的项目
    QTreeWidgetItem * _selected_item;      // 双击选中的条目

    QAction * _act_import;
    QAction * _act_setstart;
    QAction * _act_closepro;
    QAction * _act_slideshow;

    QProgressDialog * _dialog_progress;

    std::shared_ptr<ProTreeThread> _thread_create_pro;
    std::shared_ptr<OpenTreeThread> _thread_open_pro;
    std::shared_ptr<SlideShowDlg> _slide_show_dlg;

    QMediaPlayer * _player;

private slots:
    void SlotItemPressed(QTreeWidgetItem *item, int column);
    void SlotImport();
    void SlotUpdateProgress(int);
    void SlotFinishProgress();
    void SlotCancelProgress();
    void SlotSetActive();
    void SlotClosePro();
    void SlotDoubleClickItem(QTreeWidgetItem *item, int column);
    void SlotSlideShow();

public slots:
    void SlotOpenPro(const QString& path);
    void SlotShowNext();
    void SlotShowPre();
    void SlotSetMusic();

signals:
    void SigCancelProgress();
    void SigUpdateSelected(const QString& path);
    void SigUpdatePic(const QString& path);
};

#endif // PROTREEWIDGET_H
