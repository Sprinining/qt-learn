#ifndef MYCLASS_H
#define MYCLASS_H

#include <QObject>

// MyClass 继承自 QObject，支持 Qt 元对象系统
class MyClass : public QObject {
    Q_OBJECT

public:
    // 定义枚举 Option，表示不同选项，使用位掩码方便组合
    enum Option {
        OptionA = 0x01,
        OptionB = 0x02,
        OptionC = 0x04,
    };

    // 定义 Flags 类型，允许多个 Option 位组合在一起
    Q_DECLARE_FLAGS(Options, Option)

    // 将 Flags 类型注册到元对象系统，支持反射和信号槽等特性
    Q_FLAGS(Options)

    explicit MyClass(QObject* parent = nullptr);

    // 设置选项，接收多个枚举值的组合
    void setOptions(Options opts);

    // 获取当前选项组合
    Options options() const;

private:
    Options m_options; // 保存当前选项组合
};

// 启用按位操作符（|、&、^ 等）支持 Options 类型
Q_DECLARE_OPERATORS_FOR_FLAGS(MyClass::Options)

#endif // MYCLASS_H
