#include "formchangepassword.h"
#include "ui_formchangepassword.h"
#include <QDebug>
#include <QMessageBox>

// 构造函数：初始化 UI 并调用自定义初始化函数
FormChangePassword::FormChangePassword(QWidget* parent) : QWidget(parent), ui(new Ui::FormChangePassword) {
    ui->setupUi(this); // 设置 UI 元素（由 Qt Designer 生成）
    init();            // 自定义初始化函数
}

// 析构函数：释放 UI 指针
FormChangePassword::~FormChangePassword() {
    delete ui;
}

// 自定义初始化函数：设置界面样式和控件属性
void FormChangePassword::init() {
    // 设置窗口标题
    setWindowTitle(tr("修改用户密码"));

    // 设置三个密码框为“密码模式”，隐藏输入内容
    ui->lineEditOldPassword->setEchoMode(QLineEdit::Password);
    ui->lineEditNewPassword->setEchoMode(QLineEdit::Password);
    ui->lineEditNewPassword2->setEchoMode(QLineEdit::Password);

    // 用户名只读，防止篡改，并设置背景颜色
    ui->lineEditUser->setReadOnly(true);
    ui->lineEditUser->setStyleSheet("background-color: rgb(200,200,255);");

    // 为旧密码框设置提示信息
    ui->lineEditOldPassword->setToolTip(tr("旧密码验证成功才能修改为新密码。"));
}

// 接收主窗口传来的用户名和旧密码哈希，并显示用户名
void FormChangePassword::recvOldUserPassword(QString strUser, QByteArray baOldHash) {
    // 保存到成员变量中备用
    m_strUser = strUser;
    m_baOldHash = baOldHash;

    // 显示用户名到只读框
    ui->lineEditUser->setText(m_strUser);

    // 清空输入框内容，准备新输入
    ui->lineEditOldPassword->clear();
    ui->lineEditNewPassword->clear();
    ui->lineEditNewPassword2->clear();
}

// 点击“修改密码”按钮后的处理逻辑
void FormChangePassword::on_pushButtonChange_clicked() {
    // 获取三个密码框的输入内容（去除前后空格）
    QString strOldPassword = ui->lineEditOldPassword->text().trimmed();
    QString strNewPassword = ui->lineEditNewPassword->text().trimmed();
    QString strNewPassword2 = ui->lineEditNewPassword2->text().trimmed();

    // 检查密码框是否为空
    if (strOldPassword.isEmpty() || strNewPassword.isEmpty() || strNewPassword2.isEmpty()) {
        QMessageBox::information(this, tr("密码框检查"), tr("三个密码都不能为空。"));
        return;
    }

    // 检查两个新密码是否一致
    if (strNewPassword != strNewPassword2) {
        QMessageBox::information(this, tr("新密码检查"), tr("两个新密码不一致。"));
        return;
    }

    // 使用 SHA256 对旧密码计算哈希值
    QByteArray baOldHashCheck = QCryptographicHash::hash(strOldPassword.toUtf8(), QCryptographicHash::Sha256);
    baOldHashCheck = baOldHashCheck.toHex(); // 转成十六进制字符串方便比较

    // 验证旧密码是否正确
    if (baOldHashCheck != m_baOldHash) {
        QMessageBox::information(this, tr("旧密码检查"), tr("输入的旧密码不正确，不能修改密码。"));
        return;
    }

    // 若旧密码验证通过，则计算新密码的哈希值
    QByteArray baNewHash = QCryptographicHash::hash(strNewPassword.toUtf8(), QCryptographicHash::Sha256);
    baNewHash = baNewHash.toHex(); // 统一格式为 Hex 字符串

    // 发出信号，将新密码哈希和用户名传递回主窗口处理
    emit sendNewUserPassword(m_strUser, baNewHash);
}
