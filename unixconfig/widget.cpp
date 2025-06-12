#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QIntValidator>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

// 构造函数：初始化 UI 和输入校验
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // 设置 IP 地址输入框的正则校验器，确保格式合法
    QRegularExpression re("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
                          "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    QRegularExpressionValidator* reVail = new QRegularExpressionValidator(re);
    ui->lineEditIP->setValidator(reVail);

    // 设置端口号输入框的整数校验器，范围 0-65535
    QIntValidator* intVali = new QIntValidator(0, 65535);
    ui->lineEditPort->setValidator(intVali);
}

// 析构函数：释放 UI 内存
Widget::~Widget() {
    delete ui;
}

// 浏览选择源配置文件按钮点击事件
void Widget::on_btnBrowseSrc_clicked() {
    QString strSrcName = QFileDialog::getOpenFileName(this, tr("打开配置文件"), tr("."), tr("Text files(*.txt);;All files(*)"));
    if (strSrcName.isEmpty()) return;         // 用户取消选择
    ui->lineEditSrcFile->setText(strSrcName); // 显示到 UI 上
}

// 加载配置文件按钮点击事件
void Widget::on_btnLoad_clicked() {
    QString strSrc = ui->lineEditSrcFile->text();
    if (strSrc.isEmpty()) return;

    QFile fileIn(strSrc);
    if (!fileIn.open(QIODevice::ReadOnly)) {
        // 打开失败，弹出提示框显示错误信息
        QMessageBox::warning(this, tr("打开错误"), tr("打开文件错误：") + fileIn.errorString());
        return;
    }

    // 按行读取文件内容
    while (!fileIn.atEnd()) {
        QByteArray baLine = fileIn.readLine().trimmed();
        if (baLine.startsWith('#')) continue; // 跳过注释行
        AnalyzeOneLine(baLine);               // 解析每一行配置
    }

    QMessageBox::information(this, tr("加载配置"), tr("加载配置项完毕！"));
}

// 浏览保存目标文件按钮点击事件
void Widget::on_btnBrowseDst_clicked() {
    QString strDstName = QFileDialog::getSaveFileName(this, tr("保存配置文件"), tr("."), tr("Text files(*.txt);;All files(*)"));
    if (strDstName.isEmpty()) return;
    ui->lineEditDstFile->setText(strDstName); // 显示保存路径
}

// 保存配置按钮点击事件
void Widget::on_btnSave_clicked() {
    QString strSaveName = ui->lineEditDstFile->text();
    QString strIP = ui->lineEditIP->text();
    QString strPort = ui->lineEditPort->text();
    QString strHostName = ui->lineEditHostName->text();
    QString strWorkGroup = ui->lineEditWorkGroup->text();

    // 检查所有项是否填写完整
    if (strSaveName.isEmpty() || strIP.isEmpty() || strPort.isEmpty() || strHostName.isEmpty() || strWorkGroup.isEmpty()) {
        QMessageBox::warning(this, tr("保存配置"), tr("需要设置好保存文件名和所有配置项数值。"));
        return;
    }

    QFile fileOut(strSaveName);
    // 打开输出文件（覆盖模式）
    if (!fileOut.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QMessageBox::warning(this, tr("打开文件"), tr("打开目的文件失败：") + fileOut.errorString());
        return;
    }

    // 写入各项配置到文件
    QByteArray baTemp = "ip = ";
    baTemp += strIP.toLocal8Bit() + "\n";
    fileOut.write(baTemp);

    baTemp = "port = ";
    baTemp += strPort.toLocal8Bit() + "\n";
    fileOut.write(baTemp);

    baTemp = "hostname = ";
    baTemp += strHostName.toLocal8Bit() + "\n";
    fileOut.write(baTemp);

    baTemp = "workgroup = ";
    baTemp += strWorkGroup.toLocal8Bit() + "\n";
    fileOut.write(baTemp);

    QMessageBox::information(this, tr("保存配置"), tr("保存配置项成功！"));
}

// 解析一行配置的函数（key=value 格式）
void Widget::AnalyzeOneLine(const QByteArray& baLine) {
    QList<QByteArray> list = baLine.split('=');
    if (list.count() < 2) return; // 非法行

    QByteArray optionName = list[0].trimmed().toLower();    // 统一为小写
    QByteArray optionValue = list[1].trimmed();             // 去除空白
    QString strValue = QString::fromLocal8Bit(optionValue); // 转为 QString

    // 根据字段名设置 UI 中对应输入框
    if ("ip" == optionName) {
        ui->lineEditIP->setText(strValue);
        return;
    }
    if ("port" == optionName) {
        ui->lineEditPort->setText(strValue);
        return;
    }
    if ("hostname" == optionName) {
        ui->lineEditHostName->setText(strValue);
        return;
    }
    if ("workgroup" == optionName) {
        ui->lineEditWorkGroup->setText(strValue);
        return;
    }
}
