#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
}

Widget::~Widget() {
    delete ui;
}

const QString& Widget::nickName() const {
    return m_nickName;
}

int Widget::count() const {
    return m_count;
}

void Widget::setNickName(const QString& strNewName) {
    if (strNewName == m_nickName) return;
    m_nickName = strNewName;
    emit nickNameChanged(strNewName);
}

void Widget::setCount(int nNewCount) {
    if (nNewCount == m_count) return;
    m_count = nNewCount;
    emit countChanged(nNewCount);
}
