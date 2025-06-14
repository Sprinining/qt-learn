#include "userinfopage.h"
#include <QLabel>
#include <QVBoxLayout>

UserInfoPage::UserInfoPage(QWidget *parent) : QWizardPage(parent) {
    setTitle("用户信息");                      // 设置该向导页的标题，显示在向导窗口顶部

    QLabel *label = new QLabel("请输入你的名字：");  // 提示用户输入的文本标签
    nameEdit = new QLineEdit;                 // 创建文本编辑框供用户输入名字

    // 注册字段，允许通过 QWizard 的 field() 方法访问输入内容
    // 字段名为 "username*"，'*' 表示该字段是必填项，向导会强制要求用户填写
    registerField("username*", nameEdit);

    // 创建垂直布局，将标签和文本框垂直排列
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(nameEdit);

    // 设置页面布局，使控件显示在窗口中
    setLayout(layout);
}
