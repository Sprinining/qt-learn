#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // 设置树形控件只有 1 列
    ui->treeWidget->setColumnCount(1);

    // 创建 A 节点，并作为顶级节点加入树中
    QTreeWidgetItem* itemA = new QTreeWidgetItem();
    itemA->setText(0, "A");
    ui->treeWidget->addTopLevelItem(itemA);

    // 创建 B、C 节点并作为 A 的子节点
    QTreeWidgetItem* itemB = new QTreeWidgetItem();
    itemB->setText(0, "B");
    itemA->addChild(itemB);

    QTreeWidgetItem* itemC = new QTreeWidgetItem();
    itemC->setText(0, "C");
    itemA->addChild(itemC);

    // 创建 D、E 节点，直接将 B 作为父节点（自动建立父子关系）
    QTreeWidgetItem* itemD = new QTreeWidgetItem(itemB);
    itemD->setText(0, "D");

    QTreeWidgetItem* itemE = new QTreeWidgetItem(itemB);
    itemE->setText(0, "E");

    // 创建 F、G 节点，直接将 C 作为父节点
    QTreeWidgetItem* itemF = new QTreeWidgetItem(itemC);
    itemF->setText(0, "F");

    QTreeWidgetItem* itemG = new QTreeWidgetItem(itemC);
    itemG->setText(0, "G");

    // 展开所有子节点
    ui->treeWidget->expandAll();
}

Widget::~Widget() {
    delete ui;
}

// ------------------------ 先序遍历 ------------------------
void Widget::on_btnPreorder_clicked() {
    QTreeWidgetItem* itemA = ui->treeWidget->topLevelItem(0);
    qDebug() << tr("先序遍历：");
    preorderTraversal(itemA);
}

void Widget::preorderTraversal(QTreeWidgetItem* curItem) {
    if (!curItem) return;

    int nChildCount = curItem->childCount();
    qDebug() << curItem->text(0);

    for (int i = 0; i < nChildCount; ++i) {
        QTreeWidgetItem* child = curItem->child(i);
        preorderTraversal(child);
    }
}

// ------------------------ 后序遍历 ------------------------
void Widget::on_btnPostorder_clicked() {
    QTreeWidgetItem* itemA = ui->treeWidget->topLevelItem(0);
    qDebug() << tr("后序遍历：");
    postorderTraversal(itemA);
}

void Widget::postorderTraversal(QTreeWidgetItem* curItem) {
    if (!curItem) return;

    int nChildCount = curItem->childCount();

    for (int i = 0; i < nChildCount; ++i) {
        QTreeWidgetItem* child = curItem->child(i);
        postorderTraversal(child);
    }

    qDebug() << curItem->text(0);
}

// ------------------------ 中序遍历（非标准，适用于树形控件） ------------------------
void Widget::on_btnMidorder_clicked() {
    QTreeWidgetItem* itemA = ui->treeWidget->topLevelItem(0);
    qDebug() << tr("中序遍历：");
    midorderTraversal(itemA);
}

void Widget::midorderTraversal(QTreeWidgetItem* curItem) {
    if (!curItem) return;

    int nChildCount = curItem->childCount();
    if (nChildCount == 0) {
        qDebug() << curItem->text(0);
        return;
    }

    // 遍历第一个子节点（作为“左子树”）
    midorderTraversal(curItem->child(0));

    // 打印当前节点
    qDebug() << curItem->text(0);

    // 遍历剩下的子节点（作为“右子树”）
    for (int i = 1; i < nChildCount; ++i) {
        midorderTraversal(curItem->child(i));
    }
}

// ------------------------ 层序遍历 ------------------------
void Widget::on_btnLevelorder_clicked() {
    QTreeWidgetItem* itemA = ui->treeWidget->topLevelItem(0);
    qDebug() << tr("按层遍历：（没有回归的特性，使用队列实现）");
    levelorderTraversal(itemA);
}

void Widget::levelorderTraversal(QTreeWidgetItem* curItem) {
    if (!curItem) return;

    QList<QTreeWidgetItem*> queue;
    queue.append(curItem);

    while (!queue.isEmpty()) {
        QTreeWidgetItem* node = queue.takeFirst();
        qDebug() << node->text(0);

        int childCount = node->childCount();
        for (int i = 0; i < childCount; ++i) {
            queue.append(node->child(i));
        }
    }
}

// ------------------------ 迭代器遍历（同先序） ------------------------
void Widget::on_btnIterator_clicked() {
    QTreeWidgetItem* itemA = ui->treeWidget->topLevelItem(0);
    qDebug() << tr("迭代器遍历：（同先序）");
    iteratorTraversal(itemA);
}

void Widget::iteratorTraversal(QTreeWidgetItem* curItem) {
    if (!curItem) return;

    // 使用 QTreeWidgetItemIterator 进行先序遍历
    QTreeWidgetItemIterator it(curItem);
    while (*it) {
        qDebug() << (*it)->text(0);
        ++it; // 前进到下一个节点
    }
}
