#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QTextStream>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this); // 设置 UI 布局
}

Widget::~Widget() {
    delete ui;
}

// “输入”按钮点击事件处理函数
void Widget::on_btnIn_clicked() {
    // 创建输入输出流对象（绑定到标准输入和输出）
    QTextStream tsIn(stdin);   // 标准输入，等同于 std::cin
    QTextStream tsOut(stdout); // 标准输出，等同于 std::cout

    // 提示用户在命令行输入一行字符串
    QString strOut = tr("请输入一行字符串：");
    tsOut << strOut << Qt::endl;

    // 从命令行读取一整行用户输入（遇到回车结束）
    QString strMsg = tsIn.readLine();

    // 将用户输入显示到界面上的 QLineEdit 控件中
    ui->lineEditMsg->setText(strMsg);
}

// “输出”按钮点击事件处理函数
void Widget::on_btnOut_clicked() {
    // 创建输出流对象，用于写入标准输出
    QTextStream tsOut(stdout); // 标准输出

    // 从界面上的 QLineEdit 中获取用户输入的字符串
    QString strMsg = ui->lineEditMsg->text();

    // 输出一行信息到命令行
    tsOut << Qt::endl << tr("输出信息：") << strMsg << Qt::endl;

    // 使用 Qt 的调试输出（会自动输出到调试终端）
    qDebug() << Qt::endl << tr("这行是调试信息。") << Qt::endl;
}
