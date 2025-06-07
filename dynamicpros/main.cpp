#include "widget.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    Widget w;

    // 检查对象中是否已有名为 "myValue" 的属性（无论是 Q_PROPERTY 还是动态属性）
    // 如果没有，返回 QVariant::Invalid
    qDebug() << w.property("myValue").isValid(); // 输出 false（尚未设置）

    // 设置一个名为 "myValue" 的动态属性（未通过 Q_PROPERTY 声明）
    // 实际上在内部以 QVariant 形式存在，和成员变量无关
    w.setProperty("myValue", 3.14);

    // 读取刚设置的动态属性值，转换为 double
    qDebug() << w.property("myValue").toDouble(); // 输出 3.14

    // 同样设置另一个动态属性 "myName"
    w.setProperty("myName", "Wid");
    qDebug() << w.property("myName").toString(); // 输出 "Wid"

    // 查看当前对象上的所有动态属性名（即未通过 Q_PROPERTY 声明、setProperty 设置的）
    // 输出格式是 QStringList，例如：("myValue", "myName")
    qDebug() << w.dynamicPropertyNames();

    w.show();
    return a.exec();
}
