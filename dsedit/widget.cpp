#include "widget.h"
#include "./ui_widget.h"
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QTextStream>

// 构造函数，初始化 UI 和列表控件选择模式
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    // 设置列表控件只能单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

// 析构函数，释放 UI 资源
Widget::~Widget() {
    delete ui;
}

// 点击“添加”按钮的槽函数
void Widget::on_btnAdd_clicked() {
    // 获取输入框中的数据
    QString strName = ui->lineEditName->text().trimmed();
    QString strAge = ui->lineEditAge->text().trimmed();
    QString strWeight = ui->lineEditWeight->text().trimmed();

    // 检查三个数据项是否为空
    if (strName.isEmpty() || strAge.isEmpty() || strWeight.isEmpty()) {
        QMessageBox::warning(this, tr("添加行"), tr("请填好姓名、年龄、体重三个数据项再添加。"));
        return;
    }

    // 将年龄和体重字符串转换为数值
    qint32 nAge = strAge.toInt();
    double dblWeight = strWeight.toDouble();

    // 年龄和体重范围校验
    if ((nAge < 0) || (nAge > 600)) {
        QMessageBox::warning(this, tr("添加行"), tr("年龄数值不对，应该 0~600 "));
        return;
    }
    if (dblWeight < 0.1) {
        QMessageBox::warning(this, tr("添加行"), tr("体重数值不对，至少 0.1kg "));
        return;
    }

    // 构造用于显示的字符串行
    QString strAll;
    QTextStream tsLine(&strAll);
    tsLine << strName << "\t" << nAge << "\t" << Qt::fixed << qSetRealNumberPrecision(2) << dblWeight;

    // 添加到 listWidget 控件中
    ui->listWidget->addItem(strAll);
}

// 点击“删除”按钮的槽函数
void Widget::on_btnDel_clicked() {
    // 获取当前选中行
    int nCurRow = ui->listWidget->currentRow();
    if (nCurRow < 0) return; // 未选中任何行则不操作

    // 删除该行
    ui->listWidget->takeItem(nCurRow);
}

// 点击“保存 DS 文件”按钮的槽函数
void Widget::on_btnSaveDS_clicked() {
    int nCount = ui->listWidget->count();
    // 无数据不保存
    if (nCount < 1) return;

    // 弹出保存文件对话框
    QString strFileName = QFileDialog::getSaveFileName(this, tr("保存为 DS 文件"), tr("."), tr("DS files(*.ds);;All files(*)"));
    if (strFileName.isEmpty()) return;

    // 打开文件写入
    QFile fileOut(strFileName);
    if (!fileOut.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("无法打开文件"), tr("无法打开要写入的文件：") + fileOut.errorString());
        return;
    }

    // 使用 QDataStream 写入二进制数据
    QDataStream dsOut(&fileOut);
    dsOut.setVersion(QDataStream::Qt_6_9); // 设置版本，确保兼容性

    // 写入文件头部（魔数 + 版本 + 行数）
    dsOut << qint16(0x4453); // 'D''S' 文件标识
    dsOut << qint16(0x0100); // 文件版本 1.0
    dsOut << qint32(nCount); // 总行数

    // 写入每行的姓名、年龄、体重数据
    QString strCurName;
    qint32 nCurAge;
    double dblCurWeight;

    for (int i = 0; i < nCount; i++) {
        QString strLine = ui->listWidget->item(i)->text();
        QTextStream tsLine(&strLine);
        tsLine >> strCurName >> nCurAge >> dblCurWeight;
        dsOut << strCurName << nCurAge << dblCurWeight;
    }

    // 写入完成提示
    QMessageBox::information(this, tr("保存DS文件"), tr("保存为 .ds 文件成功！"));
}

// 点击“加载 DS 文件”按钮的槽函数
void Widget::on_btnLoadDS_clicked() {
    // 弹出打开文件对话框
    QString strFileName = QFileDialog::getOpenFileName(this, tr("打开DS文件"), tr("."), tr("DS files(*.ds);;All files(*)"));
    if (strFileName.isEmpty()) return;

    // 打开文件
    QFile fileIn(strFileName);
    if (!fileIn.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("打开DS文件"), tr("打开DS文件失败: ") + fileIn.errorString());
        return;
    }

    // 创建数据流读取文件
    QDataStream dsIn(&fileIn);

    // 读取头部数据
    qint16 nDS;
    qint16 nVersion;
    qint32 nCount;
    dsIn >> nDS >> nVersion >> nCount;

    // 检查标识是否合法
    if (0x4453 != nDS) {
        QMessageBox::warning(this, tr("打开文件"), tr("指定的文件不是 .ds 文件类型，无法加载。"));
        return;
    }

    // 检查版本是否支持
    if (0x0100 != nVersion) {
        QMessageBox::warning(this, tr("打开文件"), tr("指定的 .ds 文件格式版本不是 1.0，暂时不支持。"));
        return;
    } else {
        dsIn.setVersion(QDataStream::Qt_6_9); // 设置流版本
    }

    // 检查行数是否有效
    if (nCount < 1) {
        QMessageBox::warning(this, tr("打开文件"), tr("指定的 .ds 文件内数据行计数小于 1，无数据加载。"));
        return;
    }

    // 清空原有内容
    ui->listWidget->clear();

    // 读取每行数据
    QString strCurName;
    qint32 nCurAge;
    double dblCurWeight;

    for (int i = 0; i < nCount; i++) {
        // 检查流状态
        if (dsIn.status() != QDataStream::Ok) {
            qDebug() << tr("第 %1 行读取前的状态出错：%2").arg(i).arg(dsIn.status());
            break;
        }

        // 读取数据项
        dsIn >> strCurName >> nCurAge >> dblCurWeight;

        // 构造显示字符串
        QString strLine = tr("%1\t%2\t%3").arg(strCurName).arg(nCurAge).arg(dblCurWeight, 0, 'f', 2);
        ui->listWidget->addItem(strLine);
    }

    // 提示完成
    QMessageBox::information(this, tr("加载DS文件"), tr("加载DS文件完成！"));
}

// 列表控件选中项变化的槽函数
void Widget::on_listWidget_currentRowChanged(int currentRow) {
    if (currentRow < 0) return; // 无选中项

    // 读取该行数据
    QString strLine = ui->listWidget->item(currentRow)->text();
    QTextStream tsLine(&strLine);

    // 拆分成三个字段
    QString strName;
    int nAge;
    double dblWeight;
    tsLine >> strName >> nAge >> dblWeight;

    // 显示到输入框中
    ui->lineEditName->setText(strName);
    ui->lineEditAge->setText(tr("%1").arg(nAge));
    ui->lineEditWeight->setText(tr("%1").arg(dblWeight));
}
