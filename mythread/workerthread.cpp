#include "workerthread.h"
#include <functional>

WorkerThread::WorkerThread(int n, QObject* parent) : QThread(parent), m_n(n) {
}

void WorkerThread::run() {
    // 这是线程的入口函数，run() 会在新线程中执行

    // 这里模拟一个耗时计算，比如斐波那契数
    std::function<long long(int)> fib = [&](int x) {
        if (x <= 1) return 1LL;
        return fib(x - 1) + fib(x - 2);
    };

    long long result = fib(m_n);

    // 线程中不能直接操作 UI，只能发信号通知主线程
    emit resultReady(result);

    // 线程结束，run函数返回，线程自动退出
}
