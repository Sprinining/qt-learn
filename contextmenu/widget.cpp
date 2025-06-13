#include "widget.h"
#include "./ui_widget.h"
#include <QMessageBox>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    createMenu(); // 创建右键菜单
}

Widget::~Widget() {
    delete ui;
}

// 创建并初始化右键菜单
void Widget::createMenu() {
    // 创建菜单对象（菜单标题文本不会显示，仅用于调试可读性）
    m_menuContext = new QMenu(tr("ContextMenu"), this);

    // 创建“添加条目”菜单项
    QAction* actAdd = new QAction(tr("添加条目"), this);
    m_menuContext->addAction(actAdd);

    // 创建“编辑条目”菜单项
    QAction* actEdit = new QAction(tr("编辑条目"), this);
    m_menuContext->addAction(actEdit);

    // 创建“删除条目”菜单项
    QAction* actDel = new QAction(tr("删除条目"), this);
    m_menuContext->addAction(actDel);

    // 创建“清空所有”菜单项
    QAction* actClearAll = new QAction(tr("清空所有"), this);
    m_menuContext->addAction(actClearAll);

    // 启用列表控件的自定义右键菜单功能
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // 将“请求右键菜单”的信号连接到自定义槽函数
    connect(ui->listWidget, &QListWidget::customContextMenuRequested, this, &Widget::onCustomContextMenuRequested);

    // 为每个菜单项连接点击槽函数
    connect(actAdd, &QAction::triggered, this, &Widget::onAddItemTriggered);
    connect(actEdit, &QAction::triggered, this, &Widget::onEditItemTriggered);
    connect(actDel, &QAction::triggered, this, &Widget::onDelItemTriggered);
    connect(actClearAll, &QAction::triggered, this, &Widget::onClearAllTriggered);
}

// 槽函数：处理右键菜单请求
void Widget::onCustomContextMenuRequested(const QPoint& pos) {
    // 将控件内相对坐标转换为屏幕全局坐标
    QPoint screenPos = ui->listWidget->mapToGlobal(pos);
    // 显示右键菜单（阻塞式，用户点了才返回）
    QAction* actRet = m_menuContext->exec(screenPos);
    if (actRet != nullptr) {
        // 菜单项非空，打印被点击的菜单项文本（调试用）
        qDebug() << tr("返回的菜单项：") + actRet->text();
    }
}

// 槽函数：添加条目
void Widget::onAddItemTriggered() {
    // 创建新条目
    QListWidgetItem* itemNew = new QListWidgetItem(tr("新建条目"));
    // 设置为可编辑条目
    itemNew->setFlags(itemNew->flags() | Qt::ItemIsEditable);
    // 加入列表控件
    ui->listWidget->addItem(itemNew);
    // 设置当前项为新条目
    ui->listWidget->setCurrentItem(itemNew);
    // 弹出编辑框以立即修改文本
    ui->listWidget->editItem(itemNew);
}

// 槽函数：编辑当前选中条目
void Widget::onEditItemTriggered() {
    // 获取当前选中项
    QListWidgetItem* curItem = ui->listWidget->currentItem();
    if (curItem == nullptr) {
        // 没有选中项，打印提示并返回
        qDebug() << tr("没有选中的条目。");
        return;
    }
    // 设置为可编辑状态
    curItem->setFlags(curItem->flags() | Qt::ItemIsEditable);
    // 弹出编辑框
    ui->listWidget->editItem(curItem);
}

// 槽函数：删除当前选中条目
void Widget::onDelItemTriggered() {
    QListWidgetItem* curItem = ui->listWidget->currentItem();
    if (curItem == nullptr) {
        qDebug() << tr("没有选中的条目。");
        return;
    }
    // 删除选中项
    delete curItem;
    curItem = nullptr;
}

// 槽函数：清空所有条目（带确认）
void Widget::onClearAllTriggered() {
    int nCount = ui->listWidget->count();
    if (nCount < 1) return; // 没有内容就不处理

    // 弹出确认对话框
    int buttonRet = QMessageBox::question(this, tr("清空所有"), tr("请确认是否清空所有条目？"));
    if (buttonRet == QMessageBox::Yes) {
        ui->listWidget->clear(); // 确认后清空
    } else {
        return; // 取消不处理
    }
}
