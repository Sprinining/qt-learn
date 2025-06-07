#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
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

void Widget::sliderValueChanged(int value) {
    if (isMovie && movie && movie->isValid()) {
        movie->jumpToFrame(value);  // 跳转到滑动条对应帧
        isPlaying = false;
        movie->stop();  // 停止播放（用户手动控制）
    }
}

