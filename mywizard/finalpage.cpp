#include "finalpage.h"
#include <QVBoxLayout>

FinalPage::FinalPage(QWidget* parent) : QWizardPage(parent) {
    setTitle("完成"); // 设置该向导页的标题，显示在向导窗口顶部

    infoLabel = new QLabel; // 创建一个 QLabel 用于显示欢迎信息

    QVBoxLayout* layout = new QVBoxLayout; // 垂直布局管理器，用于安排控件位置
    layout->addWidget(infoLabel);          // 将 QLabel 添加到布局中

    setLayout(layout); // 将布局设置给该页面，使其生效
}

// 重写 QWizardPage 的 initializePage()，每次该页面显示时调用
void FinalPage::initializePage() {
    // 从向导的字段中获取用户输入的用户名，字段名为 "username"
    QString name = field("username").toString();

    // 根据用户名动态设置标签文本，显示欢迎信息
    infoLabel->setText("你好，" + name + "，欢迎使用 QWizard！");
}
