#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QMessageBox>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this); // 初始化界面
}

Widget::~Widget() {
    delete ui; // 释放 UI 内存
}

// 添加顶层节点（省市）
void Widget::on_btnAddTop_clicked() {
    // 获取省市名称、经度、纬度三个文本框内容
    QString strName = ui->lineEditName->text();
    QString strLon = ui->lineEditLon->text();
    QString strLat = ui->lineEditLat->text();

    // 检查是否全部输入
    if (strName.isEmpty() || strLon.isEmpty() || strLat.isEmpty()) {
        QMessageBox::information(this, tr("输入检查"), tr("三个编辑框均需要输入信息！"));
        return;
    }

    // 创建一个新的树形节点并设置三列文本
    QTreeWidgetItem* itemNew = new QTreeWidgetItem();
    itemNew->setText(0, strName);
    itemNew->setText(1, strLon);
    itemNew->setText(2, strLat);

    // 添加到树的顶层（根节点）
    ui->treeWidget->addTopLevelItem(itemNew);
    ui->treeWidget->setFocus(); // 设置焦点，方便键盘操作
}

// 添加子节点（市县）
void Widget::on_btnAddChild_clicked() {
    // 获取当前选中的节点
    QTreeWidgetItem* curItem = ui->treeWidget->currentItem();
    if (curItem == nullptr) {
        QMessageBox::information(this, tr("无选中节点"), tr("请先选中一个节点，然后为其添加子节点！"));
        return;
    }

    // 获取输入框内容
    QString strName = ui->lineEditName->text();
    QString strLon = ui->lineEditLon->text();
    QString strLat = ui->lineEditLat->text();

    // 检查是否全部输入
    if (strName.isEmpty() || strLon.isEmpty() || strLat.isEmpty()) {
        QMessageBox::information(this, tr("输入检查"), tr("三个编辑框均需要输入信息！"));
        return;
    }

    // 创建新的子节点
    QTreeWidgetItem* itemChild = new QTreeWidgetItem();
    itemChild->setText(0, strName);
    itemChild->setText(1, strLon);
    itemChild->setText(2, strLat);

    // 添加为当前选中节点的子节点
    curItem->addChild(itemChild);
    ui->treeWidget->expandItem(curItem); // 展开当前节点以显示子项
    ui->treeWidget->setFocus();          // 设置焦点
}

// 删除选中的叶子节点（无子节点）
void Widget::on_btnDelLeaf_clicked() {
    // 获取当前选中的节点
    QTreeWidgetItem* curItem = ui->treeWidget->currentItem();
    if (curItem == nullptr) {
        QMessageBox::warning(this, tr("未选中节点"), tr("未选中节点，没东西删除。"));
        return;
    }

    // 判断是否是叶子节点（无子节点）
    if (curItem->childCount() > 0) {
        QMessageBox::warning(this, tr("不是叶子节点"), tr("不是叶子节点，不能删除！"));
        return;
    }

    // 是叶子节点，直接 delete，QTreeWidget 会自动移除显示
    delete curItem;
    curItem = nullptr;
}

// 删除选中节点及其整个子树
void Widget::on_btnDelSubtree_clicked() {
    // 获取当前选中节点
    QTreeWidgetItem* curItem = ui->treeWidget->currentItem();
    if (curItem == nullptr) {
        QMessageBox::warning(this, tr("未选中节点"), tr("未选中节点，没东西删除。"));
        return;
    }

    // 递归删除以当前节点为根的整棵子树
    removeSubtree(curItem);
}

// 递归删除子树的函数
void Widget::removeSubtree(QTreeWidgetItem* curLevelItem) {
    if (curLevelItem == nullptr) return;

    int nCount = curLevelItem->childCount(); // 获取子节点数量

    // 若没有子节点，说明是叶子节点，直接删除
    if (nCount < 1) {
        delete curLevelItem; // 删除该节点
        curLevelItem = nullptr;
        return;
    }

    // 注意：删除时不能直接用 curLevelItem->child(i)，因为删除会导致索引混乱
    while (curLevelItem->childCount() > 0) {
        QTreeWidgetItem* curChild = curLevelItem->takeChild(0); // 每次取第一个子项
        removeSubtree(curChild);                                // 递归删除该子项及其后代
    }

    // 子节点都删除完毕后，删除当前节点
    delete curLevelItem;
    curLevelItem = nullptr;
}
