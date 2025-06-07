#include "showmsg.h"
#include "widget.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    Widget w;

    ShowMsg s;
    // 关联，信号里的字符串参数会自动传递给槽函数
    // 在 main 函数里，需要手动加 QObject:: 前缀来调用 connect 函数
    QObject::connect(&w, &Widget::sendMsg, &s, &ShowMsg::recvMsg);

    w.show();
    return a.exec();
}
