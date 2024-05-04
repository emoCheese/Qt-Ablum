#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include "protree.h"
#include "wizard.h"
#include "protreewidget.h"
#include "picshow.h"
#include "protree.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setMinimumSize(1629, 869);
    ui->setupUi(this);
    // 设置菜单
    ui->menu_file->addAction(ui->act_create_pro);
    ui->menu_file->addAction(ui->act_open_pro);
    ui->menu_setting->addAction(ui->act_music);



    _pro_tree = new ProTree(this);
    QTreeWidget* tree_widget = dynamic_cast<ProTree*>(_pro_tree)->GetTreeWidget();
    auto * pro_tree_widget = dynamic_cast<ProTreeWidget*>(tree_widget);
    connect(this, &MainWindow::SigOpenPro, pro_tree_widget, &ProTreeWidget::SlotOpenPro);
    ui->proLayout->addWidget(_pro_tree);

    _picshow = new PicShow(this);
    ui->picLayout->addWidget(_picshow);

    auto * pro_pic_show = dynamic_cast<PicShow*>(_picshow);
    connect(pro_tree_widget, &ProTreeWidget::SigUpdateSelected, pro_pic_show, &PicShow::SlotUpdateSelectItem);

    connect(pro_pic_show, &PicShow::SigNextClicked, pro_tree_widget, &ProTreeWidget::SlotShowNext);
    connect(pro_pic_show, &PicShow::SigPreClicked, pro_tree_widget, &ProTreeWidget::SlotShowPre);
    connect(pro_tree_widget, &ProTreeWidget::SigUpdatePic, pro_pic_show, &PicShow::SlotUpdatePic);

    connect(ui->act_music, &QAction::triggered, pro_tree_widget, &ProTreeWidget::SlotSetMusic);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    auto * pro_pic_show = dynamic_cast<PicShow*>(_picshow);
    pro_pic_show->ReloadPic();
    QMainWindow::resizeEvent(event);
}


void MainWindow::on_act_create_pro_triggered()
{
    qDebug() << "slot create pro triggered" << Qt::endl;
    Wizard wizard(this);
    wizard.setWindowTitle(tr("创建项目"));
    auto * page = wizard.page(0);
    page->setTitle(tr("设置项目配置"));
    // 连接信号和槽，把项目配置传回来
    connect(&wizard, &Wizard::SigProSettings, dynamic_cast<ProTree*>(_pro_tree), &ProTree::AddProToTree);


    wizard.show();
    wizard.exec();
    // 断开所有信号
    disconnect();
}


void MainWindow::on_act_open_pro_triggered()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle(tr("选择导入的项目"));
    file_dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if(file_dialog.exec())
        fileNames = file_dialog.selectedFiles();
    if(fileNames.length() <= 0)
        return;
    QString import_path = fileNames.at(0);
    emit SigOpenPro(import_path);
}


