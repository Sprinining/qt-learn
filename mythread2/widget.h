#ifndef WIDGET_H
#define WIDGET_H

#include "workerthread.h"
#include <QLabel>
#include <QWidget>

class Widget : public QWidget {
    Q_OBJECT

  public:
    explicit Widget(QWidget* parent = nullptr);
    ~Widget();

  private slots:
    void onStartWork();
    void onResultReady(long long result);

  private:
    QLabel* label;
    WorkerThread* workerThread;
};

#endif // WIDGET_H
