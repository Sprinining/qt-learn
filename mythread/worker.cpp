#include "worker.h"

// 构造函数，调用基类 QObject 构造函数
Worker::Worker(QObject* parent) : QObject(parent) {
}

// 执行耗时任务的槽函数，例如计算斐波那契数列的第 n 项
void Worker::doWork(int n) {
    // 使用递归 lambda 计算斐波那契数列
    // 这里采用简单递归，模拟耗时操作
    std::function<long long(int)> fib = [&](int x) {
        if (x <= 1) return 1LL;
        return fib(x - 1) + fib(x - 2);
    };

    long long result = fib(n);

    // 任务完成，发射信号通知结果
    emit resultReady(result);
}
