#ifndef WORKER_H
#define WORKER_H

#include <QObject>

// Worker 类用于在子线程中执行耗时任务
// 继承自 QObject，以支持 Qt 的信号与槽机制
class Worker : public QObject {
    Q_OBJECT

  public:
    // 构造函数，parent 默认为 nullptr
    explicit Worker(QObject* parent = nullptr);

  public slots:
    // 执行具体任务的槽函数
    // 例如计算斐波那契数列的第 n 项
    void doWork(int n);

  signals:
    // 任务完成后发出的信号，携带计算结果
    // 通常连接主线程的槽函数以更新 UI
    void resultReady(long long result);
};

#endif // WORKER_H
