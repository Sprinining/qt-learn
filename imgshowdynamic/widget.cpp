#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QResizeEvent>
#include <QScrollArea>

// 构造函数：初始化 UI 并用 QScrollArea 包装 label 实现滚动支持
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // 获取 labelShow 的原始几何尺寸
    // 表示该控件相对于其父控件的几何信息（位置 + 大小）
    QRect rcLabel = ui->labelShow->geometry();

    // 创建一个滚动区域并将 labelShow 设置为其子控件
    QScrollArea* scrollArea = new QScrollArea(this);
    // 把 labelShow 这个 QLabel 设置为 scrollArea（滚动区域）的子控件。
    // scrollArea 会自动显示 labelShow 的内容；
    // 如果 labelShow 太大（超出 scrollArea 大小），scrollArea 会提供滚动条；
    // scrollArea 会接管 labelShow 的父对象和显示逻辑。
    scrollArea->setWidget(ui->labelShow);
    // 将 scrollArea 的大小和位置设置为 labelShow 原本在窗口中的位置和大小
    scrollArea->setGeometry(rcLabel);

    // 输出当前支持的图片与动画格式
    qDebug() << QImageReader::supportedImageFormats();
    qDebug() << QMovie::supportedFormats();

    // 设置主界面窗体最小尺寸
    this->setMinimumSize(350, 350);
}

// 析构函数：释放资源
Widget::~Widget() {
    clearOldShow(); // 清除旧显示内容（手动释放 pixmap 或 movie）
    delete ui;
}

// 清除旧的图像/动画内容，释放内存
void Widget::clearOldShow() {
    ui->labelShow->clear(); // 清空显示

    // 释放静态图资源
    if (pixmap != nullptr) {
        delete pixmap;
        pixmap = nullptr;
    }

    // 释放动态图资源
    if (movie != nullptr) {
        if (isPlaying) movie->stop(); // 正在播放先停止
        delete movie;
        movie = nullptr;
    }

    isMovie = false;
    isPlaying = false;
}

// 点击“打开图片”按钮后的响应槽函数
void Widget::on_btnOpenPic_clicked() {
    QString fileName;

    // 弹出文件选择对话框
    fileName = QFileDialog::getOpenFileName(this, tr("打开静态图"), "", "Pictures (*.bmp *.jpg *.jpeg *.png *.xpm);;All files(*)");
    if (fileName.isEmpty()) return;

    clearOldShow(); // 释放上一个资源
    qDebug() << fileName;

    pixmap = new QPixmap();
    if (pixmap->load(fileName)) {
        ui->labelShow->setPixmap(*pixmap);          // 设置图像
        ui->labelShow->setGeometry(pixmap->rect()); // 自动调整 label 大小
        isMovie = false;
        isPlaying = false;
    } else {
        delete pixmap;
        pixmap = nullptr;
        QMessageBox::critical(this, tr("打开失败"), tr("打开图片失败，文件名为：\r\n%1").arg(fileName));
    }
}

// 点击“打开动画”按钮后的响应槽函数
void Widget::on_btnOpenMov_clicked() {
    QString fileName;

    // 弹出文件选择对话框
    fileName = QFileDialog::getOpenFileName(this, tr("打开动态图"), "", "Movies (*.gif *.mng);;All files(*)");
    if (fileName.isEmpty()) return;

    clearOldShow(); // 清除旧资源
    qDebug() << fileName;

    movie = new QMovie(fileName);
    if (!movie->isValid()) {
        QMessageBox::critical(this, tr("动态图格式不可用"), tr("动态图格式不支持或读取出错，文件名为：\r\n%1").arg(fileName));
        delete movie;
        movie = nullptr;
        return;
    }

    // 获取总帧数（有些 gif 不支持，返回 -1）
    int count = movie->frameCount();
    qDebug() << tr("总帧数：%1").arg(count);

    // 设置滑动条最大值（回退默认值 100 以支持未知帧数）
    ui->horizontalSlider->setMaximum(count > 0 ? count : 100);

    // 设置动画显示到 label 中
    ui->labelShow->setMovie(movie);
    isMovie = true;
    isPlaying = false;

    // 使用新信号槽语法连接错误处理与帧更新
    connect(movie, &QMovie::error, this, &Widget::recvPlayError);
    connect(movie, &QMovie::frameChanged, this, &Widget::recvFrameNumber);

    // 跳转到第一帧并设置 label 大小
    if (movie->jumpToFrame(0)) {
        ui->labelShow->setGeometry(movie->frameRect());
    }
}

