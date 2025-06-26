#include "drawingarea.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

DrawingArea::DrawingArea(QWidget *parent)
    : QWidget(parent), drawing(false), // 初始不绘图
      penColor(Qt::red),               // 默认画笔颜色红色
      penSize(5),                      // 默认笔粗为5
      eraseMode(false) {               // 默认不开启橡皮擦
    setMouseTracking(true); // 开启鼠标追踪，即使无按键移动也能响应mouseMoveEvent
}

// 清空所有绘制数据，重绘界面为空白
void DrawingArea::clearPoints() {
    points.clear(); // 清除所有点坐标
    colors.clear(); // 清除对应颜色数据
    sizes.clear();  // 清除对应大小数据
    update();       // 请求重绘，刷新界面
}

// 设置画笔颜色，同时关闭橡皮擦模式，记录颜色用于恢复
void DrawingArea::setPenColor(const QColor &color) {
    lastPenColor = color; // 记录用户设置的颜色
    penColor = color;     // 当前画笔颜色设为该颜色
    eraseMode = false;    // 关闭橡皮擦模式
}

// 设置画笔大小（圆点半径）
void DrawingArea::setPenSize(int size) { penSize = size; }

// 切换橡皮擦模式，开启时画笔颜色为白色（背景色），关闭时恢复用户上次颜色
void DrawingArea::toggleEraserMode() {
    eraseMode = !eraseMode; // 状态取反切换
    if (eraseMode) {
        penColor = Qt::white; // 橡皮擦模式画白色
    } else {
        penColor = lastPenColor; // 恢复上次画笔颜色
    }
    update(); // 刷新界面，颜色改变可能会有视觉提示
}

// 保存绘图区为图片文件（png等格式）
void DrawingArea::saveToImage(const QString &filePath) {
    QPixmap pixmap(size()); // 创建与控件同尺寸的画布
    render(&pixmap);        // 把当前控件内容渲染到 pixmap
    pixmap.save(filePath);  // 保存到指定文件
}

// 鼠标左键按下事件，开启绘图状态，添加第一个点
void DrawingArea::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawing = true;         // 标记绘图开始
        addPoint(event->pos()); // 添加当前点
    }
}

// 鼠标移动事件，若处于绘图状态，持续添加点
void DrawingArea::mouseMoveEvent(QMouseEvent *event) {
    if (drawing) {
        addPoint(event->pos()); // 添加当前点
    }
}

// 鼠标左键释放事件，结束绘图状态
void DrawingArea::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawing = false; // 标记绘图结束
    }
}

// 绘图事件，绘制所有记录的点（圆形），使用对应颜色和大小
void DrawingArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(event->rect(), Qt::white); // 清空区域，填充背景色白色

    for (int i = 0; i < points.size(); ++i) {
        painter.setBrush(colors[i]);                        // 设置画刷颜色
        painter.setPen(Qt::NoPen);                          // 无边框
        painter.drawEllipse(points[i], sizes[i], sizes[i]); // 画圆点
    }
}

// 添加绘制点（坐标、颜色、大小），添加后请求重绘
void DrawingArea::addPoint(const QPoint &pt) {
    points.append(pt); // 记录点坐标
    colors.append(eraseMode
                      ? Qt::white
                      : penColor); // 当前模式决定颜色（橡皮擦白色，正常画笔色）
    sizes.append(penSize);         // 当前画笔大小
    update();                      // 请求刷新界面，触发paintEvent
}
