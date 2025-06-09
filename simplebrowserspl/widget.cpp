#include "widget.h"

#include <QFileDialog>
#include <QSettings>
#include <QUrl>

#include "./ui_widget.h"

// 构造函数：初始化 UI 组件并连接信号与槽函数
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this); // 设置 UI

    ui->plainTextEdit->setReadOnly(true);        // 设置 plainTextEdit 为只读，用于显示 HTML 源码
    ui->textBrowser->setOpenExternalLinks(true); // 启用超链接的外部打开功能（在浏览器中打开）
    ui->btnBackward->setEnabled(false);          // 初始化时“后退”按钮禁用
    ui->btnForward->setEnabled(false);           // 初始化时“前进”按钮禁用

    // 连接按钮点击信号与 QTextBrowser 的后退/前进槽函数
    connect(ui->btnBackward, &QPushButton::clicked, ui->textBrowser, &QTextBrowser::backward);
    connect(ui->btnForward, &QPushButton::clicked, ui->textBrowser, &QTextBrowser::forward);

    loadSettings();
}

// 析构函数：释放 UI 资源
Widget::~Widget() {
    delete ui;
}

// 打开按钮点击事件：选择 HTML 文件并加载到 QTextBrowser 中
void Widget::on_btnOpen_clicked() {
    // 使用 QFileDialog 打开一个文件选择对话框，获取用户选择的 HTML 文件的 URL
    // 参数说明：
    // 1. this：父窗口指针，指定这个对话框的父对象为当前
    // Widget，确保对话框在当前窗口之上弹出
    // 2. "open HTML"：对话框标题，用于提示用户当前操作是“打开 HTML 文件”
    // 3. QUrl()：初始路径，传入空 QUrl 表示使用默认目录（可用
    // QUrl::fromLocalFile("路径") 指定初始目录）
    // 4. "HTML files(*.htm *.html)"：过滤器字符串，仅显示 *.htm 和 *.html
    // 后缀的文件
    QUrl urlFile = QFileDialog::getOpenFileUrl(this, "open HTML", QUrl(), "HTML files(*.htm *.html)");

           // 判断用户是否实际选择了文件（即返回的 URL 非空）
    if (!urlFile.isEmpty()) {
        qDebug() << urlFile;                 // 输出选择的文件 URL，用于调试
        ui->textBrowser->setSource(urlFile); // 将选择的 HTML 文件加载到 QTextBrowser 中显示
    }
}

// 当 textBrowser 可以后退时，启用“后退”按钮
void Widget::on_textBrowser_backwardAvailable(bool arg1) {
    ui->btnBackward->setEnabled(arg1);
}

// 当 textBrowser 可以前进时，启用“前进”按钮
void Widget::on_textBrowser_forwardAvailable(bool arg1) {
    ui->btnForward->setEnabled(arg1);
}

// 当 textBrowser 内容变化时，更新 plainTextEdit 显示 HTML 源码
void Widget::on_textBrowser_textChanged() {
    QString strHtml = ui->textBrowser->toHtml(); // 获取 HTML 内容
    ui->plainTextEdit->setPlainText(strHtml);    // 显示在 plainTextEdit 中
}

// 当窗口关闭时触发的事件处理函数
void Widget::closeEvent(QCloseEvent* event) {
    // 保存当前窗口及控件的状态到设置文件
    saveSettings();
}

// 保存窗口及控件状态到配置文件
void Widget::saveSettings() {
    // 创建 QSettings 对象，参数是组织名和应用名，数据存储路径与注册表或配置文件相关
    QSettings settings("QtGuide", "SimpleBrowser");

    // 保存主窗口的几何信息（位置和大小）
    QByteArray baMainWidget = this->saveGeometry();

    // 保存分割器（splitter）的状态（布局信息）
    QByteArray baSplitter = ui->splitter->saveState();

    // 获取当前文本浏览器的 URL 来源
    QUrl urlSrc = ui->textBrowser->source();

    // 将以上信息写入配置文件或注册表
    settings.setValue("MainWidget", baMainWidget);
    settings.setValue("Splitter", baSplitter);
    settings.setValue("URL", urlSrc);
}

// 读取配置文件，恢复窗口和控件的状态
void Widget::loadSettings() {
    // 创建 QSettings 对象，路径和保存时相同
    QSettings settings("QtGuide", "SimpleBrowser");

    // 如果存在保存的主窗口几何信息，则恢复窗口位置和大小
    if (settings.contains("MainWidget")) {
        QByteArray baMainWidget = settings.value("MainWidget").toByteArray();
        this->restoreGeometry(baMainWidget);
    }

    // 如果存在保存的分割器状态，则恢复布局
    if (settings.contains("Splitter")) {
        QByteArray baSplitter = settings.value("Splitter").toByteArray();
        ui->splitter->restoreState(baSplitter);
    }

    // 如果存在保存的 URL，则设置文本浏览器显示该 URL
    if (settings.contains("URL")) {
        QUrl urlSrc = settings.value("URL").toUrl();
        ui->textBrowser->setSource(urlSrc);
    }
}
