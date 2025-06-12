#include "widget.h"
#include "./ui_widget.h"
#include "ui_widget.h"

#include <QFileDialog>         // 文件选择对话框
#include <QMessageBox>         // 弹出提示框
#include <QStorageInfo>        // 获取磁盘分区信息
#include <QTextDocumentWriter> // 将 QTextDocument 写入文件

// 构造函数，初始化界面
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this); // 初始化 UI 布局
}

// 析构函数，释放资源
Widget::~Widget() {
    delete ui;
}

// 槽函数：点击“获取磁盘信息”按钮后调用
void Widget::on_btnGetDiskInfo_clicked() {
    // 获取当前所有挂载的磁盘分区信息
    QList<QStorageInfo> listDisks = QStorageInfo::mountedVolumes();

    // 用于存储所有磁盘信息的字符串
    QString strInfo;
    QString strTemp;

    // 获取分区数量
    int nCount = listDisks.count();

    // 遍历每个分区
    for (int i = 0; i < nCount; i++) {
        // 添加当前分区编号
        strTemp = tr("分区 %1 信息：<br>").arg(i);
        strInfo += strTemp;

        // 设备名称（如 /dev/sda1）
        strTemp = listDisks[i].device();
        strInfo += tr("设备：") + strTemp + tr("<br>");

        // 挂载点路径（如 /home、C:\）
        strTemp = listDisks[i].rootPath();
        strInfo += tr("挂载点：") + strTemp + tr("<br>");

        // 判断是否为可用分区（加载成功、可访问）
        if (listDisks[i].isValid() && listDisks[i].isReady()) {
            // 卷名（比如 "本地磁盘"）
            strTemp = tr("卷名：%1<br>").arg(listDisks[i].displayName());
            strInfo += strTemp;

            // 文件系统类型（如 NTFS、ext4）
            strTemp = tr("文件系统类型：%1<br>").arg(QString(listDisks[i].fileSystemType()));
            strInfo += strTemp;

            // 是否为系统根目录
            if (listDisks[i].isRoot()) {
                strTemp = tr("<font color=red><b>系统根：是</b></font><br>");
            } else {
                strTemp = tr("系统根：否<br>");
            }
            strInfo += strTemp;

            // 是否只读文件系统
            if (listDisks[i].isReadOnly()) {
                strTemp = tr("只读：是<br>");
            } else {
                strTemp = tr("只读：否<br>");
            }
            strInfo += strTemp;

            // 获取磁盘空间信息（单位：GB）
            double dblAllGB = 1.0 * listDisks[i].bytesTotal() / (1024 * 1024 * 1024);
            double dblFreeGB = 1.0 * listDisks[i].bytesFree() / (1024 * 1024 * 1024);
            strTemp = tr("总空间(GB)：%1 已用：%2 剩余：%3<br>").arg(dblAllGB, 0, 'f', 3).arg(dblAllGB - dblFreeGB, 0, 'f', 3).arg(dblFreeGB, 0, 'f', 3);
            strInfo += strTemp;
        } else {
            // 不可用或未加载的分区
            strTemp = tr("<b>设备不可用或未加载。</b><br>");
            strInfo += strTemp;
        }

        // 每个分区之间增加换行
        strInfo += tr("<br>");
    }

    // 显示收集到的所有磁盘信息（HTML 格式）到 textEdit 中
    ui->textEdit->setText(strInfo);
}

// 槽函数：点击“保存信息”按钮后调用
void Widget::on_btnSaveDiskInfo_clicked() {
    // 弹出“另存为”对话框，让用户选择保存路径和格式
    QString strFileName = QFileDialog::getSaveFileName(this,
                                                       tr("保存信息"),                                              // 对话框标题
                                                       tr("."),                                                     // 默认路径
                                                       tr("Html files(*.htm);;Text files(*.txt);;ODF files(*.odf)") // 文件类型过滤器
                                                       );

    // 如果用户没有选择文件名，直接返回
    if (strFileName.length() < 1) return;

    // 创建 QTextDocumentWriter 对象，用于写入 QTextDocument 到文件
    QTextDocumentWriter tw(strFileName);

    // 执行写入操作，参数是 QTextEdit 的文档对象
    bool bRes = tw.write(ui->textEdit->document());

    // 根据写入结果显示提示框
    if (bRes) {
        QMessageBox::information(this, tr("保存成功"), tr("信息已成功保存到文件。"));
    } else {
        QMessageBox::warning(this, tr("保存出错"), tr("保存到文件出错！"));
    }
}
