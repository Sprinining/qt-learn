#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QFileDialog>     // 文件对话框，用于选择文件
#include <QFileInfo>       // 用于获取文件信息（如文件名、路径等）
#include <QListWidget>     // 列表控件类
#include <QListWidgetItem> // 列表控件的条目类
#include <QMessageBox>     // 消息框控件，用于弹出提示信息

// 构造函数
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);                                                      // 设置 UI
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection); // 设置列表支持多选
    // 默认勾选自动排序复选框
    ui->checkBoxAutoSort->setCheckState(Qt::Checked);
}

// 析构函数
Widget::~Widget() {
    delete ui;
}

// 点击“添加”按钮的槽函数
void Widget::on_btnAdd_clicked() {
    // 弹出文件选择对话框，允许用户选择多个音乐文件
    QStringList slist = QFileDialog::getOpenFileNames(this, tr("添加多个音乐文件"), tr("."), tr("Music files(*.mp3 *.wma *.wav);;All files(*)"));
    int nCount = slist.count(); // 获取选中的文件数
    if (nCount < 1) return;     // 没有选中文件就直接返回

    // 遍历每一个选中的文件
    for (int i = 0; i < nCount; i++) {
        // 创建新条目，并添加到 listWidget 中
        QListWidgetItem* theItem = new QListWidgetItem(ui->listWidget);
        QFileInfo fi(slist[i]);                     // 获取文件信息
        theItem->setText(fi.completeBaseName());    // 设置显示的名称为不带扩展名的基本名
        theItem->setToolTip(fi.absoluteFilePath()); // 设置工具提示为完整路径，便于后续导出
    }
}

// 点击“删除”按钮的槽函数
void Widget::on_btnDel_clicked() {
    QList<QListWidgetItem*> itemList = ui->listWidget->selectedItems(); // 获取所有被选中的条目
    int nCount = itemList.count();                                      // 数量
    if (nCount < 1) return;
    // 删除选中的条目（从后往前删以避免索引混乱）
    for (int i = nCount - 1; i >= 0; i--) {
        int theRow = ui->listWidget->row(itemList[i]); // 获取条目的行号
        ui->listWidget->takeItem(theRow);              // 从控件中移除条目
        delete itemList[i];                            // 删除对象释放内存
        itemList[i] = nullptr;
    }
    itemList.clear(); // 清空指针列表
}

// 点击“导出M3U”按钮的槽函数
void Widget::on_btnExportM3U_clicked() {
    int nCount = ui->listWidget->count(); // 获取列表条目数量
    if (nCount < 1) return;               // 没有条目就返回

    // 弹出保存文件对话框，获取要保存的文件名
    QString strName = QFileDialog::getSaveFileName(this, tr("保存为 M3U 文件"), tr("."), tr("M3U files(*.m3u)"));
    if (strName.isEmpty()) return;

    QFile fileOut(strName); // 创建文件对象
    // 尝试以写文本方式打开文件
    if (!fileOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("打开文件"), tr("无法打开指定文件，请检查是否有写入权限！"));
        return;
    }

    QTextStream tsOut(&fileOut);        // 创建文本流用于写入
    tsOut << tr("#EXTM3U") << Qt::endl; // 写入 M3U 文件头

    // 将每一个条目的完整路径写入文件
    for (int i = 0; i < nCount; i++) {
        QString strCurName = ui->listWidget->item(i)->toolTip(); // 获取完整路径
        tsOut << strCurName << Qt::endl;
    }
}

// 点击“查找”按钮的槽函数
void Widget::on_btnFind_clicked() {
    QString strTemplate = ui->lineEditTemplate->text(); // 获取查找关键词
    if (strTemplate.isEmpty()) return;

    // 清除之前选中的条目
    ui->listWidget->setCurrentItem(NULL, QItemSelectionModel::Clear);

    // 查找所有包含关键词的条目
    QList<QListWidgetItem*> list = ui->listWidget->findItems(strTemplate, Qt::MatchContains);
    int nCount = list.count();
    if (nCount < 1) {
        QMessageBox::information(this, tr("查找条目"), tr("没有找到匹配的条目文本。"));
        return;
    }

    // 高亮第一个匹配项并滚动到它的位置
    ui->listWidget->setCurrentItem(list[0]);
    ui->listWidget->scrollToItem(list[0], QAbstractItemView::PositionAtTop);

    // 设置所有匹配项为选中状态
    for (int i = 0; i < nCount; i++) {
        list[i]->setSelected(true);
    }

    // 设置焦点到列表控件
    ui->listWidget->setFocus();
}

// “自动排序”复选框点击槽函数
void Widget::on_checkBoxAutoSort_clicked(bool checked) {
    if (checked) {
        ui->listWidget->setSortingEnabled(true); // 启用排序功能
        ui->checkBoxReverse->setEnabled(true);   // 启用“逆序”复选框
        // 根据“逆序”复选框判断排序方式
        if (ui->checkBoxReverse->checkState() != Qt::Checked) {
            ui->listWidget->sortItems(Qt::AscendingOrder); // 正序排序
        } else {
            ui->listWidget->sortItems(Qt::DescendingOrder); // 逆序排序
        }
    } else {
        ui->listWidget->setSortingEnabled(false); // 关闭排序功能
        ui->checkBoxReverse->setEnabled(false);   // 禁用“逆序”复选框
    }
}

// “逆序排序”复选框点击槽函数
void Widget::on_checkBoxReverse_clicked(bool checked) {
    if (!checked) {
        ui->listWidget->sortItems(Qt::AscendingOrder); // 正序
    } else {
        ui->listWidget->sortItems(Qt::DescendingOrder); // 逆序
    }
}
