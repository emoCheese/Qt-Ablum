#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    virtual void resizeEvent(QResizeEvent *event);
private:
    Ui::MainWindow *ui;

    QWidget * _pro_tree;
    QWidget * _picshow;

private slots:
    void on_act_create_pro_triggered();
    void on_act_open_pro_triggered();

signals:
    void SigOpenPro(const QString& path);
};
#endif // MAINWINDOW_H
