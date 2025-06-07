#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QMessageBox>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_btnAbout_clicked() {
    QMessageBox::about(this, tr("关于本程序"), tr("启动画面演示程序，版本 1.0，\r\n使用外挂资源 bigpics.rcc。"));
}

void Widget::on_btnAboutQt_clicked() {
    QMessageBox::aboutQt(this, tr("关于 Qt"));
}
