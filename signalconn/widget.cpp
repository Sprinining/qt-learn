#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    // 第四个参数是信号，直接关联到自定义的信号，而不需要槽函数中转
    connect(ui->pushButton, &QPushButton::clicked, this, &Widget::sendVoid);
}

Widget::~Widget() {
    delete ui;
}
