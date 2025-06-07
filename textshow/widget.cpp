#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    // 接收端是标签控件
    connect(ui->lineEdit, &QLineEdit::textEdited, ui->label, &QLabel::setText);
    // 接收端是文本浏览控件
    connect(ui->lineEdit, &QLineEdit::textEdited, ui->textBrowser, &QTextBrowser::setText);
    // 接收端是主窗口的 printText 槽
    connect(ui->lineEdit, &QLineEdit::textEdited, this, &Widget::printText);
    // 接收端是主窗口内的一个 lambda 表达式，用于打印文本
    // 如果接收端是 this，可以省略第三个参数
    connect(ui->lineEdit, &QLineEdit::textEdited,
            [](const QString& text) { qDebug() << "printText2: " << text; });
}

Widget::~Widget() {
    delete ui;
}

void Widget::printText(const QString& text) {
    // 打印到调试输出面板
    qDebug() << "printText: " << text;
}
