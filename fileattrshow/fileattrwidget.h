#ifndef FILEATTRWIDGET_H
#define FILEATTRWIDGET_H

#include "tabpreview.h" // 自定义的预览窗口类
#include <QFile>        // 用于文件操作
#include <QFileDialog>  // 用于打开文件对话框
#include <QFileInfo>    // 提供文件信息类
#include <QWidget>      // QWidget 基类

QT_BEGIN_NAMESPACE
namespace Ui {
class FileAttrWidget; // 前向声明 UI 类（由 Qt Designer 自动生成）
}
QT_END_NAMESPACE

// 文件属性展示与操作的主窗口类，继承自 QWidget
class FileAttrWidget : public QWidget {
    Q_OBJECT

  public:
    // 构造函数，支持父窗口传入
    FileAttrWidget(QWidget* parent = nullptr);

    // 析构函数，负责资源释放
    ~FileAttrWidget();

  signals:
    // 当用户选择了新文件，文件名发生变化时发出该信号
    void fileNameChanged(const QString& fileName);

  private slots:
    // 点击“选择文件”按钮的槽函数，弹出文件选择对话框
    void on_pushButtonSelectFile_clicked();

    // 点击“预览”按钮的槽函数，打开预览窗口
    void on_pushButtonPreview_clicked();

  private:
    Ui::FileAttrWidget* ui; // UI 指针，管理界面控件（由 Qt Designer 自动生成）

    TabPreview* m_pPreviewWnd; // 文件预览子窗口指针

    QString m_strFileName; // 当前选择的文件全路径名

    QFileInfo m_fileInfo; // 文件信息对象，获取如大小、时间等属性

    void init(); // 初始化函数，负责界面设置、信号连接等
};

#endif // FILEATTRWIDGET_H
