#include "slideshowdlg.h"
#include "ui_slideshowdlg.h"
#include "prelistwid.h"

SlideShowDlg::SlideShowDlg(QWidget *parent, QTreeWidgetItem * first_item, QTreeWidgetItem * last_item)
    : QDialog(parent)
    , ui(new Ui::SlideShowDlg)
    , _first_item(first_item), _last_item(last_item)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->slidepreBtn->SetIcons(":/res/icon/previous.png", ":/res/icon/previous_hover.png", ":/res/icon/previous_press.png");
    ui->slidenextBtn->SetIcons(":/res/icon/next.png", ":/res/icon/next_hover.png", ":/res/icon/next_press.png");

    ui->closeBtn->SetIcons(":/res/icon/closeshow.png", ":/res/icon/closeshow_hover.png", ":/res/icon/closeshow_press.png");
    // 设置播放和暂停
    ui->playBtn->SetIcons(":/res/icon/play.png", ":/res/icon/play_hover.png", ":/res/icon/play_press.png"
                          , ":/res/icon/pause.png", ":/res/icon/pause_hover.png", ":/res/icon/pause_press.png");
    connect(ui->closeBtn, &QPushButton::clicked, this, &SlideShowDlg::close);
    connect(ui->slidenextBtn, &QPushButton::clicked, this, &SlideShowDlg::SlotSlideNext);
    connect(ui->slidepreBtn, &QPushButton::clicked, this, &SlideShowDlg::SlotSlidePre);


    auto * prelistWid = dynamic_cast<PreListWid*>(ui->preListWidget);
    connect(ui->picAnimation, &PicAnimationWid::SigUpPreList, prelistWid, &PreListWid::SlotUpPreList);
    connect(ui->picAnimation, &PicAnimationWid::SigSelectItem, prelistWid, &PreListWid::SlotSelectItem);

    connect(prelistWid, &PreListWid::SigUpSelectShow, ui->picAnimation, &PicAnimationWid::SlotUpSelectShow);

    connect(ui->playBtn, &PicStateBtn::clicked, ui->picAnimation, &PicAnimationWid::SlotStartOrStop);
    connect(ui->picAnimation, &PicAnimationWid::SigStart, ui->playBtn, &PicStateBtn::SlotStart);
    connect(ui->picAnimation, &PicAnimationWid::SigStop, ui->playBtn, &PicStateBtn::SlotStop);

    ui->picAnimation->SetPixmap(_first_item);
    ui->picAnimation->Start();
}

SlideShowDlg::~SlideShowDlg()
{
    delete ui;
}

void SlideShowDlg::SlotSlideNext()
{
    ui->picAnimation->SlideNext();
}

void SlideShowDlg::SlotSlidePre()
{
    ui->picAnimation->SlidePre();
}
