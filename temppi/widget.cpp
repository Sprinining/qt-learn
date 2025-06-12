#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QDir>
#include <QElapsedTimer> // 用于计算耗时
#include <QMessageBox>
#include <QTemporaryDir>  // 用于创建临时文件夹
#include <QTemporaryFile> // 用于创建临时文件

// 构造函数，初始化 UI 界面
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    ui->radioButton1k->setChecked(true);                         // 默认选择 1k 精度
    qDebug() << tr("应用程序名称：") << qApp->applicationName(); // 打印程序名
    qDebug() << tr("系统临时文件路径：") << QDir::tempPath();    // 打印临时目录路径
}

// 析构函数，释放 UI 资源
Widget::~Widget() {
    delete ui;
}

// 获取用户在界面中选择的 PI 精度长度
int Widget::getPILength() {
    if (ui->radioButton1k->isChecked()) {
        return 1000;
    } else if (ui->radioButton2k->isChecked()) {
        return 2000;
    } else if (ui->radioButton4k->isChecked()) {
        return 4000;
    } else {
        return 8000;
    }
}

// “计算 PI” 按钮的槽函数
void Widget::on_btnCalcPI_clicked() {
    int nPILength = getPILength(); // 获取计算位数
    QElapsedTimer eTimer;
    eTimer.start(); // 启动计时器

    m_calcPI.Calc(nPILength); // 执行 PI 计算

    qint64 nms = eTimer.elapsed(); // 获取耗时（单位：毫秒）

    // 创建一个临时文件保存计算结果
    QTemporaryFile tf;
    if (!tf.open()) {
        QMessageBox::information(this, tr("打开临时文件"), tr("打开临时文件失败！"));
        return;
    }

    tf.setAutoRemove(false); // 设置为不自动删除（为了查看生成结果）

    m_calcPI.WriteToFile(tf); // 将结果写入临时文件

    // 构造输出信息并展示到文本框
    QString strInfo;
    strInfo += tr("计算 %1 位 PI 耗时 %2 毫秒\r\n").arg(nPILength).arg(nms);
    strInfo += tr("保存到临时文件：\r\n%1\r\n").arg(tf.fileName());

    ui->textEdit->setText(strInfo);
}

// “计算所有精度 PI” 按钮的槽函数
void Widget::on_btnCalcAll_clicked() {
    // 创建一个临时目录用于保存所有精度的结果文件
    QTemporaryDir td("PI-"); // 文件夹前缀 PI-
    QString strTempDir;

    if (td.isValid()) {
        strTempDir = td.path();
    } else {
        QMessageBox::warning(this, tr("新建临时文件夹"), tr("新建临时文件夹失败！"));
        return;
    }

    td.setAutoRemove(false); // 设置为不自动删除目录

    QString strInfo; // 最终展示到文本框的信息

    // 依次计算 1000、2000、4000、8000 位的 PI
    for (int i = 1; i <= 8; i *= 2) {
        int nPILength = i * 1000;

        QElapsedTimer eTimer;
        eTimer.start();
        m_calcPI.Calc(nPILength);      // 计算 PI
        qint64 nms = eTimer.elapsed(); // 计算耗时

        // 构造当前输出文件路径
        QString strCurName = strTempDir + tr("/%1.txt").arg(nPILength);
        QFile fileCur(strCurName);

        // 打开文件写入
        if (!fileCur.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, tr("新建文件"), tr("新建存储 PI 的文件失败！"));
            return;
        }

        m_calcPI.WriteToFile(fileCur); // 写入 PI 到文件

        // 构造结果字符串
        strInfo += tr("计算 %1 位 PI，耗时 %2 毫秒\r\n存到 %3 \r\n\r\n").arg(nPILength).arg(nms).arg(fileCur.fileName());
    }

    // 显示所有结果到文本框
    ui->textEdit->setText(strInfo);
}
