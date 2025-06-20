#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QDebug>
#include <QThread>

class WorkerThread : public QThread {
    Q_OBJECT

  public:
    explicit WorkerThread(int n, QObject* parent = nullptr);
    void run() override;

  signals:
    void resultReady(long long result);

  private:
    int m_n;
};

#endif // WORKERTHREAD_H
