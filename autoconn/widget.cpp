#include "widget.h"
#include "./ui_widget.h"
#include <QMessageBox>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_hungryBtn_clicked() {
    QMessageBox::information(this, tr("xixi"), tr("hehe"));
}
