#include "protreewidget.h"

#include "protreeitem.h"
#include "const.h"
#include <QHeaderView>
#include <QGuiApplication>
#include <QMenu>
#include <QFileDialog>
#include <QDebug>
#include <picshow.h>

#include "removeprodialog.h"
#include "slideshowdlg.h"


ProTreeWidget::ProTreeWidget(QWidget * parent)
    : _right_click_item(nullptr), _active_item(nullptr), _dialog_progress(nullptr), _selected_item(nullptr)
    , _thread_create_pro(nullptr), _thread_open_pro(nullptr)
{
    this->header()->hide();
    connect(this, &ProTreeWidget::itemPressed, this, &ProTreeWidget::SlotItemPressed);
    _act_import = new QAction(QIcon(":/res/icon/import.png"), tr("导入文件"), this);
    _act_setstart = new QAction(QIcon(":/res/icon/core.png"), tr("设置活动项目"), this);
    _act_closepro = new QAction(QIcon(":/res/icon/close.png"), tr("关闭项目"), this);
    _act_slideshow = new QAction(QIcon(":/res/icon/slideshow.png"), tr("轮播图播放"), this);

    connect(_act_import, &QAction::triggered, this, &ProTreeWidget::SlotImport);
    connect(_act_setstart, &QAction::triggered, this, &ProTreeWidget::SlotSetActive);
    connect(_act_closepro, &QAction::triggered, this, &ProTreeWidget::SlotClosePro);  
    connect(_act_slideshow, &QAction::triggered, this, &ProTreeWidget::SlotSlideShow);

    connect(this, &ProTreeWidget::itemDoubleClicked, this, &ProTreeWidget::SlotDoubleClickItem);
}

void ProTreeWidget::AddProToTree(const QString &name, const QString &path)
{
    QDir dir(path);
    QString file_path = dir.absoluteFilePath(name);
    if(_set_path.find(file_path) != _set_path.end())
        return;

    QDir pro_dir(file_path);
    if(!pro_dir.exists())
    {
        bool enable = pro_dir.mkpath(file_path);
        if(!enable)
            return;
    }
    _set_path.insert(file_path);
    auto * item = new ProTreeItem(this, name, file_path, TreeItemPro);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/res/icon/dir.png"));
    item->setData(0, Qt::ToolTipRole, file_path);
    this->addTopLevelItem(item);
}

void ProTreeWidget::SlotItemPressed(QTreeWidgetItem *pressedItem, int column)
{
    if(QGuiApplication::mouseButtons() == Qt::RightButton)
    {
        QMenu menu(this);
        int itemType = pressedItem->type();
        if(itemType == TreeItemPro)
        {
            _right_click_item = pressedItem;
            menu.addAction(_act_import);
            menu.addAction(_act_setstart);
            menu.addAction(_act_closepro);
            menu.addAction(_act_slideshow);
            menu.exec(QCursor::pos());
        }
    }
}

void ProTreeWidget::SlotImport()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle(tr("选择导入的文件夹"));
    QString path = "";
    if(!_right_click_item)
    {
        qDebug()<<"_right_click_item is empty" << Qt::endl;
        return;
    }
    path = dynamic_cast<ProTreeItem*>(_right_click_item)->GetPath();
    file_dialog.setDirectory(path);
    file_dialog.setViewMode(QFileDialog::Detail);
    QStringList filenames;
    if(file_dialog.exec())
        filenames = file_dialog.selectedFiles();
    if(filenames.length() <= 0)
        return;

    QString import_path = filenames.at(0);
    int file_count = 0;
    _dialog_progress = new QProgressDialog(this);

    _thread_create_pro =
        std::make_shared<ProTreeThread>(std::ref(import_path), std::ref(path), _right_click_item, file_count, this, _right_click_item, nullptr);

    connect(_thread_create_pro.get(), &ProTreeThread::SigUpdateProgress, this, &ProTreeWidget::SlotUpdateProgress);
    connect(_thread_create_pro.get(), &ProTreeThread::SigFinishProgress, this, &ProTreeWidget::SlotFinishProgress);
    connect(_dialog_progress, &QProgressDialog::canceled, this, &ProTreeWidget::SlotCancelProgress);
    connect(this, &ProTreeWidget::SigCancelProgress, _thread_create_pro.get(), &ProTreeThread::SlotCancelProgress);

    _thread_create_pro->start();
    _dialog_progress->setWindowTitle("Please wati...");
    _dialog_progress->setFixedWidth(PROGRESS_WIDTH);
    _dialog_progress->setRange(0, PROGRESS_WIDTH);
    _dialog_progress->exec();
}

