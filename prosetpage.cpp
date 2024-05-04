#include "prosetpage.h"
#include "ui_prosetpage.h"

#include <QLineEdit>
#include <QDir>
#include <QFileDialog>

ProSetPage::ProSetPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    // 将 lineEdit注册为 wizard 的 field 保证两个lineEdit是空的时候无法点击下一步
    registerField("proPath", ui->lineEdit_2);
    registerField("proName", ui->lineEdit);

    // lineEdit编辑的时候就会发送textEdited信号，进而触发ProSetPage发送completeChanged信号
    // completeChanged信号发出后会触发QWizardPage类的isComplete函数
    // isComplete函数是虚函数，我们可以加上自己的验证逻辑重新实现。
    connect(ui->lineEdit, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    connect(ui->lineEdit_2, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);

    QString path = QDir::currentPath();
    ui->lineEdit_2->setText(path);                                           // 设置默认路径
    ui->lineEdit_2->setCursorPosition(ui->lineEdit_2->text().size());        // 将光标设置到末尾
    ui->lineEdit_2->setClearButtonEnabled(true);
    ui->lineEdit->setClearButtonEnabled(true);
}

ProSetPage::~ProSetPage()
{
    delete ui;
}

void ProSetPage::GetProSettings(QString &name, QString &path)
{
    name = ui->lineEdit->text();
    path = ui->lineEdit_2->text();
}

// 由 wizard 调用决定next和finish按钮是否有效
bool ProSetPage::isComplete() const
{
    if(ui->lineEdit->text() == "" || ui->lineEdit_2->text() == "")
        return false;
    QDir dir(ui->lineEdit_2->text());
    if(!dir.exists())
    {
        ui->tips->setText("项目路径不存在");
        return false;
    }

    // 判断路径
    QString absFilePath = dir.absoluteFilePath(ui->lineEdit->text());
    QDir dist_dir(absFilePath);
    if(dist_dir.exists())
    {
        ui->tips->setText("该路径下项目已经存在");
        return false;
    }
    ui->tips->setText("");
    return QWizardPage::isComplete();
}

void ProSetPage::on_pushButton_2_clicked()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle("选择导入的文件夹");
    auto path = QDir::currentPath();
    file_dialog.setDirectory(path);
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList file_names;
    if(file_dialog.exec())
        file_names = file_dialog.selectedFiles();
    if(file_names.length() <= 0)
        return;

    QString import_path = file_names.at(0);  // 返回打开文件夹的路径

    ui->lineEdit_2->setText(import_path);
    // QFileInfo fileInfo(import_path);
    // QString fileName = fileInfo.fileName();
    // ui->lineEdit->setText(fileName);


}

