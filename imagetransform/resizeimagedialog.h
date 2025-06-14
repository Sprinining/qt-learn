#ifndef RESIZEIMAGEDIALOG_H
#define RESIZEIMAGEDIALOG_H

#include <QDialog> // 引入 QDialog 基类

// Qt Designer 生成的命名空间，前向声明 UI 类（界面元素封装）
namespace Ui {
class ResizeImageDialog;
}

// 缩放图片的对话框类，继承自 QDialog
class ResizeImageDialog : public QDialog {
    Q_OBJECT // 启用 Qt 的信号槽机制

  public :
    // 构造函数，parent 可选，默认无父对象
    explicit ResizeImageDialog(QWidget* parent = nullptr);

    // 析构函数，释放资源
    ~ResizeImageDialog();

  signals:
    // 发送新尺寸信号（宽度和高度），由主窗口接收并完成缩放操作
    void sendNewSize(int nNewWidth, int nNewHeight);

  public slots:
    // 接收旧尺寸（来自主窗口），用于初始化输入框显示等
    void recvOldSize(int nOldWidth, int nOldHeight);

  private slots:
    // 点击“设置新尺寸”按钮的槽函数，读取输入并发出 sendNewSize 信号
    void on_pushButtonSetNewSize_clicked();

  private:
    Ui::ResizeImageDialog* ui; // Qt UI 界面指针（由 Designer 自动生成）
    void init();               // 初始化函数，用于设置界面默认状态、信号槽连接等
};

#endif // RESIZEIMAGEDIALOG_H
