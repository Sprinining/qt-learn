#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QTextStream>

// 构造函数：初始化 UI 并设置 listWidget 的选择模式为单选
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

// 析构函数：释放 UI 指针
Widget::~Widget() {
    delete ui;
}

// 加载按钮点击槽函数：从文件加载数据到 listWidget 中
void Widget::on_btnLoad_clicked() {
    // 弹出文件选择对话框
    QString strFile = QFileDialog::getOpenFileName(this, tr("打开文件"), tr("."), tr("Text Files(*.txt);;All files(*)"));
    if (strFile.isEmpty()) return;

    QFile fileIn(strFile);
    // 打开文件失败提示
    if (!fileIn.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("打开文件"), tr("打开文件失败：") + fileIn.errorString());
        return;
    }

    QTextStream tsIn(&fileIn);
    QString strCurName;
    int nCurAge;
    double dblCurWeight;
    ui->listWidget->clear(); // 清空现有项

    // 逐行读取文件内容
    while (!tsIn.atEnd()) {
        tsIn >> strCurName;
        if (strCurName.isEmpty()) {
            tsIn.skipWhiteSpace(); // 跳过空白
            continue;
        }
        if (strCurName.startsWith('#')) {
            tsIn.readLine(); // 跳过注释行
            continue;
        }

        tsIn >> nCurAge >> dblCurWeight;

        // 使用 QTextStream 格式化输出为固定小数点两位
        QString strOut;
        QTextStream tsOut(&strOut);
        tsOut.setRealNumberNotation(QTextStream::FixedNotation);
        tsOut.setRealNumberPrecision(2);
        tsOut << strCurName << "\t" << nCurAge << "\t" << dblCurWeight;

        ui->listWidget->addItem(strOut); // 添加到列表
    }

    QMessageBox::information(this, tr("加载表格"), tr("加载自定义表格完毕。"));
}

// 保存按钮点击槽函数：将 listWidget 中的数据保存到文件
void Widget::on_btnSave_clicked() {
    int nCount = ui->listWidget->count();
    if (nCount < 1) return;

    QString strFileSave = QFileDialog::getSaveFileName(this, tr("保存文件"), tr("."), tr("XLS files(*.xls);;Text Files(*.txt)"));
    if (strFileSave.isEmpty()) return;

    QFile fileOut(strFileSave);
    if (!fileOut.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("保存文件"), tr("打开保存文件失败：") + fileOut.errorString());
        return;
    }

    QTextStream tsOut(&fileOut);
    tsOut.setRealNumberNotation(QTextStream::FixedNotation);
    tsOut.setRealNumberPrecision(2);

    // 写入表头
    tsOut << tr("#姓名\t岁数\t体重") << Qt::endl;

    // 写入每一行数据
    for (int i = 0; i < nCount; i++) {
        QString strCurAll = ui->listWidget->item(i)->text();
        QTextStream tsLine(&strCurAll);
        QString strName;
        int nAge;
        double dblWeight;
        tsLine >> strName >> nAge >> dblWeight;
        tsOut << strName << "\t" << nAge << "\t" << dblWeight << Qt::endl;
    }

    QMessageBox::information(this, tr("保存文件"), tr("保存表格文件成功。"));
}

// 删除按钮点击槽函数：删除当前选中的行
void Widget::on_btnDelRow_clicked() {
    int nCurIndex = ui->listWidget->currentRow(); // 获取当前行索引
    if (nCurIndex < 0) return;                    // 没有选中则返回
    ui->listWidget->takeItem(nCurIndex);          // 删除该项
}

// 添加按钮点击槽函数：添加新行数据到 listWidget
void Widget::on_btnAddRow_clicked() {
    QString strName = ui->lineEditName->text().trimmed();
    QString strAge = ui->lineEditAge->text().trimmed();
    QString strWeight = ui->lineEditWeight->text().trimmed();

    // 检查输入项是否为空
    if (strName.isEmpty() || strAge.isEmpty() || strWeight.isEmpty()) {
        QMessageBox::warning(this, tr("添加行"), tr("请先填好三项数据再添加！"));
        return;
    }

    int nAge = strAge.toInt();
    double dblWeight = strWeight.toDouble();

    // 检查年龄与体重范围
    if ((nAge < 0) || (nAge > 600)) {
        QMessageBox::warning(this, tr("添加行"), tr("年龄不能是负数或超过600！"));
        return;
    }
    if (dblWeight < 0.1) {
        QMessageBox::warning(this, tr("添加行"), tr("重量不能低于 0.1 kg ！"));
        return;
    }

    // 使用 QString::arg 格式化为两位小数
    QString strCurAll = tr("%1\t%2\t%3").arg(strName).arg(nAge).arg(dblWeight, 0, 'f', 2);
    ui->listWidget->addItem(strCurAll); // 添加新项
}

// 当前列表项变化时触发：同步数据到编辑框
void Widget::on_listWidget_currentRowChanged(int currentRow) {
    if (currentRow < 0) return;

    QString strCurAll = ui->listWidget->item(currentRow)->text();
    QTextStream tsLine(&strCurAll);
    QString strName;
    int nAge;
    double dblWeight;

    tsLine >> strName >> nAge >> dblWeight;

    // 设置文本框内容
    ui->lineEditName->setText(strName);
    ui->lineEditAge->setText(tr("%1").arg(nAge));
    ui->lineEditWeight->setText(tr("%1").arg(dblWeight));
}
