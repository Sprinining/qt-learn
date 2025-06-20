#include "widget.h"
#include <QPushButton>
#include <QVBoxLayout>

Widget::Widget(QWidget* parent) : QWidget(parent), workerThread(nullptr) {

    auto* btn = new QPushButton("计算 Fib(35)", this);
    label = new QLabel("等待中...", this);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(btn);
    layout->addWidget(label);

    connect(btn, &QPushButton::clicked, this, &Widget::onStartWork);
}

Widget::~Widget() {
    if (workerThread) {
        workerThread->quit();
        workerThread->wait();
        delete workerThread;
    }
}

void Widget::onStartWork() {
    if (workerThread && workerThread->isRunning()) return;

    label->setText("计算中...");
    workerThread = new WorkerThread(35);

    connect(workerThread, &WorkerThread::resultReady, this, &Widget::onResultReady);
    connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);

    workerThread->start();
}

void Widget::onResultReady(long long result) {
    label->setText(QString("结果: %1").arg(result));
}
