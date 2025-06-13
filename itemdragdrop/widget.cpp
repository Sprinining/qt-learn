#include "widget.h"
#include "./ui_widget.h"

// 构造函数
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // ========== 初始化 QListWidget 控件条目 ==========
    for (int i = 0; i < 5; i++) {
        QListWidgetItem* itemL = new QListWidgetItem(ui->listWidget); // 创建列表条目
        itemL->setText(tr("listItem %1").arg(i));                     // 设置文本，如 "listItem 0"
    }

    // ========== 初始化 QTreeWidget 控件 ==========
    ui->treeWidget->setColumnCount(2);                                    // 设置树形控件有两列
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::Stretch); // 所有列自动均分宽度

    // 创建若干条目，每个条目有两列文本
    for (int i = 0; i < 5; i++) {
        QTreeWidgetItem* itemT = new QTreeWidgetItem(ui->treeWidget); // 添加根节点
        itemT->setText(0, tr("treeItem %1, 0").arg(i));               // 第一列文本
        itemT->setText(1, tr("t%1, 1").arg(i));                       // 第二列文本
    }

    // ========== 初始化 QTableWidget 控件 ==========
    ui->tableWidget->setColumnCount(3);                                              // 设置列数为 3
    ui->tableWidget->setRowCount(3);                                                 // 设置行数为 3
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 所有列自动拉伸

    // 创建表格项
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            QTableWidgetItem* itemTA = new QTableWidgetItem();
            itemTA->setText(tr("tableItem %1, %2").arg(i).arg(j)); // 设置内容，如 "tableItem 0, 1"
            ui->tableWidget->setItem(i, j, itemTA);                // 设置到表格中
        }
    }

    // 默认选择内部移动模式（radioButtonInter 被选中）
    ui->radioButtonInter->setChecked(true);
    on_radioButtonInter_clicked(true); // 调用槽函数，启用内部拖动
}

// 析构函数
Widget::~Widget() {
    delete ui;
}

// 内部拖动模式：控件内条目可通过拖动重新排序（不支持跨控件）
void Widget::on_radioButtonInter_clicked(bool checked) {
    if (checked) {
        // 设置列表控件为内部移动模式
        ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);
        // 设置树形控件为内部移动模式
        ui->treeWidget->setDragDropMode(QAbstractItemView::InternalMove);
        // 设置表格控件为内部移动模式
        ui->tableWidget->setDragDropMode(QAbstractItemView::InternalMove);
    }
}

// 跨控件拖动模式：支持拖动条目到其他控件（如列表 -> 表格）
void Widget::on_radioButtonOuter_clicked(bool checked) {
    if (checked) {
        // 分别为各控件启用跨控件拖动支持
        setOuterDragDrop(ui->listWidget);
        setOuterDragDrop(ui->treeWidget);
        setOuterDragDrop(ui->tableWidget);
    }
}

// 为传入的视图控件启用跨界拖拽
void Widget::setOuterDragDrop(QAbstractItemView* view) {
    view->setSelectionMode(QAbstractItemView::SingleSelection); // 设置为单选模式，避免多项拖动
    view->setDragEnabled(true);                                 // 启用拖动功能（作为拖动源）
    view->viewport()->setAcceptDrops(true);                     // 视口接收拖放（作为拖动目标）
    view->setDropIndicatorShown(true);                          // 拖动过程中显示插入指示器
    view->setDragDropMode(QAbstractItemView::DragDrop);         // 既可拖出又可拖入，支持跨控件拖放
}
