#ifndef WIDGET_H
#define WIDGET_H

#include "tabpreview.h" // 自定义的文件预览标签页类
#include <QFile>        // 文件操作相关类
#include <QFileDialog>  // 文件选择对话框类
#include <QFileInfo>    // 文件信息类，获取文件大小、路径、时间等属性
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget; // 自动生成的UI类声明
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

  public:
    // 构造函数，parent为父窗口指针，默认空指针
    explicit Widget(QWidget* parent = nullptr);

    // 析构函数，负责清理资源
    ~Widget();

  signals:
    // 当文件名发生变化时触发该信号，参数是新的文件名
    void fileNameChanged(const QString& fileName);

  private slots:
    // 按钮点击槽函数，响应“选择文件”按钮的点击事件
    void on_pushButtonSelectFile_clicked();

  private:
    Ui::Widget* ui;            // 界面UI对象指针，由Qt Designer生成和管理
    TabPreview* m_pTabPreview; // 自定义的文件预览标签页指针，用于显示文件预览内容
    QString m_strFileName;     // 保存当前选择的文件全路径
    QFileInfo m_fileInfo;      // QFileInfo对象，用于获取文件详细信息，如大小、修改时间等
};

#endif // WIDGET_H
