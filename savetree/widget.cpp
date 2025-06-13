#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QMessageBox>

// 构造函数，设置 UI 与初始树结构
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this); // 初始化 UI 控件

    // 设置树的列数为 1，设置树头标题
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->headerItem()->setText(0, "TreeHeader");

    // 创建顶级节点 A
    QTreeWidgetItem* itemA = new QTreeWidgetItem();
    itemA->setText(0, "A");
    // 设置节点 A 支持双击编辑
    itemA->setFlags(itemA->flags() | Qt::ItemIsEditable);
    ui->treeWidget->addTopLevelItem(itemA);

    // 创建 A 的两个子节点 B 和 C，并设置为可编辑
    QTreeWidgetItem* itemB = new QTreeWidgetItem(itemA);
    itemB->setText(0, "B");
    itemB->setFlags(itemB->flags() | Qt::ItemIsEditable);

    QTreeWidgetItem* itemC = new QTreeWidgetItem(itemA);
    itemC->setText(0, "C");
    itemC->setFlags(itemC->flags() | Qt::ItemIsEditable);

    // 展开所有节点
    ui->treeWidget->expandAll();

    // 设置默认文件名为 s.tree
    ui->lineEditFileName->setText("s.tree");
}

// 析构函数，释放 UI 内存
Widget::~Widget() {
    delete ui;
}

// 添加顶级节点按钮槽函数
void Widget::on_btnAddTop_clicked() {
    QString strText = ui->lineEditItemText->text();
    if (strText.isEmpty()) {
        QMessageBox::warning(this, tr("添加"), tr("条目文本为空不能添加。"));
        return;
    }

    // 创建顶级节点并添加到树中
    QTreeWidgetItem* itemNew = new QTreeWidgetItem();
    itemNew->setText(0, strText);
    itemNew->setFlags(itemNew->flags() | Qt::ItemIsEditable);
    ui->treeWidget->addTopLevelItem(itemNew);
    ui->treeWidget->setFocus();
}

// 添加子节点按钮槽函数
void Widget::on_btnAddChild_clicked() {
    QTreeWidgetItem* curItem = ui->treeWidget->currentItem();
    if (curItem == nullptr) {
        QMessageBox::warning(this, tr("添加子节点"), tr("未选中节点，无法添加子节点。"));
        return;
    }

    QString strText = ui->lineEditItemText->text();
    if (strText.isEmpty()) {
        QMessageBox::warning(this, tr("添加子节点"), tr("条目文本为空不能添加。"));
        return;
    }

    // 创建子节点并添加到当前选中节点
    QTreeWidgetItem* itemChild = new QTreeWidgetItem(curItem);
    itemChild->setText(0, strText);
    itemChild->setFlags(itemChild->flags() | Qt::ItemIsEditable);

    // 展开父节点
    ui->treeWidget->expandItem(curItem);
    ui->treeWidget->setFocus();
}

// 修改树头按钮槽函数
void Widget::on_btnEditHeader_clicked() {
    QString strText = ui->lineEditItemText->text();
    if (strText.isEmpty()) {
        QMessageBox::warning(this, tr("修改树头"), tr("条目文本为空，不能修改树头文本。"));
        return;
    }

    // 修改树头文本
    ui->treeWidget->headerItem()->setText(0, strText);
}

// 保存文件按钮槽函数
void Widget::on_btnSaveFile_clicked() {
    QString strFileName = ui->lineEditFileName->text().trimmed();
    if (strFileName.isEmpty()) {
        QMessageBox::warning(this, tr("保存"), tr("文件名为空，无法保存。"));
        return;
    }

    m_file.setFileName(strFileName);
    if (!m_file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("打开"), tr("要写入的文件无法打开，请检查文件名或权限。"));
        return;
    }

    // 配置数据流绑定文件
    m_ds.setDevice(&m_file);

    // 写入树头信息
    QTreeWidgetItem* iHeader = ui->treeWidget->headerItem();
    m_ds << (*iHeader);

    // 获取隐形根条目（用于遍历所有顶级条目）
    QTreeWidgetItem* iroot = ui->treeWidget->invisibleRootItem();
    // 递归保存整棵树
    saveTree(iroot);

    QMessageBox::information(this, tr("保存完毕"), tr("保存节点到文件完毕。"));

    // 清理资源
    m_file.close();
    m_ds.setDevice(nullptr);
}

// 清空树按钮槽函数
void Widget::on_btnClearTree_clicked() {
    ui->treeWidget->clear();
    ui->treeWidget->headerItem()->setText(0, "");
}

// 加载文件按钮槽函数
void Widget::on_btnLoadFile_clicked() {
    QString strFileName = ui->lineEditFileName->text().trimmed();
    if (strFileName.isEmpty()) {
        QMessageBox::warning(this, tr("文件名"), tr("文件名为空，无法加载。"));
        return;
    }

    m_file.setFileName(strFileName);
    if (!m_file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("打开"), tr("无法打开目标文件，请检查文件名或权限。"));
        return;
    }

    m_ds.setDevice(&m_file);

    // 清空原树
    on_btnClearTree_clicked();

    // 读取树头信息
    QTreeWidgetItem* iHeader = ui->treeWidget->headerItem();
    m_ds >> (*iHeader);
    int nColCount = iHeader->columnCount();
    qDebug() << "Header columns: " << nColCount;

    if ((nColCount < 0) || (nColCount > MAX_COLS)) {
        QMessageBox::critical(this, tr("树头加载异常"), tr("树头条目加载异常，列计数小于 0 或大于 1000。"));
        ui->treeWidget->setColumnCount(1);
        m_file.close();
        m_ds.setDevice(nullptr);
        return;
    }

    // 获取隐形根条目
    QTreeWidgetItem* iroot = ui->treeWidget->invisibleRootItem();
    // 递归加载树结构
    loadFile(iroot);

    // 判断是否加载完整
    QString strMsg = tr("加载文件中树形节点结束。");
    if (m_ds.status() != QDataStream::Ok) {
        strMsg += tr("\r\n文件读取异常，只加载了合格的部分数据。");
    }
    if (!m_ds.atEnd()) {
        int nres = m_file.size() - m_file.pos();
        strMsg += tr("\r\n文件内容未全部加载，后面数据不合格或与该树无关。\r\n剩余未读数据： %1 B").arg(nres);
    }

    QMessageBox::information(this, tr("加载文件结束"), strMsg);
    ui->treeWidget->expandAll();
    m_file.close();
    m_ds.setDevice(nullptr);
}

// 递归保存树结构（先序遍历）
void Widget::saveTree(QTreeWidgetItem* curItem) {
    if (curItem == nullptr) return;

    int nChildCount = curItem->childCount();

    // 保存当前节点和其子节点数量
    m_ds << (*curItem) << nChildCount;

    // 递归保存子节点
    for (int i = 0; i < nChildCount; i++) {
        QTreeWidgetItem* curChild = curItem->child(i);
        saveTree(curChild);
    }
}

// 递归加载树结构（先序遍历）
void Widget::loadFile(QTreeWidgetItem* parentItem) {
    if (m_ds.atEnd()) return;

    // 创建一个新节点，并读取其内容
    QTreeWidgetItem* item = new QTreeWidgetItem();
    m_ds >> (*item);

    // 加入父节点
    parentItem->addChild(item);

    // 读取当前节点的子节点数量
    int nChildCount = 0;
    m_ds >> nChildCount;

    // 递归读取所有子节点
    for (int i = 0; i < nChildCount; ++i) {
        loadFile(item);
    }
}
