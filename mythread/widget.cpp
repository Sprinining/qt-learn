#include "widget.h"
#include "./ui_widget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

// 构造函数，创建 UI 组件，初始化线程和 Worker 对象，连接信号槽
Widget::Widget(QWidget* parent) {
    // 创建一个按钮，点击时触发计算任务
    auto* btn = new QPushButton("计算 Fib(35)");
    // 显示结果或状态的标签
    label = new QLabel("等待中...");
    // 垂直布局，按钮和标签依次排列
    auto* layout = new QVBoxLayout;
    layout->addWidget(btn);
    layout->addWidget(label);
    // 设置 Widget 的布局为上述创建的布局
    setLayout(layout);

    // 创建新的线程，传入 this 作为父对象，方便资源管理
    workerThread = new QThread(this);
    // 创建 Worker 对象，负责执行耗时任务
    worker = new Worker();
    // 将 Worker 对象移动到子线程中执行
    worker->moveToThread(workerThread);

    // 启动线程
    workerThread->start();

    // 连接按钮点击信号到本类槽函数，响应用户操作
    connect(btn, &QPushButton::clicked, this, &Widget::onButtonClicked);
    // 连接自定义信号，触发 Worker 在子线程执行任务
    connect(this, &Widget::startWorkInThread, worker, &Worker::doWork);
    // 连接 Worker 完成任务信号，更新主线程 UI 显示结果
    connect(worker, &Worker::resultReady, this, &Widget::handleResult);

    // 线程结束时，自动释放 Worker 对象资源
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    // 当 Widget 被销毁时，通知线程退出
    connect(this, &Widget::destroyed, workerThread, &QThread::quit);
    // 线程结束时自动释放线程对象资源
    connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);
}

// 析构函数，关闭线程并等待线程退出，释放 ui 资源
Widget::~Widget() {
    workerThread->quit(); // 通知线程退出事件循环
    workerThread->wait(); // 阻塞等待线程退出
    delete ui;            // 删除 UI 界面指针（如果有使用 ui 文件）
}

// 按钮点击槽函数，更新状态显示，并发射信号通知子线程开始工作
void Widget::onButtonClicked() {
    label->setText("计算中...");
    emit startWorkInThread(35); // 启动计算 Fib(35) 的任务
}

// 接收子线程计算完成信号，更新标签显示结果
void Widget::handleResult(long long result) {
    label->setText(QString("结果: %1").arg(result));
}
