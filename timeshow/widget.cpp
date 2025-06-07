#include "widget.h"
#include "./ui_widget.h"

// 构造函数：初始化界面和定时器
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);              // 设置 UI（绑定 .ui 文件中控件）
    this->setWindowTitle("电子钟"); // 设置窗口标题

    // 创建定时器，用于每秒更新时间
    timer = new QTimer(this);     // 创建 QTimer 对象，parent 是 this 表示归属当前窗口
    timer->setSingleShot(false);  // 设置为重复定时（非一次性）
    timer->setInterval(1 * 1000); // 设置定时器间隔为 1 秒（单位：毫秒）

    // 连接定时器超时信号到自定义槽函数 recvTimer
    connect(timer, &QTimer::timeout, this, &Widget::recvTimer);

    // 启动定时器
    timer->start();

    // 初始化日历控件选中今天日期
    on_pushButton_clicked();
}

// 析构函数：释放 UI 资源
Widget::~Widget() {
    delete ui;
}

// 定时器超时后的槽函数（每秒调用一次）
void Widget::recvTimer() {
    QDateTime dt = QDateTime::currentDateTime();      // 获取当前系统时间
    QString timeStr = dt.time().toString("HH:mm:ss"); // 格式化为“时:分:秒”的字符串

    ui->lcdNumber->display(timeStr);                    // 在 LCD 显示时间
    ui->progressBar->setValue(dt.time().second() % 10); // 进度条表示当前秒数模 10，用于循环效果
}

// 点击按钮后设置日历控件为当前日期
void Widget::on_pushButton_clicked() {
    QDateTime dt = QDateTime::currentDateTime();    // 获取当前时间
    ui->calendarWidget->setSelectedDate(dt.date()); // 设置日历控件选中的日期为今天
    ui->calendarWidget->setFocus();                 // 设置焦点到日历控件（方便键盘操作）
}
