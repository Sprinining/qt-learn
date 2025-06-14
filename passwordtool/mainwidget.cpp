#include "mainwidget.h"
#include "./ui_mainwidget.h"
#include <QDebug>
#include <QMessageBox>

// 构造函数：初始化 UI 并调用 init 函数设置界面和子窗口
MainWidget::MainWidget(QWidget* parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this); // 加载 UI 界面
    init();            // 初始化窗口、子窗口、信号槽等
}

// 析构函数：释放资源
MainWidget::~MainWidget() {
    delete m_pFormChild; // 释放子窗口对象
    m_pFormChild = nullptr;
    delete ui; // 释放 UI 对象
}

// 初始化函数：设置窗口标题、控件属性并建立信号槽连接
void MainWidget::init() {
    // 设置主窗口标题
    setWindowTitle(tr("用户名密码管理工具"));

    // 设置密码输入框为密码模式，输入内容以 * 显示
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);

    // 初始化子窗口指针
    m_pFormChild = nullptr;
    // 创建子窗口，parent 设为 nullptr 防止父窗口控制其生命周期
    m_pFormChild = new FormChangePassword(nullptr);

    // 连接主窗口 -> 子窗口的信号槽：传递用户名和旧密码哈希
    connect(this, &MainWidget::sendOldUserPassword, m_pFormChild, &FormChangePassword::recvOldUserPassword);

    // 连接子窗口 -> 主窗口的信号槽：接收修改后的新密码哈希
    connect(m_pFormChild, &FormChangePassword::sendNewUserPassword, this, &MainWidget::recvNewUserPassword);
}

// 刷新用户列表显示：根据当前的用户哈希表刷新列表控件
void MainWidget::updateListShow() {
    ui->listWidgetShow->clear(); // 清空原有列表项

    // 获取所有用户名（即 QMap 的 key 列表）
    QList<QString> listKeys = m_mapUserAndHash.keys();
    int nCount = listKeys.count();

    // 遍历每个用户，将用户名和哈希拼接显示到列表中
    for (int i = 0; i < nCount; i++) {
        QString curKey = listKeys[i];
        QString strTemp = curKey + QString("\t") + m_mapUserAndHash[curKey];
        ui->listWidgetShow->addItem(strTemp);
    }
}

// 添加用户按钮点击处理函数
void MainWidget::on_pushButtonAddUser_clicked() {
    // 获取用户输入的用户名和密码
    QString strNewUser = ui->lineEditUser->text().trimmed();
    QString strPassword = ui->lineEditPassword->text().trimmed();

    // 检查用户名或密码是否为空
    if (strNewUser.isEmpty() || strPassword.isEmpty()) {
        QMessageBox::information(this, tr("用户名密码检查"), tr("用户名或密码为空，不能添加。"));
        return;
    }

    // 检查用户名是否已存在
    if (m_mapUserAndHash.contains(strNewUser)) {
        QMessageBox::information(this, tr("用户名检查"), tr("已存在该用户名，不能再新增同名。"));
        return;
    }

    // 生成密码哈希（使用 SHA256）
    QByteArray baNewHash = QCryptographicHash::hash(strPassword.toUtf8(), QCryptographicHash::Sha256);
    baNewHash = baNewHash.toHex(); // 转换为十六进制字符串方便显示与存储

    // 添加到用户-哈希映射表中
    m_mapUserAndHash.insert(strNewUser, baNewHash);

    // 刷新用户列表显示
    updateListShow();
}

// 修改密码按钮点击处理函数
void MainWidget::on_pushButtonChangePassword_clicked() {
    int curIndex = ui->listWidgetShow->currentRow(); // 获取当前选中行
    if (curIndex < 0) return;                        // 没有选中任何行

    // 获取当前条目
    QListWidgetItem* curItem = ui->listWidgetShow->item(curIndex);

    // 如果被选中，提取用户名并发送旧密码哈希到子窗口
    if (curItem->isSelected()) {
        QString curLine = curItem->text();             // 获取文本："用户名\t密码哈希"
        QStringList curKeyValue = curLine.split('\t'); // 拆分成用户名和哈希
        QString strUser = curKeyValue[0];
        QByteArray baOldHash = m_mapUserAndHash[strUser];

        // 发出信号，发送用户名和旧密码哈希给子窗口
        emit sendOldUserPassword(strUser, baOldHash);

        // 显示修改密码的子窗口
        m_pFormChild->show();

        // 若子窗口被最小化，恢复原尺寸显示
        if (m_pFormChild->isMinimized()) m_pFormChild->showNormal();

        // 子窗口置顶
        m_pFormChild->raise();
    }
}

// 删除用户按钮点击处理函数
void MainWidget::on_pushButtonDelUser_clicked() {
    int curIndex = ui->listWidgetShow->currentRow(); // 获取当前行索引
    if (curIndex < 0) return;

    QListWidgetItem* curItem = ui->listWidgetShow->item(curIndex);

    if (curItem->isSelected()) {
        QString curLine = curItem->text(); // "用户名\t哈希"
        QStringList curKeyValue = curLine.split('\t');
        QString strUser = curKeyValue[0];

        // 从 map 中删除该用户
        m_mapUserAndHash.remove(strUser);

        // 从 UI 列表中删除条目
        ui->listWidgetShow->takeItem(curIndex); // 先从列表中拿出来
        delete curItem;                         // 然后释放内存
        curItem = nullptr;
    }
}

// 接收子窗口返回的新密码哈希，并更新用户哈希表
void MainWidget::recvNewUserPassword(QString strUser, QByteArray baNewHash) {
    m_mapUserAndHash[strUser] = baNewHash; // 更新哈希值
    updateListShow();                      // 刷新列表显示

    m_pFormChild->hide();                                                 // 隐藏子窗口
    QMessageBox::information(this, tr("修改密码"), tr("修改密码成功。")); // 弹窗提示
}
