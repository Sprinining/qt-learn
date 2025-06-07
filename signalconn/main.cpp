#include "showvoid.h"
#include "widget.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    Widget w;

    ShowVoid s;
    // clicked 发给 sendVoid，sendVoid 再发给 recvVoid
    QObject::connect(&w, &Widget::sendVoid, &s, &ShowVoid::recvVoid);

    w.show();
    return a.exec();
}
