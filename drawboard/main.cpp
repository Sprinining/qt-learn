#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("Qt 简易绘图板");
    w.resize(800, 600);
    w.show();
    return a.exec();
}
