#include "widget.h"
#include "./ui_widget.h"
#include <QMessageBox>

// 构造函数，初始化界面和控件设置
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this); // 加载 UI 设计器生成的界面

    // 设置日期编辑器可以弹出日历控件，方便用户选择日期
    ui->dateEdit->setCalendarPopup(true);

    // 设置表格控件的行为
    // 每次选中整行
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置只能单选，方便后续删除功能的使用
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // 启用自动排序（会根据列头点击进行排序）
    ui->tableWidget->setSortingEnabled(true);
    // 默认按第 0 列升序排序
    ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);

    // 让表格的最后一列自动拉伸填满剩余空间
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    // 设置行高，让图标更大一些看起来更美观
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(36);
    // 设置图标大小
    ui->tableWidget->setIconSize(QSize(32, 32));

    // 修改主窗口宽度，便于展示表格拉伸效果
    QSize sz = this->size();
    sz.setWidth(640);
    this->resize(sz);
}

// 析构函数，释放 UI 内存
Widget::~Widget() {
    delete ui;
}

// 当前选中项发生变化时触发
void Widget::on_tableWidget_currentItemChanged(QTableWidgetItem* current, QTableWidgetItem* previous) {
    // 打印当前条目的行、列、文本内容
    if (current != nullptr) qDebug() << tr("当前条目行号：%1，列号：%2，文本：%3").arg(current->row()).arg(current->column()).arg(current->text());
}

// 添加按钮点击事件
void Widget::on_btnAdd_clicked() {
    QString strName = ui->lineEditName->text().trimmed(); // 获取姓名并去掉前后空格
    if (strName.isEmpty()) {
        QMessageBox::warning(this, tr("添加行"), tr("姓名不能为空！"));
        return;
    }

    // 添加数据时必须先关闭排序，否则新行可能会被移动，导致 setItem 设置错误
    ui->tableWidget->setSortingEnabled(false);

    // 获取当前行数，准备插入新行
    int nOldRowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(nOldRowCount); // 在末尾插入一行

    // 头像图标（从下拉框当前项获取）
    QIcon iconHead = ui->comboBoxPhotos->itemIcon(ui->comboBoxPhotos->currentIndex());
    // 创建带图标和文本的姓名项
    QTableWidgetItem* itemName = new QTableWidgetItem(iconHead, strName);
    ui->tableWidget->setItem(nOldRowCount, 0, itemName);

    // 性别
    QString strGender = ui->lineEditGender->text();
    QTableWidgetItem* itemGender = new QTableWidgetItem(strGender);
    ui->tableWidget->setItem(nOldRowCount, 1, itemGender);

    // 生日（格式化为 yyyy/MM/dd）
    QString strBirthday = ui->dateEdit->date().toString("yyyy/MM/dd");
    QTableWidgetItem* itemBirthday = new QTableWidgetItem(strBirthday);
    ui->tableWidget->setItem(nOldRowCount, 2, itemBirthday);

    // 婚否（复选框形式）
    Qt::CheckState cks = ui->checkBoxIsMarried->checkState();
    QTableWidgetItem* itemIsMarried = new QTableWidgetItem();
    itemIsMarried->setCheckState(cks); // 设置复选框状态
    ui->tableWidget->setItem(nOldRowCount, 3, itemIsMarried);

    // 地址
    QString strAddress = ui->lineEditAddress->text();
    QTableWidgetItem* itemAddress = new QTableWidgetItem(strAddress);
    ui->tableWidget->setItem(nOldRowCount, 4, itemAddress);

    // 添加完毕后重新开启排序
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);

    // 设置当前选中的单元格为刚插入的 itemName 并滚动到它
    ui->tableWidget->setCurrentItem(itemName);
    ui->tableWidget->scrollToItem(itemName);
}

// 删除按钮点击事件
void Widget::on_btnDel_clicked() {
    // 获取当前选中的单元格
    QTableWidgetItem* curItem = ui->tableWidget->currentItem();
    if (curItem == nullptr) return; // 没有选中时直接返回

    // 如果当前单元格被选中，删除其所在行
    if (curItem->isSelected()) ui->tableWidget->removeRow(curItem->row());
}
