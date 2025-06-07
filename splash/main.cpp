#include "widget.h" // 引入主窗口类的声明

#include <QApplication>  // 应用程序主入口类
#include <QPixmap>       // 图像处理类，主要用于加载和显示图片
#include <QResource>     // Qt 资源系统类，用于注册外部 .rcc 资源
#include <QSplashScreen> // 启动画面类
#include <QThread>       // 线程处理类，用于调用 sleep 等函数

int main(int argc, char* argv[]) {
    QApplication a(argc, argv); // 创建 Qt 应用程序对象，argc/argv 是命令行参数

	// rcc -binary bigpics.qrc -o bigpics.rcc
	// 手动生成后放到影子构建文件夹下

    // 将 bigpics.rcc 注册为资源文件，里面应包含 splash.png 等图片
    QResource::registerResource("bigpics.rcc");

    // 加载启动图片（注意路径是资源路径而非文件路径）
    QPixmap pixmap(":/splash.png");

    // 缩放图片至 480x270 尺寸（适配窗口大小）
    pixmap = pixmap.scaled(QSize(480, 270));

    // 创建启动画面（Splash Screen）对象并展示图片
    QSplashScreen splash(pixmap);
    splash.show();

    // 在启动图上显示一行提示文字，左下角对齐
    splash.showMessage(QObject::tr("加载中 ..."), Qt::AlignLeft | Qt::AlignBottom);

    // 处理当前事件队列，确保 splash 能及时显示出来
    a.processEvents();

    // 创建主窗口对象
    Widget w;

    // 模拟加载过程，当前线程休眠 3 秒（注意：不可用于真实长耗时任务）
    w.thread()->sleep(3);

    // 显示主窗口
    w.show();

    // 当主窗口显示完成后，关闭启动画面
    splash.finish(&w);

    // 进入 Qt 应用程序事件循环
    return a.exec();
}
