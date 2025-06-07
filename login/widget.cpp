#include "widget.h"
#include "./ui_widget.h"      // 自动生成的 UI 头文件，包含 ui 设计的控件定义
#include <QCryptographicHash> // 提供哈希算法支持（如 SHA3-256）
#include <QMessageBox>        // 用于弹出提示信息框

// 构造函数：初始化 UI 界面
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);                            // 设置 UI，连接 .ui 文件中的控件
    ui->pwdEdt->setEchoMode(QLineEdit::Password); // 设置密码输入框为“密码模式”，隐藏输入内容
}

// 析构函数：释放 UI 占用的资源
Widget::~Widget() {
    delete ui;
}

// 登录按钮点击槽函数
void Widget::on_loginBtn_clicked() {
    // 判断用户名或密码是否为空
    if (ui->userEdt->text().isEmpty() || ui->pwdEdt->text().isEmpty()) {
        // 弹出警告框提示
        QMessageBox::warning(this, tr("警告信息"), tr("用户名或密码为空，不能登录。"));
        return;
    }

    // 获取输入的用户名
    name = ui->userEdt->text();

    // 获取密码并进行 SHA3-256 哈希处理
    pwdHash = QCryptographicHash::hash(ui->pwdEdt->text().toUtf8(), QCryptographicHash::Sha3_256);

    // 构造用户信息字符串，使用 \r\n 实现换行（适用于 Windows）
    QString msg = tr("用户名：") + name + tr("\r\n") + tr("密码Hash：");
    msg.append(pwdHash.toHex()); // 转换为十六进制字符串显示

    qDebug() << msg;                                     // 在控制台输出调试信息
    QMessageBox::information(this, tr("用户信息"), msg); // 弹出信息框显示用户名和密码 Hash
}

// 退出按钮点击槽函数
void Widget::on_exitBtn_clicked() {
    this->close(); // 关闭主窗口
}