void ProTreeWidget::SlotUpdateProgress(int count)
{
    if(!_dialog_progress)
        return;
    if(count >= PROGRESS_MAX)
    {
        _dialog_progress->setValue(count&PROGRESS_MAX);
    }
    else
        _dialog_progress->setValue(count);
}

void ProTreeWidget::SlotFinishProgress()
{
    _dialog_progress->setValue(PROGRESS_MAX);
    _dialog_progress->deleteLater();
}

void ProTreeWidget::SlotCancelProgress()
{
    emit SigCancelProgress();
    delete _dialog_progress;
    _dialog_progress = nullptr;
}

void ProTreeWidget::SlotSetActive()
{
    if(!_right_click_item)
        return;
    QFont nullFont;
    nullFont.setBold(false);
    // 若之前有选中项，取消之前的字体加粗
    if(_active_item)
        _active_item->setFont(0, nullFont);
    // 将当前选中项设置为活动项
    _active_item = _right_click_item;
    nullFont.setBold(true);
    _active_item->setFont(0, nullFont);
}

void ProTreeWidget::SlotClosePro()
{
    RemoveProDialog remove_pro_dialog;
    auto res = remove_pro_dialog.exec();
    if(res == QDialog::Rejected)
        return;

    bool remove = remove_pro_dialog.IsRemoved();
    auto index_right_btn = this->indexOfTopLevelItem(_right_click_item);
    auto protreeitem = dynamic_cast<ProTreeItem*>(_right_click_item);
    auto delete_path = protreeitem->GetPath();
    _set_path.remove(delete_path);
    if(remove)
    {
        QDir delete_dir(delete_path);
        delete_dir.removeRecursively();
    }
    if(protreeitem == _active_item)
        _active_item = nullptr;
    delete this->takeTopLevelItem(index_right_btn);
    _right_click_item = nullptr;

}

void ProTreeWidget::SlotDoubleClickItem(QTreeWidgetItem *item, int column)
{
    if(QGuiApplication::mouseButtons() == Qt::LeftButton)
    {
        auto * tree_item = dynamic_cast<ProTreeItem*>(item);
        if(!tree_item)
            return;
        int itemType = tree_item->type();
        if(itemType == TreeItemPic)
        {
            emit SigUpdateSelected(tree_item->GetPath());
            _selected_item = tree_item;
        }
    }
}

void ProTreeWidget::SlotSlideShow()
{
    if(!_right_click_item)
        return;
    auto * right_pro_item = dynamic_cast<ProTreeItem*>(_right_click_item);
    auto * last_child_item = right_pro_item->GetLastPicChild();
    if(!last_child_item)
        return;
    auto * first_child_item = right_pro_item->GetFirstPicChild();
    if(!first_child_item)
        return;
    _slide_show_dlg = std::make_shared<SlideShowDlg>(this, first_child_item, last_child_item);

    _slide_show_dlg->setModal(true);  // 设置模态对话框
    _slide_show_dlg->showMaximized();
}

void ProTreeWidget::SlotOpenPro(const QString &path)
{
    if(_set_path.find(path) != _set_path.end())  // 判断文件是否已经打开
        return;

    _set_path.insert(path);
    int file_count = 0;
    QDir pro_dir(path);
    const QString proname = pro_dir.dirName();

    _thread_open_pro = std::make_shared<OpenTreeThread>(std::ref(path), std::ref(file_count), this, nullptr);
    _thread_open_pro->start();
}

void ProTreeWidget::SlotShowNext()
{
    if(!_selected_item)
        return;
    auto * curItem = dynamic_cast<ProTreeItem*>(_selected_item)->GetNextItem();
    if(!curItem)
        return;
    emit SigUpdatePic(curItem->GetPath());
    _selected_item = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::SlotShowPre()
{
    if(!_selected_item)
        return;
    auto * curItem = dynamic_cast<ProTreeItem*>(_selected_item)->GetPreItem();
    if(!curItem)
        return;
    emit SigUpdatePic(curItem->GetPath());
    _selected_item = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::SlotSetMusic()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::ExistingFiles);
    file_dialog.setWindowTitle(tr("选择背景音乐"));
    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);
    file_dialog.setNameFilter("(*.mp3)");
    QStringList fileNames;
    if(file_dialog.exec())
        fileNames = file_dialog.selectedFiles();
    else
        return;
    if(fileNames.count() <= 0)
        return;

}
