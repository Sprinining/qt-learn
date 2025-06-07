#include "widget.h"
#include "./ui_widget.h"
#include <QMessageBox>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    // 三个按钮都关联到同一个槽函数，省略第三个参数 this
    connect(ui->fbiBtn, &QPushButton::clicked, this, &Widget::someoneComing);
    connect(ui->ciaBtn, &QPushButton::clicked, this, &Widget::someoneComing);
    connect(ui->deaBtn, &QPushButton::clicked, this, &Widget::someoneComing);
}

Widget::~Widget() {
    delete ui;
}

void Widget::someoneComing(){
    QString name = this->sender()->objectName();
    qDebug() << name;

    QString msg = "我去！是";
    if("fbiBtn" == name){
        msg.append("FBI！");
    }else if("ciaBtn" == name){
        msg.append("CIA！");
    }else if("deaBtn" == name){
        msg.append("DEA！");
    }else{
        return;
    }

    QMessageBox::information(this, tr("有人敲门"), msg);
}
