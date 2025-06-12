#include "widget.h"
#include "./ui_widget.h"
#include <QDataStream>     // 用于对象串行化（序列化）和反串行化
#include <QDebug>          // 用于调试输出
#include <QFile>           // 文件操作类
#include <QFileDialog>     // 文件选择对话框
#include <QIntValidator>   // 整数范围验证器，用于端口号输入
#include <QListWidgetItem> // 用于操作列表控件的行
#include <QMessageBox>     // 消息弹窗

// 构造函数：初始化界面和控件设置
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);                                                    // 设置 UI
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection); // 列表只能单选

    // 设置两个端口输入框只能输入 0-65535 之间的整数
    QIntValidator* valSrc = new QIntValidator(0, 65535);
    ui->lineEditSrcPort->setValidator(valSrc);
    QIntValidator* valDst = new QIntValidator(0, 65535);
    ui->lineEditDstPort->setValidator(valDst);
}

// 析构函数：释放 UI
Widget::~Widget() {
    delete ui;
}

// 重载运算符：将 UDP 包写入数据流中（用于保存）
QDataStream& operator<<(QDataStream& stream, const UDPPacker& udp) {
    stream << udp.m_srcPort;
    stream << udp.m_dstPort;
    stream << udp.m_length;
    stream << udp.m_checksum;
    stream.writeRawData(udp.m_data.data(), udp.m_data.size()); // 写入报文数据部分
    return stream;
}

// 重载运算符：从数据流中读取 UDP 包（用于加载）
QDataStream& operator>>(QDataStream& stream, UDPPacker& udp) {
    stream >> udp.m_srcPort;
    stream >> udp.m_dstPort;
    stream >> udp.m_length;
    stream >> udp.m_checksum;
    int nMsgLen = udp.m_length - 8;       // UDP 包数据区长度 = 总长度 - 头部长度
    char* buff = new char[nMsgLen];       // 分配内存缓冲区
    stream.readRawData(buff, nMsgLen);    // 读入数据区
    udp.m_data.setRawData(buff, nMsgLen); // 设置 QByteArray 的原始数据指针
    return stream;
}

// 添加 UDP 包按钮点击槽函数
void Widget::on_btnAddUDP_clicked() {
    QString strSrcPort = ui->lineEditSrcPort->text().trimmed();
    QString strDstPort = ui->lineEditDstPort->text().trimmed();
    QString strMsg = ui->lineEditMsg->text().trimmed();

    // 若任一输入为空，提示警告
    if (strSrcPort.isEmpty() || strDstPort.isEmpty() || strMsg.isEmpty()) {
        QMessageBox::warning(this, tr("添加包"), tr("请先填写两个端口和消息字符串。"));
        return;
    }

    UDPPacker udp;
    QByteArray baMsg = strMsg.toUtf8();    // 消息转为字节数组
    udp.m_srcPort = strSrcPort.toUShort(); // 源端口
    udp.m_dstPort = strDstPort.toUShort(); // 目的端口
    udp.m_length = 8 + baMsg.size();       // 总长度 = 头部 + 数据
    udp.m_checksum = 0;                    // UDP 校验和可省略为 0
    udp.m_data = baMsg;                    // 设置数据字段

    QByteArray baAll;
    QDataStream dsOut(&baAll, QIODevice::ReadWrite); // 将数据写入内存字节数组
    dsOut << udp;

    QString strAll = baAll.toHex();  // 转为十六进制字符串显示
    ui->listWidget->addItem(strAll); // 添加到列表控件中
}

// 删除 UDP 包按钮点击槽函数
void Widget::on_btnDelUDP_clicked() {
    int nCurRow = ui->listWidget->currentRow(); // 获取当前行号
    if (nCurRow < 0) return;                    // 无选中项，直接返回
    ui->listWidget->takeItem(nCurRow); // 删除该行项
}

// 保存按钮槽函数
void Widget::on_btnSave_clicked() {
    int nCount = ui->listWidget->count(); // 获取 UDP 包数量
    if (nCount < 1) return;

    QString strFileName = QFileDialog::getSaveFileName(this, tr("保存UDP文件"), tr("."), tr("UDP files(*.udp);;All files(*)"));
    if (strFileName.isEmpty()) return;

    QFile fileOut(strFileName);
    if (!fileOut.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("保存UDP文件"), tr("打开要保存的文件失败：") + fileOut.errorString());
        return;
    }

    QDataStream dsOut(&fileOut); // 输出数据流
    dsOut << qint32(nCount);     // 首先写入包个数

    UDPPacker udpCur;
    for (int i = 0; i < nCount; i++) {
        QString strHex = ui->listWidget->item(i)->text();        // 提取十六进制字符串
        QByteArray baCur = QByteArray::fromHex(strHex.toUtf8()); // 转为字节数组
        QDataStream dsIn(baCur);
        dsIn >> udpCur; // 提取结构体
        dsOut << udpCur; // 写入文件
    }

    QMessageBox::information(this, tr("保存UDP包"), tr("保存UDP包到文件完毕！"));
}

// 加载按钮槽函数
void Widget::on_btnLoad_clicked() {
    QString strFileName = QFileDialog::getOpenFileName(this, tr("打开UDP文件"), tr("."), tr("UDP files(*.udp);;All files(*)"));
    if (strFileName.isEmpty()) return;

    QFile fileIn(strFileName);
    if (!fileIn.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("打开UDP文件"), tr("打开指定UDP文件失败：") + fileIn.errorString());
        return;
    }

    QDataStream dsIn(&fileIn); // 输入数据流
    qint32 nCount;
    dsIn >> nCount; // 读取包数量
    if (nCount < 1) {
        QMessageBox::warning(this, tr("加载UDP包"), tr("指定UDP文件内数据包计数小于1，无法加载。"));
        return;
    }

    ui->listWidget->clear(); // 清空列表控件
    UDPPacker udpCur;
    for (int i = 0; i < nCount; i++) {
        if (dsIn.status() != QDataStream::Ok) {
            qDebug() << tr("读取第 %1 个数据包前的状态错误：%2").arg(i).arg(dsIn.status());
            break;
        }

        dsIn >> udpCur;

        QByteArray baCur;
        QDataStream dsOut(&baCur, QIODevice::ReadWrite); // 把 UDP 包写回字节流
        dsOut << udpCur;

        QString strHex = baCur.toHex();  // 转为十六进制字符串
        ui->listWidget->addItem(strHex); // 添加显示
    }

    QMessageBox::information(this, tr("加载UDP包"), tr("加载文件中的UDP包完成！"));
}

// 当前行变更槽函数：选中某行时，更新界面显示
void Widget::on_listWidget_currentRowChanged(int currentRow) {
    if (currentRow < 0) return;

    QString strHex = ui->listWidget->item(currentRow)->text();
    QByteArray baAll = QByteArray::fromHex(strHex.toUtf8());
    QDataStream dsIn(baAll);
    UDPPacker udp;
    dsIn >> udp; // 提取结构体

    // 更新界面三个编辑框的内容
    ui->lineEditSrcPort->setText(tr("%1").arg(udp.m_srcPort));
    ui->lineEditDstPort->setText(tr("%1").arg(udp.m_dstPort));
    ui->lineEditMsg->setText(QString::fromUtf8(udp.m_data));
}
