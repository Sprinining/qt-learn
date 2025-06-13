#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // 设置表格为 4 行 4 列
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(4);

    // 设置表格列宽自动拉伸，所有列均匀分配宽度
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 初始化表格内容
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // 新建条目，并设置内容为 “tableItem i, j”
            QTableWidgetItem* itemNew = new QTableWidgetItem();
            itemNew->setText(tr("tableItem %1, %2").arg(i).arg(j));
            ui->tableWidget->setItem(i, j, itemNew); // 插入到表格中
        }
    }
}

Widget::~Widget() {
    delete ui;
}

// 槽函数：启用交替行颜色显示
void Widget::on_pushButtonAlternatingRowColors_clicked() {
    // 启用交替行颜色功能
    ui->tableWidget->setAlternatingRowColors(true);

    // 设置交替行的背景颜色为天蓝色，网格线颜色为深绿色
    QString strStyle = " QTableWidget{ alternate-background-color: skyblue; "
                       "gridline-color: darkgreen; } ";

    // 追加样式表（保留旧样式）
    ui->tableWidget->setStyleSheet(ui->tableWidget->styleSheet() + strStyle);
}

// 槽函数：设置选中条目的前景色和背景色
void Widget::on_pushButtonSelectionCustom_clicked() {
    // 设置选中条目的前景色为红色，背景色为黄色
    QString strStyle = " QTableWidget{ selection-color: red; "
                       "selection-background-color: yellow; } ";

    // 追加样式表
    ui->tableWidget->setStyleSheet(ui->tableWidget->styleSheet() + strStyle);

    // 获取当前选中的条目
    QTableWidgetItem* curItem = ui->tableWidget->currentItem();
    if (curItem != nullptr) curItem->setSelected(true); // 强制将当前条目标记为选中状态
}

// 槽函数：设置所有单元格条目的颜色样式
void Widget::on_pushButtonItemCustom_clicked() {
    // 设置所有条目的前景色为蓝色，背景色为浅绿色
    QString strStyle = " QTableWidget::item{ "
                       "color: blue; "
                       "background-color: lightgreen; "
                       "} ";

    // 追加样式表（注意：此样式可能覆盖其他样式）
    ui->tableWidget->setStyleSheet(ui->tableWidget->styleSheet() + strStyle);
}

// 槽函数：设置左上角按钮（角按钮）的样式
void Widget::on_pushButtonCornerButtonCustom_clicked() {
    // 设置左上角的按钮（行列头交叉处）为绿色，边框为立体效果
    QString strStyle = " QTableCornerButton::section{ "
                       " background: green;  "
                       " border: 2px outset green; "
                       "} ";

    // 追加样式表
    ui->tableWidget->setStyleSheet(ui->tableWidget->styleSheet() + strStyle);
}

// 槽函数：设置表头（行头/列头）的样式
void Widget::on_pushButtonHeaderCustom_clicked() {
    // 设置表头文字颜色为深蓝色，背景颜色为青色
    QString strStyle = " QHeaderView::section { "
                       " color: darkblue; "
                       " background-color: cyan; "
                       "} ";

    // 追加样式表
    ui->tableWidget->setStyleSheet(ui->tableWidget->styleSheet() + strStyle);
}

// 槽函数：清除所有样式
void Widget::on_pushButtonClearStyle_clicked() {
    // 打印当前样式表内容，便于调试
    qDebug() << "old style sheets: \r\n" << ui->tableWidget->styleSheet() << Qt::endl;

    // 清空样式表，恢复默认样式
    ui->tableWidget->setStyleSheet("");

    // 同时关闭交替行颜色显示
    ui->tableWidget->setAlternatingRowColors(false);
}
