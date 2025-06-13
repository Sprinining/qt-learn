#include "tabpreview.h"
#include "ui_tabpreview.h"

TabPreview::TabPreview(QWidget* parent) : QWidget(parent), ui(new Ui::TabPreview) {
    ui->setupUi(this);
    initControls(); // 初始化控件，设置按钮组、样式等
}

TabPreview::~TabPreview() {
    delete ui; // 释放 UI 指针资源
}

void TabPreview::initControls() {
    // 设置三个按钮为可切换状态（类似复选框的选中与未选中状态）
    ui->pushButtonTextPreview->setCheckable(true);
    ui->pushButtonImagePreview->setCheckable(true);
    ui->pushButtonBytePreview->setCheckable(true);

    // 按钮分组，分组内按钮互斥（只能选中一个）
    // 为每个按钮分配唯一 ID（0, 1, 2）
    m_buttonGroup.addButton(ui->pushButtonTextPreview, 0);
    m_buttonGroup.addButton(ui->pushButtonImagePreview, 1);
    m_buttonGroup.addButton(ui->pushButtonBytePreview, 2);

    // 绑定按钮分组的点击信号到堆栈控件，切换不同的页面
    connect(&m_buttonGroup, &QButtonGroup::idClicked, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
    // 设置所有被选中按钮的样式，背景色为黄色
    this->setStyleSheet("QPushButton:checked { background-color: yellow }");

    // 设置字节浏览区背景颜色为浅蓝色
    ui->textBrowserByte->setStyleSheet("background-color: #AAEEFF");

    // 设置图片预览标签背景颜色为浅灰色
    ui->labelImagePreview->setStyleSheet("background-color: #E0E0E0");
}

void TabPreview::onFileNameChanged(const QString& fileName) {
    m_strFileName = fileName;
    // 尝试将文件作为图片加载
    bool isImage = m_image.load(m_strFileName);

    if (isImage) {
        // 是图片时，清空文字提示，显示图片
        ui->labelImagePreview->setText("");
        ui->labelImagePreview->setPixmap(m_image);
    } else {
        // 不是图片时，清空显示的图片，显示提示文字
        m_image = QPixmap(); // 清空图片
        ui->labelImagePreview->setPixmap(m_image);
        ui->labelImagePreview->setText(tr("不是支持的图片，无法以图片预览。"));
    }

    // 打开文件，读取前200字节做文本和十六进制预览
    QFile fileIn(m_strFileName);
    if (!fileIn.open(QIODevice::ReadOnly)) {
        // 无法打开文件，打印调试信息
        qDebug() << tr("文件无法打开：") << m_strFileName;
    } else {
        QByteArray baData = fileIn.read(200); // 读取前200字节
        // 转为本地编码的文本显示
        QString strText = QString::fromLocal8Bit(baData);
        ui->textBrowserText->setText(strText);

        // 转为大写十六进制字符串显示
        QString strHex = baData.toHex().toUpper();
        ui->textBrowserByte->setText(strHex);
    }

    // 根据文件类型，默认切换到对应预览页
    if (isImage) {
        // 图片文件，切换到图片预览按钮（触发 clicked 信号）
        ui->pushButtonImagePreview->click();
    } else {
        // 非图片文件
        if (m_strFileName.endsWith(".txt", Qt::CaseInsensitive) || m_strFileName.endsWith(".h", Qt::CaseInsensitive) || m_strFileName.endsWith(".cpp", Qt::CaseInsensitive) ||
            m_strFileName.endsWith(".c", Qt::CaseInsensitive)) {
            // 纯文本相关文件，切换到文本预览页
            ui->pushButtonTextPreview->click();
        } else {
            // 其他文件，切换到字节预览页
            ui->pushButtonBytePreview->click();
        }
    }
}
