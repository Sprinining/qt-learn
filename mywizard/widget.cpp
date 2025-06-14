#include "widget.h"
#include "finalpage.h" // 自定义 QWizardPage 子类：完成页
#include "ui_widget.h"
#include "userinfopage.h" // 自定义 QWizardPage 子类：用户信息页

#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWizard>
#include <QWizardPage>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this); // 加载 UI 文件中的控件和布局

    // 绑定按钮点击信号到对应槽函数
    connect(ui->btnMessageBox, &QPushButton::clicked, this, &Widget::showMessageBox);
    connect(ui->btnWizard, &QPushButton::clicked, this, &Widget::showWizard);
}

Widget::~Widget() {
    delete ui; // 释放 UI 指针，防止内存泄漏
}

// 演示 QMessageBox 的用法
void Widget::showMessageBox() {
    QMessageBox msgBox;

    msgBox.setWindowTitle("QMessageBox 示例");     // 设置对话框标题
    msgBox.setText("是否确认继续操作？");          // 主文本
    msgBox.setInformativeText("此操作不可撤销！"); // 补充信息文本
    msgBox.setIcon(QMessageBox::Warning);          // 设置警告图标

    // 设置标准按钮：Yes 和 No
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    msgBox.setDefaultButton(QMessageBox::No); // 默认选中 No 按钮

    int ret = msgBox.exec(); // 以模态方式显示消息框，阻塞直到用户选择

    // 根据用户选择显示不同的信息提示
    if (ret == QMessageBox::Yes) {
        QMessageBox::information(this, "结果", "你选择了是");
    } else {
        QMessageBox::information(this, "结果", "你选择了否");
    }
}

// 演示 QWizard 的用法
void Widget::showWizard() {
    QWizard wizard;
    wizard.setWindowTitle("QWizard 示例");       // 设置向导窗口标题
    wizard.setWizardStyle(QWizard::ModernStyle); // 设置向导风格（现代）

    // 添加页面：必须是 QWizardPage 的子类实例
    wizard.addPage(new UserInfoPage); // 用户信息输入页
    wizard.addPage(new FinalPage);    // 完成提示页

    wizard.exec(); // 以模态方式启动向导，用户完成或取消时返回
}
