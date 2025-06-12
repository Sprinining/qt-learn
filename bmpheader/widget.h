#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

// ==================== BMP 头结构定义区域 ==================== //

// 为了避免结构体因为字节对齐而产生错误的大小，强制使用 1 字节对齐
// BMP 文件头长度应为 14 字节，信息头长度应为 40 字节
#pragma pack(1)

// BMP 文件头结构（Bitmap File Header），共 14 字节
struct BMPFileHeader {
    quint16 bfType;      // 文件类型标志，必须为 'BM' (0x4D42)，表示为位图文件
    quint32 bfSize;      // 文件大小（单位：字节），包括文件头、信息头、颜色表和像素数据
    quint16 bfReserved1; // 保留字段，必须为 0
    quint16 bfReserved2; // 保留字段，必须为 0
    quint32 bfOffBits;   // 像素数据的起始偏移（单位：字节），相对于文件开头的偏移量
};

// BMP 信息头结构（Bitmap Info Header），共 40 字节
struct BMPInfoHeader {
    quint32 biSize;          // 信息头结构体大小（应为 40）
    quint32 biWidth;         // 图像宽度（单位：像素）
    quint32 biHeight;        // 图像高度（单位：像素），正值表示从下到上绘制，负值为从上到下绘制
    quint16 biPlanes;        // 色彩平面数，必须为 1（始终为 1）
    quint16 biBitCount;      // 每像素所占位数（即色深），常见值为 1, 4, 8, 24, 32
    quint32 biCompression;   // 压缩方式（0 = BI_RGB 不压缩，1 = BI_RLE8，2 = BI_RLE4 等）
    quint32 biSizeImage;     // 图像数据所占字节数（不包括头和调色板，可能为 0）
    quint32 biXPelsPerMeter; // 水平分辨率（像素/米）
    quint32 biYPelsPerMeter; // 垂直分辨率（像素/米）
    quint32 biClrUsed;       // 实际使用的调色板颜色数（0 表示使用所有）
    quint32 biClrImportant;  // 重要颜色数（0 表示所有颜色都重要）
};

// ==================== Qt UI 主窗口类定义 ==================== //

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

// 主窗口类定义，继承自 QWidget
class Widget : public QWidget {
    Q_OBJECT

  public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

  private slots:
    // 槽函数：点击 “浏览文件” 按钮
    void on_btnBrowser_clicked();
    // 槽函数：点击 “显示图片” 按钮
    void on_btnShowPic_clicked();
    // 槽函数：点击 “读取 BMP 头部” 按钮
    void on_btnReadHeader_clicked();

  private:
    // UI 指针，用于访问界面上的控件
    Ui::Widget* ui;
};

#endif // WIDGET_H
