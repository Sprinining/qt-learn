#include "widget.h"
#include "drawingarea.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent) : QWidget(parent) {
    // 创建主垂直布局，作为整个窗口的布局容器
    auto *mainLayout = new QVBoxLayout(this);

    // 创建绘图区域控件实例，设置最小高度
    drawingArea = new DrawingArea(this);
    drawingArea->setMinimumHeight(400);

    // 创建水平布局，用于放置按钮和控件
    auto *buttonLayout = new QHBoxLayout();

    // 创建几个功能按钮
    auto *clearBtn = new QPushButton("清除画布", this);
    auto *colorBtn = new QPushButton("选择颜色", this);
    auto *eraserBtn = new QPushButton("橡皮擦 ❌", this);
    auto *saveBtn = new QPushButton("保存图片", this);

    // 创建画笔粗细滑动条，水平滑动，范围1-30，默认值5
    auto *slider = new QSlider(Qt::Horizontal);
    slider->setRange(1, 30);
    slider->setValue(5);

    // 显示当前笔粗的标签
    auto *sliderLabel = new QLabel("笔粗: 5", this);

    // 将按钮和控件添加到水平布局中
    buttonLayout->addWidget(clearBtn);
    buttonLayout->addWidget(colorBtn);
    buttonLayout->addWidget(eraserBtn);
    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(sliderLabel);
    buttonLayout->addWidget(slider);

    // 主布局依次添加绘图区域和按钮布局
    mainLayout->addWidget(drawingArea);
    mainLayout->addLayout(buttonLayout);

    // 连接“清除画布”按钮点击信号，调用绘图区的 clearPoints() 清空画布
    connect(clearBtn, &QPushButton::clicked, drawingArea,
            &DrawingArea::clearPoints);

    // 连接“选择颜色”按钮，弹出颜色选择对话框，设置画笔颜色
    connect(colorBtn, &QPushButton::clicked, this, [=]() {
        QColor color = QColorDialog::getColor(Qt::red, this, "选择画笔颜色");
        if (color.isValid()) {
            drawingArea->setPenColor(color);
        }
    });

    // 连接“橡皮擦”按钮，点击时切换橡皮擦模式，并更新按钮文字状态
    connect(eraserBtn, &QPushButton::clicked, this, [=]() {
        drawingArea->toggleEraserMode();

        // 静态变量记录当前是否处于橡皮擦状态，用于切换按钮文字
        static bool erasing = false;
        erasing = !erasing;
        eraserBtn->setText(erasing ? "橡皮擦 ✅" : "橡皮擦 ❌");
    });

    // 连接“保存图片”按钮，弹出文件保存对话框，保存绘图内容为PNG图片
    connect(saveBtn, &QPushButton::clicked, this, [=]() {
        QString file =
            QFileDialog::getSaveFileName(this, "保存图片", "", "PNG 图片 (*.png)");
        if (!file.isEmpty()) {
            drawingArea->saveToImage(file);
        }
    });

    // 连接滑动条值改变信号，更新标签显示当前笔粗，同时设置绘图区域画笔大小
    connect(slider, &QSlider::valueChanged, this, [=](int value) {
        sliderLabel->setText(QString("笔粗: %1").arg(value));
        drawingArea->setPenSize(value);
    });
}
