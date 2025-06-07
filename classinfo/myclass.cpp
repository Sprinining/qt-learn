#include "myclass.h"

// 构造函数，初始化 m_options 为0（无选项）
MyClass::MyClass(QObject* parent) : QObject(parent), m_options(0) {
}

// 设置选项组合
void MyClass::setOptions(Options opts) {
    m_options = opts;
}

// 获取当前选项组合
MyClass::Options MyClass::options() const {
    return m_options;
}
