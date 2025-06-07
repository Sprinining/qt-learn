#include "myclass.h"
#include <QCoreApplication>
#include <QDebug>
#include <QMetaEnum>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    MyClass obj;

    // 使用按位或组合多个选项，设置 OptionA 和 OptionC
    obj.setOptions(MyClass::OptionA | MyClass::OptionC);

    // 直接打印枚举组合，会显示字符串形式（依赖 Q_FLAGS 宏）
    qDebug() << "Options set:" << obj.options();

    // 通过元对象系统获取枚举信息
    int index = MyClass::staticMetaObject.indexOfEnumerator("Option");
    QMetaEnum metaEnum = MyClass::staticMetaObject.enumerator(index);

    // 把枚举值（包含组合位）转换成字符串，多个选项用 '|' 连接
    QString flagsString = metaEnum.valueToKeys(obj.options());
    qDebug() << "Flags as string via QMetaEnum:" << flagsString;

    return 0;
}
