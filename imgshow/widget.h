#ifndef WIDGET_H
#define WIDGET_H

#include <QImageReader> // 用于读取图像（错误类型也在此）
#include <QMovie>       // 用于播放动态图（如 GIF）
#include <QPixmap>      // 用于显示静态图像
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = nullptr);
    ~Widget();

public slots:
    // 动图播放错误的槽函数，连接 QMovie::error 信号
    void recvPlayError(QImageReader::ImageReaderError error);

    // 帧变化的槽函数，用于更新进度条，连接 QMovie::frameChanged 信号
    void recvFrameNumber(int frameNumber);

private slots:
    // UI 按钮点击的槽函数（自动连接）
    void on_btnOpenPic_clicked(); // 打开静态图按钮
    void on_btnOpenMov_clicked(); // 打开动态图按钮
    void on_btnStart_clicked();   // 播放按钮
    void on_btnStop_clicked();    // 停止按钮

private:
    Ui::Widget* ui; // 指向 UI 界面类的指针，自动生成并管理所有控件

    // 用于显示静态图的指针（建议用智能指针管理）
    QPixmap* pixmap = nullptr;

    // 用于播放动态图的指针（如 gif/mng）
    QMovie* movie = nullptr;

    // 标志位：当前是否为动态图（true 表示是 QMovie）
    bool isMovie = false;

    // 标志位：动态图是否正在播放
    bool isPlaying = false;

    // 清除上一个图像或动画的辅助函数，释放资源并重置状态
    void clearOldShow();
};

#endif // WIDGET_H
