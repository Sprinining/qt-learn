#include "widget.h"
#include "./ui_widget.h"
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>

// 构造函数，初始化界面并设置列表项的属性
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    int nCount = ui->listWidget->count();
    for (int i = 0; i < nCount; i++) {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setFlags((item->flags()) | Qt::ItemIsEditable); // 设置可编辑
        item->setCheckState(Qt::Unchecked);                   // 初始化为未选中状态
    }
}

// 析构函数，释放资源
Widget::~Widget() {
    delete ui;
}

// 当当前选中项改变时，更新右侧文本框内容为该项的 ToolTip
void Widget::on_listWidget_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous) {
    if (current == nullptr) return;
    QString strToolTip = current->toolTip();
    ui->lineEditToolTip->setText(strToolTip);
}

// 当列表项状态改变时触发（例如勾选状态变化）
void Widget::on_listWidget_itemChanged(QListWidgetItem* item) {
    if (item == nullptr) return;
    if (item->checkState() != Qt::Checked) return;

    // 统计已勾选的项数
    int nCount = ui->listWidget->count();
    int nUsingItemsCount = 0;
    for (int i = 0; i < nCount; ++i) {
        QListWidgetItem* theItem = ui->listWidget->item(i);
        if (theItem->checkState() == Qt::Checked) nUsingItemsCount++;
    }

    // 超过 6 个时弹出警告框并撤销本次勾选
    if (nUsingItemsCount > 6) {
        QMessageBox::warning(this, tr("携带数目检查"), tr("DOTA 装备最多带 6 个！"));
        item->setCheckState(Qt::Unchecked);
    }
}

// 添加新装备按钮：选择图标文件，创建带图标的新项，并进入编辑模式
void Widget::on_btnAdd_clicked() {
    QString strItemFileName = QFileDialog::getOpenFileName(this, tr("选择装备图标文件"), tr("."), tr("Image files(*.jpg *.png *.bmp);;All files(*)"));

    if (strItemFileName.isEmpty()) return;

    QIcon iconNew(strItemFileName);
    QListWidgetItem* itemNew = new QListWidgetItem(iconNew, tr("新装备名称"), ui->listWidget);
    itemNew->setFlags((itemNew->flags()) | Qt::ItemIsEditable);
    itemNew->setCheckState(Qt::Unchecked);
    ui->listWidget->setCurrentItem(itemNew);
    ui->listWidget->editItem(itemNew);
}

// 删除当前选中项
void Widget::on_btnDel_clicked() {
    int nCurRow = ui->listWidget->currentRow();
    if (nCurRow < 0) return;
    QListWidgetItem* itemDel = ui->listWidget->takeItem(nCurRow); // 从列表中移除
    delete itemDel;                                               // 删除对象
    itemDel = nullptr;
}

// 切换视图模式：列表模式 <-> 图标模式
void Widget::on_btnViewMode_clicked() {
    QListView::ViewMode vm = ui->listWidget->viewMode();
    if (QListView::ListMode == vm) {
        ui->listWidget->setViewMode(QListView::IconMode);
    } else {
        ui->listWidget->setViewMode(QListView::ListMode);
    }
}

// 加载保存的 items 文件
void Widget::on_btnLoad_clicked() {
    QString strOpenName = QFileDialog::getOpenFileName(this, tr("打开items文件"), tr("."), tr("Items files(*.items)"));
    if (strOpenName.isEmpty()) return;

    QFile fileOpen(strOpenName);
    if (!fileOpen.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("打开文件"), tr("打开指定文件失败，请检查文件是否存在和读取权限！"));
        return;
    }

    QDataStream dsIn(&fileOpen);
    qint32 nCount;
    dsIn >> nCount;

    if (nCount <= 0) {
        QMessageBox::warning(this, tr("文件加载"), tr("文件中无条目数据可以加载！"));
        return;
    }

    for (qint32 i = 0; i < nCount; i++) {
        QListWidgetItem* theItem = new QListWidgetItem(ui->listWidget);
        dsIn >> *theItem;
        theItem->setFlags((theItem->flags()) | Qt::ItemIsEditable);
    }
}

// 保存当前列表到 items 文件
void Widget::on_btnSave_clicked() {
    QString strSaveName = QFileDialog::getSaveFileName(this, tr("保存items文件"), tr("."), tr("Items files(*.items)"));
    if (strSaveName.isEmpty()) return;

    QFile fileSave(strSaveName);
    if (!fileSave.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("打开写入文件"), tr("打开要写入的文件失败，请检查文件名和是否具有写入权限！"));
        return;
    }

    QDataStream dsOut(&fileSave);
    qint32 nCount = ui->listWidget->count();
    dsOut << nCount;

    for (qint32 i = 0; i < nCount; i++) {
        QListWidgetItem* theItem = ui->listWidget->item(i);
        dsOut << *theItem;
    }
}

// 编辑当前选中项的 ToolTip（提示信息）
void Widget::on_btnEditToolTip_clicked() {
    QString strNew = ui->lineEditToolTip->text();
    QListWidgetItem* curItem = ui->listWidget->currentItem();
    if (curItem != nullptr) curItem->setToolTip(strNew);
}
