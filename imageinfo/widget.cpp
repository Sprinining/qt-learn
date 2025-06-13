#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

// 构造函数，初始化界面和控件
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this); // 绑定 UI 文件设计的界面
    initControls();    // 初始化自定义控件和信号槽
}

// 析构函数，释放 UI 指针
Widget::~Widget() {
    delete ui;
}

// 初始化控件、填充数据、连接信号槽等
void Widget::initControls() {
    // 创建第 0 号标签页的图片预览控件 QLabel
    m_pLabelPreview = new QLabel();
    // 设置 scrollArea 的内容控件为预览标签，这样图片可以滚动显示
    ui->scrollArea->setWidget(m_pLabelPreview);
    // 设置标签背景颜色为浅灰色，便于区分区域
    m_pLabelPreview->setStyleSheet("background-color: lightgray;");

    // 第 1 号标签页，设置文本浏览框显示初始提示信息
    ui->textBrowserInfo->setText(tr("用于显示图片文件信息。"));

    // 第 2 号标签页，填充支持保存的图片格式到组合框
    QList<QByteArray> listTypes = QImageWriter::supportedImageFormats();
    int nCount = listTypes.count();
    for (int i = 0; i < nCount; i++) {
        // 添加支持的图片格式到扩展名下拉列表（组合框）
        ui->comboBoxExtFormat->addItem(listTypes[i]);
    }

    // 连接列表控件 currentRowChanged 信号到 stackedWidget 的 setCurrentIndex 槽函数
    // 当列表当前行变化时，堆叠窗口切换显示对应页面
    connect(ui->listWidgetIndex, &QListWidget::currentRowChanged, ui->stackedWidget, &QStackedWidget::setCurrentIndex);

    // 默认显示堆叠控件第 0 个页面
    ui->stackedWidget->setCurrentIndex(0);
}

// 打开图片按钮槽函数
void Widget::on_pushButtonOpen_clicked() {
    // 弹出文件选择对话框，限制图片格式
    QString strFileName = QFileDialog::getOpenFileName(this, tr("打开图片文件"), "", "Images (*.png *.bmp *.jpg);;All files(*)");

    // 如果没有选择文件则直接返回
    if (strFileName.isEmpty()) return;

    // 尝试加载选中的图片文件
    QImage imgTemp;
    if (!imgTemp.load(strFileName)) {
        // 加载失败弹窗提示
        QMessageBox::warning(this, tr("打开文件失败"), tr("加载图片数据失败，不支持该格式。"));
        return;
    }

    // 加载成功，保存图片路径和图片对象
    m_strImageName = strFileName;
    m_image = imgTemp;

    // 在第 0 号标签页的预览 QLabel 中显示图片
    m_pLabelPreview->setPixmap(QPixmap::fromImage(m_image));

    // 构造文件信息字符串
    QString strInfo = m_strImageName + tr("\r\n");
    strInfo += tr("图片尺寸： %1 x %2\r\n").arg(m_image.width()).arg(m_image.height());
    strInfo += tr("颜色深度： %1\r\n").arg(m_image.depth());

    // 显示文件信息到第 1 号标签页的文本浏览框
    ui->textBrowserInfo->setText(strInfo);
}

// 转换图片格式按钮槽函数
void Widget::on_pushButtonConvert_clicked() {
    // 获取用户选中的新格式扩展名
    QString strNewExt = ui->comboBoxExtFormat->currentText();

    // 判断原文件扩展名是否与目标格式相同（忽略大小写）
    if (m_strImageName.endsWith(strNewExt, Qt::CaseInsensitive)) {
        QMessageBox::warning(this, tr("转换图片格式"), tr("新旧图片扩展名一样，不需要转换。"));
        return;
    }

    // 构造新文件路径（同目录，文件名不变，仅扩展名变）
    QFileInfo fi(m_strImageName);
    QString strNewName = fi.absolutePath() + tr("/") + fi.completeBaseName() + tr(".") + strNewExt;
    qDebug() << strNewName; // 输出调试信息

    // 保存图片为新格式文件
    if (m_image.save(strNewName)) {
        // 成功提示
        QMessageBox::information(this, tr("转换图片格式"), tr("转换成功，新文件为：\r\n") + strNewName);
    } else {
        // 失败提示
        QMessageBox::warning(this, tr("转换图片格式"), tr("转换失败！"));
    }
}
