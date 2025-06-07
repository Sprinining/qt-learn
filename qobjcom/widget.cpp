#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    // 关联
    connect(ui->pushButton, &QPushButton::clicked, this, &Widget::buttonClicked);
}

Widget::~Widget() {
    delete ui;
}

void Widget::buttonClicked(){
    // 用 emit 发信号
    emit sendMsg(tr("这是发送的信息"));
}
