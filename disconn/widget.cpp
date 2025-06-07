#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_connBtn_clicked() {
    // 关联
    connect(ui->lineEdit, &QLineEdit::textEdited, ui->label, &QLabel::setText);

    ui->connBtn->setEnabled(false);
    ui->disconnBtn->setEnabled(true);
}

void Widget::on_disconnBtn_clicked() {
    // 断开关联
    disconnect(ui->lineEdit, &QLineEdit::textEdited, ui->label, &QLabel::setText);

    ui->connBtn->setEnabled(true);
    ui->disconnBtn->setEnabled(false);
}

void Widget::on_lineEdit_textEdited(const QString& arg1) {
}
