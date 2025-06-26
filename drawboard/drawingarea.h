#pragma once

#include <QColor>
#include <QPoint>
#include <QVector>
#include <QWidget>

// 绘图区域控件类，继承自 QWidget，实现基本的鼠标绘图功能
class DrawingArea : public QWidget {
    Q_OBJECT

public:
    explicit DrawingArea(QWidget *parent = nullptr);

    // 清空所有绘制的点和数据，刷新界面
    void clearPoints();
    // 设置画笔颜色，会自动关闭橡皮擦模式
    void setPenColor(const QColor &color);
    // 设置画笔大小（圆点半径）
    void setPenSize(int size);
    // 切换橡皮擦模式，开启时笔色为白色（背景色），关闭时恢复上一次画笔颜色
    void toggleEraserMode();
    // 保存当前绘图区域为图片文件，支持png等格式
    void saveToImage(const QString &filePath);

protected:
    // 鼠标按下事件，开始绘图（仅响应左键）
    void mousePressEvent(QMouseEvent *event) override;
    // 鼠标移动事件，按住左键时持续绘图
    void mouseMoveEvent(QMouseEvent *event) override;
    // 鼠标释放事件，结束绘图
    void mouseReleaseEvent(QMouseEvent *event) override;
    // 绘图事件，所有绘制操作都在此完成
    void paintEvent(QPaintEvent *event) override;

private:
    // 当前是否处于绘图状态（鼠标左键按下未释放）
    bool drawing;
    // 存储所有绘制的点坐标
    QVector<QPoint> points;
    // 存储每个点对应的颜色（支持多颜色绘制）
    QVector<QColor> colors;
    // 存储每个点对应的大小（笔粗）
    QVector<int> sizes;
    // 当前画笔颜色（若开启橡皮擦模式，此颜色会被覆盖为白色）
    QColor penColor;
    // 当前画笔大小（圆点半径）
    int penSize;
    // 是否处于橡皮擦模式（开启时用白色绘制）
    bool eraseMode;
    // 记录最后一次用户设置的画笔颜色，用于从橡皮擦切换回时恢复
    QColor lastPenColor;

    // 添加一个绘制点，包含坐标、颜色和大小，完成后请求重绘
    void addPoint(const QPoint &pt);
};
