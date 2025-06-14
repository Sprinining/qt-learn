#include "fileattrwidget.h"
#include "./ui_fileattrwidget.h"
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>

// 构造函数，初始化 UI 和界面状态
FileAttrWidget::FileAttrWidget(QWidget* parent) : QWidget(parent), ui(new Ui::FileAttrWidget) {
    ui->setupUi(this); // 初始化 UI 界面控件（Qt Designer 生成的界面）
    init();            // 执行初始化操作
}

// 析构函数，释放资源
FileAttrWidget::~FileAttrWidget() {
    delete m_pPreviewWnd; // 释放预览窗口对象
    m_pPreviewWnd = nullptr;
    delete ui; // 释放 UI 界面对象
}

// 初始化函数，设置控件属性、创建子窗口、连接信号槽
void FileAttrWidget::init() {
    // 将所有信息展示框设置为只读，防止用户修改
    ui->lineEditFullName->setReadOnly(true);
    ui->lineEditShortName->setReadOnly(true);
    ui->lineEditFileSize->setReadOnly(true);
    ui->lineEditTimeCreated->setReadOnly(true);
    ui->lineEditTimeRead->setReadOnly(true);
    ui->lineEditTimeModified->setReadOnly(true);

    // 初始化预览窗口指针
    m_pPreviewWnd = nullptr;
    m_pPreviewWnd = new TabPreview(nullptr); // 创建预览窗口实例

    // 设置预览窗口标题
    m_pPreviewWnd->setWindowTitle(tr("预览文件"));

    // 当文件名发生变化时，将信号连接到预览窗口的槽函数以更新内容
    connect(this, &FileAttrWidget::fileNameChanged, m_pPreviewWnd, &TabPreview::onFileNameChanged);
}

// “选择文件”按钮的槽函数
void FileAttrWidget::on_pushButtonSelectFile_clicked() {
    // 打开文件选择对话框，允许选择任意文件
    QString strName = QFileDialog::getOpenFileName(this, tr("选择文件"), tr(""), tr("All files(*)"));

    strName = strName.trimmed();   // 去掉前后空格
    if (strName.isEmpty()) return; // 用户未选择文件直接返回

    // 记录选中的文件路径
    m_strFileName = strName;

    // 使用 QFileInfo 提取文件属性
    m_fileInfo.setFile(m_strFileName);

    // 获取文件大小（字节）
    qint64 nFileSize = m_fileInfo.size();

    // 设置全路径、短文件名、文件大小信息
    ui->lineEditFullName->setText(m_strFileName);
    ui->lineEditShortName->setText(m_fileInfo.fileName());
    ui->lineEditFileSize->setText(tr("%1 字节").arg(nFileSize));

    // 获取文件创建、读取、修改时间，并格式化为字符串
    QString strTimeCreated = m_fileInfo.birthTime().toString("yyyy-MM-dd  HH:mm:ss");
    QString strTimeRead = m_fileInfo.lastRead().toString("yyyy-MM-dd  HH:mm:ss");
    QString strTimeModified = m_fileInfo.lastModified().toString("yyyy-MM-dd  HH:mm:ss");

    // 将时间信息设置到对应控件中
    ui->lineEditTimeCreated->setText(strTimeCreated);
    ui->lineEditTimeRead->setText(strTimeRead);
    ui->lineEditTimeModified->setText(strTimeModified);

    // 发送文件名变化信号，通知预览窗口更新
    emit fileNameChanged(m_strFileName);
}

// “预览”按钮的槽函数
void FileAttrWidget::on_pushButtonPreview_clicked() {
    if (m_strFileName.isEmpty()) return; // 如果未选择文件则不操作

    // 如果预览窗口已显示，先隐藏
    if (m_pPreviewWnd->isVisible()) {
        m_pPreviewWnd->hide();
    }

    // 设置预览窗口为应用程序级模态（阻塞其他窗口操作）
    m_pPreviewWnd->setWindowModality(Qt::ApplicationModal);

    // 显示预览窗口
    m_pPreviewWnd->show();
}
