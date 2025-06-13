#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QMessageBox>
#include <QTableWidgetItem>           // 表格条目
#include <QTableWidgetSelectionRange> // 表格选中区域

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    InitScoresTable(); // 初始化表格
}

Widget::~Widget() {
    delete ui;
}

// 初始化成绩表格
void Widget::InitScoresTable() {
    ui->tableWidget->setRowCount(2);           // 设置2行
    ui->tableWidget->setColumnCount(4);        // 设罤列

    // 列标题
    QStringList listHeaders;
    listHeaders << tr("语文") << tr(" 数学") << tr(" 外语") << tr(" 物理");
    ui->tableWidget->setHorizontalHeaderLabels(listHeaders);

    // 初始化两行数据
    SetTableRow(0, tr(" 小明"), tr("66"), tr("77"), tr("88"), tr("99"));
    SetTableRow(1, tr(" 小萌"), tr("99"), tr("88"), tr("77"), tr("66"));
}

// 设置指定行数据
void Widget::SetTableRow(int nRow, QString strName, QString strChinese, QString strMath, QString strForeignLanguage, QString strPhysic) {
    // 行头姓名
    QTableWidgetItem* itemName = new QTableWidgetItem(strName);
    ui->tableWidget->setVerticalHeaderItem(nRow, itemName);
    // 各科成绩
    ui->tableWidget->setItem(nRow, 0, new QTableWidgetItem(strChinese));
    ui->tableWidget->setItem(nRow, 1, new QTableWidgetItem(strMath));
    ui->tableWidget->setItem(nRow, 2, new QTableWidgetItem(strForeignLanguage));
    ui->tableWidget->setItem(nRow, 3, new QTableWidgetItem(strPhysic));
}

// 选择改变时执行
void Widget::on_tableWidget_itemSelectionChanged() {
    QList<QTableWidgetItem*> listItems = ui->tableWidget->selectedItems();
    QList<QTableWidgetSelectionRange> listRanges = ui->tableWidget->selectedRanges();
    int nItemsCount = listItems.count();
    int nRangesCount = listRanges.count();
    int nCellsCount = 0;

    // 统计选中单元格数量
    for(int i = 0; i < nRangesCount; i++) {
        nCellsCount += listRanges[i].rowCount() * listRanges[i].columnCount();
    }

    // 输出选择信息
    qDebug() << tr(" 选中条目数：%1，选中区域数：%2，选中单元格数：%3")
                    .arg(nItemsCount).arg(nRangesCount).arg(nCellsCount);
}

// 搜索匹配条目
void Widget::on_btnFind_clicked() {
    QString strDstScore = ui->lineEditFindText->text().trimmed();
    if(strDstScore.isEmpty()) return;
    int nDstScore = strDstScore.toInt();

    int nTheColumn = ui->comboBoxColumns->currentIndex();
    int nCompare = ui->comboBoxCompares->currentIndex();

    int nRowCount = ui->tableWidget->rowCount();
    int nFilteredCount = 0;
    double dblTotal = 0;
    double dblAverage = 0;
    QTableWidgetItem *itemFilteredFirst = nullptr;

    ui->tableWidget->setCurrentItem(nullptr, QItemSelectionModel::Clear); // 清除选中

    // 遍历表格查找符合条件的条目
    for(int i = 0; i < nRowCount; i++) {
        QTableWidgetItem *itemCur = ui->tableWidget->item(i, nTheColumn);
        if(!itemCur) continue;

        int nCurScore = itemCur->text().trimmed().toInt();

        bool match = false;
        if(nCompare == 0 && nCurScore == nDstScore)
            match = true;
        else if(nCompare == 1 && nCurScore <= nDstScore)
            match = true;
        else if(nCompare == 2 && nCurScore >= nDstScore)
            match = true;

        if(match) {
            nFilteredCount++;
            dblTotal += nCurScore;
            itemCur->setSelected(true);

            if(!itemFilteredFirst) {
                itemFilteredFirst = itemCur;
                ui->tableWidget->setCurrentItem(itemCur);
                ui->tableWidget->scrollToItem(itemCur);
            }
        }
    }

    if(nFilteredCount > 0)
        dblAverage = dblTotal / nFilteredCount;

    QString strMsg = tr(" 匹配条目数：%1，总值：%2，均值：%3")
                         .arg(nFilteredCount).arg(dblTotal).arg(dblAverage);
    QMessageBox::information(this, tr(" 查找"), strMsg);
    ui->tableWidget->setFocus(); // 让高亮选择显示
}

// 添加新行
void Widget::on_btnAddLines_clicked() {
    QString strName = ui->lineEditName->text().trimmed();
    if(strName.isEmpty()) return;

    int nOldRowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(nOldRowCount);

    SetTableRow(nOldRowCount, strName, tr("0"), tr("0"), tr("0"), tr("0"));
    ui->tableWidget->scrollToBottom();
}

// 删除选中行
void Widget::on_btnDelLines_clicked() {
    QList<int> listRowIndex;
    QList<QTableWidgetSelectionRange> listRanges = ui->tableWidget->selectedRanges();
    if(listRanges.isEmpty()) return;

    // 编集所有选中行号
    for(const auto &range : listRanges) {
        for(int j = 0; j < range.rowCount(); j++) {
            int nRowIndex = range.topRow() + j;
            if(!listRowIndex.contains(nRowIndex))
                listRowIndex.append(nRowIndex);
        }
    }

    std::sort(listRowIndex.begin(), listRowIndex.end()); // 排序后选择末尾删除
    for(int i = listRowIndex.count() - 1; i >= 0; i--) {
        ui->tableWidget->removeRow(listRowIndex[i]);
    }
}

// 删除选中单元格
void Widget::on_btnDesSelectedItems_clicked() {
    QList<QTableWidgetItem*> listItems = ui->tableWidget->selectedItems();
    if(listItems.isEmpty()) return;

    for(auto item : listItems) {
        int row = item->row();
        int col = item->column();
        delete ui->tableWidget->takeItem(row, col); // 删除单元格
    }
}
