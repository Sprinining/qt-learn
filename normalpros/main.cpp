#include "showchanges.h"
#include "widget.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    Widget w;

    ShowChanges s;
    QObject::connect(&w, &Widget::valueChanged, &s, &ShowChanges::recvValue);
    QObject::connect(&w, &Widget::nickNameChanged, &s, &ShowChanges::recvNickName);
    QObject::connect(&w, &Widget::countChanged, &s, &ShowChanges::recvCount);

    w.setNickName("Wid");
    qDebug() << w.nickName();
    w.setCount(7);
    qDebug() << w.count();

    w.setProperty("value", 3.14);
    qDebug() << w.property("value").toDouble();

    w.show();
    return a.exec();
}