// “播放”按钮点击槽函数
void Widget::on_btnStart_clicked() {
    if (!isMovie || isPlaying) return; // 非动画或已经在播放
    isPlaying = true;
    movie->start();
    qDebug() << tr("循环计数：%1").arg(movie->loopCount());
}

// “暂停”按钮点击槽函数
void Widget::on_btnStop_clicked() {
    if (!isMovie || !isPlaying) return;
    isPlaying = false;
    movie->stop();
}

// 动画播放出错时的处理槽函数
void Widget::recvPlayError(QImageReader::ImageReaderError error) {
    qDebug() << tr("读取动态图错误的代码：%1").arg(error);
    QMessageBox::critical(this, tr("播放出错"), tr("播放动态图出错，文件名为：\r\n%1").arg(movie->fileName()));
    isPlaying = false;
}

// 每一帧更新时，设置滑动条位置
void Widget::recvFrameNumber(int frameNumber) {
    ui->horizontalSlider->setValue(frameNumber);
}

void Widget::resizeEvent(QResizeEvent* event) {
    int W = event->size().width();
    int H = event->size().height();

    // 先计算第二行四个按钮的左上角坐标，按钮尺寸固定为 75*23
    // 第一个按钮
    int x1 = 10;                    // 左边距 10
    int y1 = H - 10 - 21 - 10 - 23; // 10 都是间隔，21 是水平滑动条高度，23 是按钮高度
    // 第四个按钮
    int x4 = W - 10 - 75; // 10 是右边距，75 是按钮宽度
    int y4 = y1;          // 与第一个按钮同一水平线
    // 计算四个按钮的三个间隙总大小
    int nTriGap = W - 10 - 10 - 75 * 4;
    // 计算单个间隙
    int nGap = nTriGap / 3;
    // 计算第二个按钮坐标
    int x2 = x1 + 75 + nGap;
    int y2 = y1;
    // 计算第三个按钮左边
    int x3 = x4 - 75 - nGap;
    int y3 = y1;

    // 设置四个按钮的矩形
    ui->btnOpenPic->setGeometry(x1, y1, 75, 23);
    ui->btnOpenMov->setGeometry(x2, y2, 75, 23);
    ui->btnStart->setGeometry(x3, y3, 75, 23);
    ui->btnStop->setGeometry(x4, y4, 75, 23);

    // 计算第三行水平滑动条的坐标和尺寸
    int xSlider = x2;
    int ySlider = H - 10 - 21;
    int wSlider = W - x2 - 10;
    int hSlider = 21;
    // 设置水平滑动条的矩形
    ui->horizontalSlider->setGeometry(xSlider, ySlider, wSlider, hSlider);

    // 计算包裹标签的滚动区域占用的矩形
    int xLabel = 10;
    int yLabel = 10;
    int wLabel = W - 10 - 10;
    int hLabel = H - 10 - 21 - 10 - 23 - 10 - 10;
    // 设置包裹标签的滚动区域矩形
    QScrollArea* pSA = this->findChild<QScrollArea*>(); // 查找子对象
    // 如果 pSA 不为 nullptr 才能设置矩形
    if (pSA != nullptr) pSA->setGeometry(xLabel, yLabel, wLabel, hLabel);
}
