#include "widget.h"
#include <QAction>
#include <QMenu>
#include <QToolButton>
#include <QVBoxLayout>

Widget::Widget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    // 创建 QToolButton
    QToolButton* toolButton = new QToolButton(this);
    toolButton->setText("工具按钮");
    toolButton->setIcon(QIcon(":/icons/sample.jpg")); // 设置图标
    toolButton->setToolTip("这是一个工具按钮");
    toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 图标在上，文字在下

    // 创建菜单
    QMenu* menu = new QMenu(this);
    QAction* openAction = new QAction("打开", this);
    QAction* saveAction = new QAction("保存", this);
    menu->addAction(openAction);
    menu->addAction(saveAction);

    toolButton->setMenu(menu);                              // 设置菜单
    toolButton->setPopupMode(QToolButton::MenuButtonPopup); // 设置弹出方式（只点击右侧三角弹出）

    // 信号连接（点击菜单项）
    connect(openAction, &QAction::triggered, this, []() { qDebug("点击了：打开"); });
    connect(saveAction, &QAction::triggered, this, []() { qDebug("点击了：保存"); });

    layout->addWidget(toolButton);
}

Widget::~Widget() {
}
