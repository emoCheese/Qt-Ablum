#ifndef PROSETPAGE_H
#define PROSETPAGE_H

#include <QWizard>

namespace Ui {
class ProSetPage;
}

class ProSetPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProSetPage(QWidget *parent = nullptr);
    ~ProSetPage();
    void GetProSettings(QString &name, QString &path);

protected:
    bool isComplete() const;  // 重写的基类函数，若在实现自己的功能后还想使用基类函数的功能，使用基类命名空间调用对应函数即可

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::ProSetPage *ui;

};

#endif // PROSETPAGE_H
