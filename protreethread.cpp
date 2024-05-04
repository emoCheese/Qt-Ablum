#include "protreethread.h"

#include <QDir>
#include "protreeitem.h"
#include "const.h"

ProTreeThread::ProTreeThread(const QString &src_path,
                             const QString &dist_path,
                             QTreeWidgetItem *parent_item,
                             int file_count,
                             QTreeWidget *self,
                             QTreeWidgetItem *root,
                             QObject *parent)
    : QThread(parent)
    , _src_path(src_path)
    , _dist_path(dist_path)
    , _file_count(file_count)
    , _parent_item(parent_item)
    , _self(self)
    , _stop(false)
{
}

ProTreeThread::~ProTreeThread()
{

}

void ProTreeThread::run()
{
    CreateProTree(_src_path, _dist_path, _parent_item, _file_count, _self, _root);
    if(_stop)
    {
        auto path = dynamic_cast<ProTreeItem*>(_root)->GetPath();
        auto index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }

    emit SigFinishProgress(_file_count);
}

void ProTreeThread::CreateProTree(const QString &src_path,
                                  const QString &dist_path,
                                  QTreeWidgetItem *parent_item,
                                  int &file_count,
                                  QTreeWidget *self,
                                  QTreeWidgetItem *root,
                                  QTreeWidgetItem *preItem)
{
    if(_stop)
        return;
    bool needcopy = true;
    if(src_path == dist_path)
        needcopy = false;
    QDir import_dir(src_path);
    QStringList nameFilters;
    import_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot); // 将需要的文件放入过滤器
    import_dir.setSorting(QDir::Name);
    QFileInfoList list = import_dir.entryInfoList();   // 获取导入目录下的所有文件
    for(int i = 0; i <list.size(); ++i)
    {
        if(_stop)
            return;
        QFileInfo fileInfo = list.at(i);
        bool bIsDir = fileInfo.isDir();

        if(bIsDir)          // 当前目录是文件夹
        {
            if(_stop)
                return;
            file_count++;
            emit SigUpdateProgress(file_count);
            QDir dist_dir(_dist_path);
            QString sub_dist_path = dist_dir.absoluteFilePath(fileInfo.fileName());
            QDir sub_dist_dir(sub_dist_path);
            if(!sub_dist_dir.exists())
            {
                bool ok = sub_dist_dir.mkpath(sub_dist_path);
                if(!ok)
                    continue;
            }
            auto * item = new ProTreeItem(parent_item, fileInfo.fileName(), sub_dist_path, root, TreeItemDir);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/res/icon/dir.png"));
            item->setData(0, Qt::ToolTipRole, sub_dist_path);
            // parent_item->addChild(item);
            CreateProTree(fileInfo.absoluteFilePath(), sub_dist_path, item, file_count, self, root, preItem);
        }
        else                // 当前目录是文件
        {
            if(_stop)
                return;
            const QString& suffix = fileInfo.completeSuffix();
            if(suffix != "png" && suffix != "jpeg" && suffix != "jpg")
                continue;
            file_count++;
            emit SigUpdateProgress(file_count);
            if(!needcopy)
                continue;
            QDir dist_dri(dist_path);
            // 将要复制的文件名和目的的绝对路径拼接在一起, absoluteFilePath 不会检测文件是否存在
            QString dist_file_path = dist_dri.absoluteFilePath(fileInfo.fileName());
            if(!QFile::copy(fileInfo.absoluteFilePath(), dist_file_path))              // 拷贝失败
                continue;
            auto * item = new ProTreeItem(parent_item, fileInfo.fileName(), dist_file_path, root, TreeItemPic);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/res/icon/pic.png"));
            item->setData(0, Qt::ToolTipRole, dist_file_path);
            // parent_item->addChild(item);
            if(preItem)
            {
                auto * pre_proitem = dynamic_cast<ProTreeItem*>(preItem);
                pre_proitem->SetNextItem(item);
            }
            item->SetPreItem(preItem);
            preItem = item;
        }
    }
}

void ProTreeThread::SlotCancelProgress()
{
    this->_stop = true;
}
