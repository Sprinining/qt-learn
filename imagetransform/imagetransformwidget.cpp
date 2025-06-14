#include "imagetransformwidget.h"
#include "./ui_imagetransformwidget.h"
#include <QDebug>
#include <QMessageBox>

// 构造函数，初始化 UI 和控件
ImageTransformWidget::ImageTransformWidget(QWidget* parent) : QWidget(parent), ui(new Ui::ImageTransformWidget) {
    ui->setupUi(this); // 加载 UI 组件
    init();            // 初始化界面元素和逻辑
}

// 析构函数，释放资源
ImageTransformWidget::~ImageTransformWidget() {
    delete m_pResizeDlg;
    m_pResizeDlg = nullptr;

    delete m_pRotateDlg;
    m_pRotateDlg = nullptr;

    delete ui;
}

// 初始化函数，设置标签、滚动区、对话框及信号槽
void ImageTransformWidget::init() {
    // 创建用于显示图片的标签
    m_pLabelImage = new QLabel();
    m_pLabelImage->setAlignment(Qt::AlignLeft | Qt::AlignTop);    // 左上对齐
    m_pLabelImage->setStyleSheet("background-color: lightgray;"); // 设置背景色

    // 将标签设置为滚动区域内容
    ui->scrollArea->setWidget(m_pLabelImage);

    // 创建缩放尺寸对话框，并设置为当前窗口的子窗口
    m_pResizeDlg = new ResizeImageDialog(this);

    // 连接信号槽：主窗口发送旧尺寸 → 子对话框接收
    connect(this, &ImageTransformWidget::sendOldSize, m_pResizeDlg, &ResizeImageDialog::recvOldSize);

    // 连接信号槽：子对话框发送新尺寸 → 主窗口执行缩放
    connect(m_pResizeDlg, &ResizeImageDialog::sendNewSize, this, &ImageTransformWidget::recvNewSizeAndResize);

    // 创建旋转图片对话框
    m_pRotateDlg = new RotateImageDialog(this);
    // 旋转使用模态对话框 + exec() 获取角度，不需额外信号槽
}

// 点击“打开图片”按钮的槽函数
void ImageTransformWidget::on_pushButtonOpen_clicked() {
    // 弹出文件选择对话框，选择图片文件
    QString strFile = QFileDialog::getOpenFileName(this, tr("打开图片文件"), tr(""), tr("Image files(*.png *.jpg *.bmp)"));

    if (strFile.isEmpty()) return; // 没选文件

    // 尝试加载图片
    bool bLoadOK = m_image.load(strFile);
    if (!bLoadOK) {
        QMessageBox::warning(this, tr("加载图片文件"), tr("加载图片文件失败，请检查文件格式。"));
        return;
    }

    // 保存文件路径
    m_strFileName = strFile;

    // 显示图片到标签
    m_pLabelImage->setPixmap(m_image);

    // 设置窗口标题为文件名
    setWindowTitle(tr("预览文件为 %1").arg(m_strFileName));
}

// 点击“缩放图片”按钮的槽函数
void ImageTransformWidget::on_pushButtonResize_clicked() {
    if (m_image.isNull()) return; // 图片未加载

    // 将当前图片的宽高发送给缩放对话框
    emit sendOldSize(m_image.width(), m_image.height());

    // 显示对话框
    m_pResizeDlg->show();
    m_pResizeDlg->raise(); // 置顶显示
}

// 点击“旋转图片”按钮的槽函数
void ImageTransformWidget::on_pushButtonRotate_clicked() {
    if (m_image.isNull()) return; // 图片未加载

    // 显示模态旋转对话框，等待用户输入角度
    int nAngle = m_pRotateDlg->exec();
    if (nAngle == 0) return; // 用户没有输入有效角度

    // 构造旋转变换矩阵
    QTransform mxRotate;
    mxRotate.rotate(nAngle);

    // 执行旋转操作，得到新图片
    QPixmap imgNew = m_image.transformed(mxRotate);

    // 更新成员变量并刷新显示
    m_image = imgNew;
    m_pLabelImage->setPixmap(m_image);
}

// 接收新尺寸并执行缩放的槽函数
void ImageTransformWidget::recvNewSizeAndResize(int nNewWidth, int nNewHeight) {
    // 如果尺寸没变化，直接返回
    if ((m_image.width() == nNewWidth) && (m_image.height() == nNewHeight)) return;

    // 缩放图片
    QPixmap imgNew = m_image.scaled(nNewWidth, nNewHeight);

    // 更新成员变量并刷新显示
    m_image = imgNew;
    m_pLabelImage->setPixmap(m_image);
}
