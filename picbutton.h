#ifndef PICBUTTON_H
#define PICBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QEvent>

class PicButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PicButton(QWidget *parent = nullptr);
    void SetIcons(const QString& normal, const QString& hover, const QString& pressed);

protected:
    bool event(QEvent *e) override;

private:
    void setNormalIcon();  // 正常状态下按钮
    void setHoverIcon();   // 鼠标悬浮状态下按钮
    void setPressedIcon(); // 鼠标按下状态下按钮

    QString _normal;
    QString _hover;
    QString _pressed;
};

#endif // PICBUTTON_H
