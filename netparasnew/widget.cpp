#include "widget.h"
#include "./ui_widget.h"
#include <QIntValidator>               // 整数输入验证器
#include <QRegularExpression>          // 正则表达式类（Qt 6 推荐）
#include <QRegularExpressionValidator> // 正则表达式验证器类

// 构造函数：初始化界面并设置输入验证规则
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this); // 设置 UI 界面

    // 设置 MAC 地址输入掩码：HH:HH:HH:HH:HH:HH（只允许输入十六进制字符）
    ui->macEdt->setInputMask("HH:HH:HH:HH:HH:HH");

    // 定义 IP 地址的正则表达式（匹配 IPv4 格式，例如 192.168.1.1）
    QRegularExpression re("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
                          "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");

    // 创建 IP 验证器，并设置给 ipEdt
    QRegularExpressionValidator* reVali = new QRegularExpressionValidator(re);
    ui->ipEdt->setValidator(reVali);

    // 创建端口号验证器，限制输入范围在 0 到 65535
    QIntValidator* intVali = new QIntValidator(0, 65535);
    ui->portEdt->setValidator(intVali);

    setLayout(ui->verticalLayout);
}

// 析构函数：释放 UI 对象
Widget::~Widget() {
    delete ui;
}

// 槽函数：当 IP 输入框内容发生变化时打印日志
void Widget::on_ipEdt_textChanged(const QString& arg1) {
    qDebug() << "IP: " << arg1;
}

// 槽函数：当端口号输入框内容发生变化时打印日志
void Widget::on_portEdt_textChanged(const QString& arg1) {
    qDebug() << "Port: " << arg1;
}

// 槽函数：当 MAC 地址输入框内容发生变化时打印日志
void Widget::on_macEdt_textChanged(const QString& arg1) {
    qDebug() << "MAC: " << arg1;
}
