#include "workerthread.h"
#include <functional>

WorkerThread::WorkerThread(int n, QObject* parent) : QThread(parent), m_n(n) {
}

void WorkerThread::run() {
    std::function<long long(int)> fib = [&](int x) {
        if (x <= 1) return 1LL;
        return fib(x - 1) + fib(x - 2);
    };

    long long result = fib(m_n);
    emit resultReady(result);
}
