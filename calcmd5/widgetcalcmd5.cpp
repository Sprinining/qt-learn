#include "widgetcalcmd5.h"
#include "./ui_widgetcalcmd5.h"

// 构造函数，初始化 UI 界面
WidgetCalcMD5::WidgetCalcMD5(QWidget* parent) : QWidget(parent), ui(new Ui::WidgetCalcMD5) {
    ui->setupUi(this); // 绑定 UI 界面
}

// 析构函数，释放 UI 内存
WidgetCalcMD5::~WidgetCalcMD5() {
    delete ui;
}

// 点击“浏览”按钮：弹出文件选择对话框
void WidgetCalcMD5::on_pushButtonBrowser_clicked() {
    // 打开文件选择对话框
    QString strFileName = QFileDialog::getOpenFileName(this, tr("选择文件"), "", "All files (*)");

    // 用户未选择文件
    if (strFileName.isEmpty()) {
        m_dlgErrorMsg.showMessage(tr("文件名为空，未选择文件！"));
        return;
    }

    // 记录文件名并显示在界面上
    m_strFileName = strFileName;
    ui->lineEditFileName->setText(m_strFileName);

    // 清空之前的显示信息
    ui->textBrowserInfo->clear();
}

// 点击“计算MD5”按钮，执行校验计算
void WidgetCalcMD5::on_pushButtonCalcMD5_clicked() {
    // 获取用户输入的文件路径
    QString strFileName = ui->lineEditFileName->text().trimmed();

    // 检查是否为空
    if (strFileName.isEmpty()) {
        m_dlgErrorMsg.showMessage(tr("文件名编辑框内容为空！"));
        return;
    }

    m_strFileName = strFileName;

    // 创建 QFile 对象
    QFile fileIn(m_strFileName);

    // 尝试打开文件
    if (!fileIn.open(QIODevice::ReadOnly)) {
        m_dlgErrorMsg.showMessage(tr("打开指定文件失败！"));
        return;
    }

    // 获取文件大小
    qint64 nFileSize = fileIn.size();
    if (nFileSize < 1) {
        m_dlgErrorMsg.showMessage(tr("文件大小为 0，没有数据！"));
        fileIn.close();
        return;
    }

    // 调用函数计算 MD5 值
    QByteArray baMD5 = calcFileMD5(fileIn, nFileSize);

    // 构造显示信息
    QString strInfo = tr("文件名：") + m_strFileName;
    strInfo += tr("\n文件大小：%1 字节").arg(nFileSize);
    strInfo += tr("\nMD5校验值：\n");
    strInfo += baMD5.toHex().toUpper(); // 转为十六进制并大写

    // 显示信息到文本框
    ui->textBrowserInfo->setText(strInfo);

    fileIn.close(); // 关闭文件
}

// 实际计算 MD5 值的函数，支持大文件分块读取 + 进度显示
QByteArray WidgetCalcMD5::calcFileMD5(QFile& fileIn, qint64 nFileSize) {
    QByteArray baRet;                                   // 返回值：最终的 MD5 校验结果
    QCryptographicHash algMD5(QCryptographicHash::Md5); // MD5 计算器
    QByteArray baCurData;                               // 用于临时存储读取的数据块

    // 小文件（<100KB）直接一次性读取计算
    if (nFileSize < 100 * 1000) {
        baCurData = fileIn.readAll(); // 一次读完
        algMD5.addData(baCurData);    // 添加到哈希器中
        baRet = algMD5.result();      // 获取最终结果
        return baRet;
    }

    // 对于大文件，进行分块读取
    qint64 oneBlockSize = nFileSize / 100; // 每块大小（大概分成 100 块）
    int nBlocksCount = 100;
    if (nFileSize % oneBlockSize != 0) {
        // 如果不能整除，块数 +1
        nBlocksCount += 1;
    }

    // 初始化进度对话框
    QProgressDialog dlgProgress(tr("正在计算MD5 ..."), tr("取消计算"), 0, nBlocksCount, this);
    dlgProgress.setWindowModality(Qt::WindowModal); // 模态，阻止用户其他操作
    dlgProgress.setMinimumDuration(0);              // 立即显示

    // 开始分块读取 + 更新进度条
    for (int i = 0; i < nBlocksCount; i++) {
        dlgProgress.setValue(i); // 设置当前进度

        // 判断用户是否取消
        if (dlgProgress.wasCanceled()) break;

        // 读取一个数据块并添加到 MD5 计算中
        baCurData = fileIn.read(oneBlockSize);
        algMD5.addData(baCurData);
    }

    // 如果没有被取消，则取出计算结果
    if (!dlgProgress.wasCanceled()) baRet = algMD5.result();

    dlgProgress.setValue(nBlocksCount); // 完成时设置为最大值，关闭进度框
    return baRet;
}
