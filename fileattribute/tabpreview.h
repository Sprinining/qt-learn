#ifndef TABPREVIEW_H
#define TABPREVIEW_H

#include <QButtonGroup>
#include <QFile>
#include <QPixmap>
#include <QWidget>

namespace Ui {
class TabPreview;
}

class TabPreview : public QWidget {
    Q_OBJECT

  public:
    explicit TabPreview(QWidget* parent = nullptr); // 构造函数，支持传入父窗口指针，默认空指针
    ~TabPreview();                                  // 析构函数，释放资源
    void initControls();                            // 初始化控件及界面设置

  public slots:
    // 槽函数，响应文件名改变信号，更新预览内容
    void onFileNameChanged(const QString& fileName);

  private:
    Ui::TabPreview* ui;         // 指向 UI 界面对象的指针，由 Qt Designer 生成的类
    QString m_strFileName;      // 用于保存当前预览的文件名
    QButtonGroup m_buttonGroup; // 按钮分组，管理多个互斥的按钮
    QPixmap m_image;            // 用于保存当前加载的预览图像
};

#endif // TABPREVIEW_H
