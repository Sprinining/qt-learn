#ifndef WIDGET_H
#define WIDGET_H

#include "worker.h"
#include <QLabel>
#include <QThread>
#include <QWidget>

// Qt 命名空间宏，兼容自动生成的 UI 代码
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

// Widget 类，主界面窗口，负责 UI 和启动子线程工作
class Widget : public QWidget {
    Q_OBJECT

  public:
    // 构造函数，默认父对象为 nullptr
    Widget(QWidget* parent = nullptr);

    // 析构函数，负责资源清理
    ~Widget();

  private slots:
    // 按钮点击槽函数，启动耗时任务
    void onButtonClicked();

    // 接收 Worker 返回结果，更新界面
    void handleResult(long long);

  private:
    Ui::Widget* ui;        // UI 界面指针（由 Qt Designer 生成）
    Worker* worker;        // 子线程中执行任务的 Worker 对象
    QThread* workerThread; // 运行 Worker 的线程
    QLabel* label;         // 用于显示任务结果的标签控件

  signals:
    // 自定义信号，通知子线程开始执行任务
    void startWorkInThread(int n);
};

#endif // WIDGET_H
