#ifndef ROTATEIMAGEDIALOG_H
#define ROTATEIMAGEDIALOG_H

#include <QDialog> // Qt 提供的对话框基类

// Qt Designer 自动生成的命名空间，前向声明 UI 类
namespace Ui {
class RotateImageDialog;
}

// 旋转图片的对话框类，继承自 QDialog
class RotateImageDialog : public QDialog {
    Q_OBJECT // 启用 Qt 的信号与槽功能

  public :
    // 构造函数，parent 默认为空指针（无父窗口）
    explicit RotateImageDialog(QWidget* parent = nullptr);

    // 析构函数，自动释放资源
    ~RotateImageDialog();

  private slots:
    // 当点击“确认旋转”按钮时触发的槽函数
    void on_pushButtonRotating_clicked();

  private:
    Ui::RotateImageDialog* ui; // UI 界面指针，用于访问 Designer 中定义的控件
    void init();               // 初始化函数，用于设置控件状态、连接信号槽等
};

#endif // ROTATEIMAGEDIALOG_H
