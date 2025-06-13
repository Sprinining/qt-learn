#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QMessageBox>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    // 创建自定义的文件预览标签页对象
    m_pTabPreview = new TabPreview();
    // 将文件预览标签页添加到主界面的 tabWidget 中，标签名称为“文件预览”
    ui->tabWidget->addTab(m_pTabPreview, tr("文件预览"));
    // 连接当前类发出的文件名改变信号与文件预览页接收文件名改变槽函数
    connect(this, &Widget::fileNameChanged, m_pTabPreview, &TabPreview::onFileNameChanged);
}

Widget::~Widget() {
    delete ui; // 释放 UI 对象内存
}

void Widget::on_pushButtonSelectFile_clicked() {
    // 弹出文件选择对话框，允许用户选择任意文件
    QString strName = QFileDialog::getOpenFileName(this, tr("选择文件"), tr(""), tr("All files(*)"));
    strName = strName.trimmed();   // 去除文件名字符串首尾空格
    if (strName.isEmpty()) return; // 如果没有选择文件，直接返回

    // 设置成员变量，保存选择的文件名
    m_strFileName = strName;
    // 使用 QFileInfo 设置当前文件信息对象
    m_fileInfo.setFile(m_strFileName);
    // 获取文件大小（单位：字节）
    qint64 nFileSize = m_fileInfo.size();

    // 更新界面上显示的完整文件路径文本框
    ui->lineEditFullName->setText(m_strFileName);
    // 更新界面上显示的文件名（不带路径）文本框
    ui->lineEditShortName->setText(m_fileInfo.fileName());
    // 更新界面上显示的文件大小文本框，格式为“xxx 字节”
    ui->lineEditFileSize->setText(tr("%1 字节").arg(nFileSize));

    // 获取文件的三个时间属性（创建时间、最后访问时间、最后修改时间），格式化为字符串
    QString strTimeCreated = m_fileInfo.birthTime().toString("yyyy-MM-dd  HH:mm:ss");
    QString strTimeRead = m_fileInfo.lastRead().toString("yyyy-MM-dd  HH:mm:ss");
    QString strTimeModified = m_fileInfo.lastModified().toString("yyyy-MM-dd  HH:mm:ss");

    // 更新界面上显示的文件时间文本框
    ui->lineEditTimeCreated->setText(strTimeCreated);
    ui->lineEditTimeRead->setText(strTimeRead);
    ui->lineEditTimeModified->setText(strTimeModified);

    // 发射信号，通知关联的控件文件名已改变，可以更新预览等操作
    emit fileNameChanged(m_strFileName);
}
