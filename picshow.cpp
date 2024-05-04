#include "picshow.h"
#include "ui_picshow.h"

PicShow::PicShow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PicShow)
{
    ui->setupUi(this);
    ui->previousBtn->SetIcons(":/res/icon/previous.png", ":/res/icon/previous_hover.png", ":/res/icon/previous_press.png");
    ui->nextBtn->SetIcons(":/res/icon/next.png", ":/res/icon/next_hover.png", ":/res/icon/next_press.png");

    // 设置按钮完全透明
    QGraphicsOpacityEffect * opacity_pre = new QGraphicsOpacityEffect(this);
    opacity_pre->setOpacity(0);
    ui->previousBtn->setGraphicsEffect(opacity_pre);

    QGraphicsOpacityEffect * opacity_next = new QGraphicsOpacityEffect(this);
    opacity_next->setOpacity(0);
    ui->nextBtn->setGraphicsEffect(opacity_next);

    _animation_show_pre = new QPropertyAnimation(opacity_pre, "opacity", this);
    _animation_show_pre->setEasingCurve(QEasingCurve::Linear);
    _animation_show_pre->setDuration(500);

    _animation_show_next = new QPropertyAnimation(opacity_next, "opacity", this);
    _animation_show_next->setEasingCurve(QEasingCurve::Linear);
    _animation_show_next->setDuration(500);

    connect(ui->nextBtn, &QPushButton::clicked, this, &PicShow::SigNextClicked);
    connect(ui->previousBtn, &QPushButton::clicked, this, &PicShow::SigPreClicked);
}

PicShow::~PicShow()
{
    delete ui;
}

void PicShow::ReloadPic()
{
    if(_selected_path != "")
    {
        const auto& width = ui->gridLayout->geometry().width();
        const auto& height = ui->gridLayout->geometry().height();
        _pixmap.load(_selected_path);
        _pixmap = _pixmap.scaled(width, height, Qt::KeepAspectRatio);
        ui->label->setPixmap(_pixmap);
    }
}

bool PicShow::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::Enter:
        ShowPreNextBtns(true);
        break;
    case QEvent::Leave:
        ShowPreNextBtns(false);
        break;
    default:
        break;
    }
    return QDialog::event(event);
}

void PicShow::ShowPreNextBtns(bool b_show)
{
    if(!b_show && _b_btnvisible)
    {
        _animation_show_pre->stop();
        _animation_show_pre->setStartValue(1);
        _animation_show_pre->setEndValue(0);
        _animation_show_pre->start();

        _animation_show_next->stop();
        _animation_show_next->setStartValue(1);
        _animation_show_next->setEndValue(0);
        _animation_show_next->start();

        _b_btnvisible = false;
        return;
    }
    if(b_show && !_b_btnvisible)
    {
        _animation_show_pre->stop();
        _animation_show_pre->setStartValue(0);
        _animation_show_pre->setEndValue(1);
        _animation_show_pre->start();

        _animation_show_next->stop();
        _animation_show_next->setStartValue(0);
        _animation_show_next->setEndValue(1);
        _animation_show_next->start();

        _b_btnvisible = true;
        return;
    }
}

void PicShow::SlotUpdateSelectItem(const QString &path)
{
    _selected_path = path;
    _pixmap.load(path);
    auto width = this->width() - 20;
    auto height = this->height() - 20;
    _pixmap = _pixmap.scaled(width, height, Qt::KeepAspectRatio);
    ui->label->setPixmap(_pixmap);
}

void PicShow::SlotUpdatePic(const QString &path)
{
    _selected_path = path;
    if(_selected_path != "")
    {
        const auto& width = ui->gridLayout->geometry().width();
        const auto& height = ui->gridLayout->geometry().height();
        _pixmap.load(_selected_path);
        _pixmap = _pixmap.scaled(width, height, Qt::KeepAspectRatio);
        ui->label->setPixmap(_pixmap);
    }
}
