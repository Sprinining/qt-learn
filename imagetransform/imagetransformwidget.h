#ifndef IMAGETRANSFORMWIDGET_H
#define IMAGETRANSFORMWIDGET_H

// 引入自定义对话框头文件：缩放尺寸对话框和旋转对话框
#include "resizeimagedialog.h"
#include "rotateimagedialog.h"

// 引入 Qt 的标准组件
#include <QFileDialog> // 用于文件选择对话框
#include <QLabel>      // 图片显示控件
#include <QPixmap>     // 用于加载和操作图片
#include <QTransform>  // 用于图片的矩阵变换（缩放、旋转等）
#include <QWidget>     // 基类 QWidget

QT_BEGIN_NAMESPACE
namespace Ui {
class ImageTransformWidget; // 前向声明 UI 类
}
QT_END_NAMESPACE

// ImageTransformWidget 继承自 QWidget，是图片缩放与旋转的主界面类
class ImageTransformWidget : public QWidget {
    Q_OBJECT // Qt 元对象宏，支持信号槽机制

  public :
    // 构造函数与析构函数
    ImageTransformWidget(QWidget* parent = nullptr);
    ~ImageTransformWidget();

  signals:
    // 向 resizeImageDialog 发出旧的图片尺寸（宽、高）
    void sendOldSize(int nOldWidth, int nOldHeight);

  public slots:
    // 接收 resizeImageDialog 返回的新尺寸，并执行缩放操作
    void recvNewSizeAndResize(int nNewWidth, int nNewHeight);

  private slots:
    // “打开图片”按钮的槽函数
    void on_pushButtonOpen_clicked();

    // “缩放图片”按钮的槽函数
    void on_pushButtonResize_clicked();

    // “旋转图片”按钮的槽函数
    void on_pushButtonRotate_clicked();

  private:
    Ui::ImageTransformWidget* ui; // UI 指针，界面布局对象

    QLabel* m_pLabelImage; // 用于显示图片的 QLabel
    QPixmap m_image;       // 当前加载的图片对象
    QString m_strFileName; // 当前图片的文件路径

    ResizeImageDialog* m_pResizeDlg; // 缩放尺寸对话框指针
    RotateImageDialog* m_pRotateDlg; // 旋转图片对话框指针

    void init(); // 初始化函数，设置界面控件、信号槽等
};

#endif // IMAGETRANSFORMWIDGET_H
