#include "widget.h"
#include "./ui_widget.h"
#include <QMessageBox>

// 构造函数：初始化 UI 和控件
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this); // 绑定 .ui 文件中定义的界面
    initControls();    // 初始化控件的默认状态
}

// 析构函数：释放 UI 内存
Widget::~Widget() {
    delete ui;
}

// 初始化控件
void Widget::initControls() {
    // 默认 IP 设置为 192.168.1.1，并勾选对应的单选框
    m_strIP = tr("192.168.1.1");
    ui->radioButtonIP1->setChecked(true);

    // 默认端口设置为 80，并勾选对应的单选框
    m_nPort = 80;
    ui->radioButtonPort1->setChecked(true);

    // 设置 IP 和 Port 的 GroupBox 为可勾选（Checkable）
    ui->groupBoxIP->setCheckable(true);
    ui->groupBoxPort->setCheckable(true);

    // 设置“连接操作”分组框为可勾选，但初始状态为未启用（即禁用内部控件）
    ui->groupBoxConnection->setCheckable(true);
    ui->groupBoxConnection->setChecked(false);
}

// IP 单选框点击事件，设置对应 IP
void Widget::on_radioButtonIP1_clicked() {
    m_strIP = tr("192.168.1.1");
}

void Widget::on_radioButtonIP2_clicked() {
    m_strIP = tr("192.168.1.2");
}

void Widget::on_radioButtonIP3_clicked() {
    m_strIP = tr("192.168.1.3");
}

// Port 单选框点击事件，设置对应端口号
void Widget::on_radioButtonPort1_clicked() {
    m_nPort = 80;
}

void Widget::on_radioButtonPort2_clicked() {
    m_nPort = 443;
}

void Widget::on_radioButtonPort3_clicked() {
    m_nPort = 8080;
}

// 点击“新建连接”按钮后的处理逻辑
void Widget::on_pushButtonConnect_clicked() {
    // 弹出提示框，显示连接信息
    QString strInfo = tr("新建连接成功：\r\nIP：%1\r\nPort：%2").arg(m_strIP).arg(m_nPort);
    QMessageBox::information(this, tr("新建连接"), strInfo);

    // 禁用 IP 和端口选择组（即取消勾选 groupBox）
    ui->groupBoxIP->setChecked(false);
    ui->groupBoxPort->setChecked(false);

    // 禁用“新建连接”按钮，防止重复连接
    ui->pushButtonConnect->setEnabled(false);

    // 启用连接操作组（勾选 groupBoxConnection，内部控件可用）
    ui->groupBoxConnection->setChecked(true);
}

// 点击“发送数据”按钮后的处理
void Widget::on_pushButtonSend_clicked() {
    QMessageBox::information(this, tr("发送数据"), tr("已发送数据。"));
}

// 点击“接收数据”按钮后的处理
void Widget::on_pushButtonRecv_clicked() {
    QMessageBox::information(this, tr("接收数据"), tr("已接收数据。"));
}

// 点击“关闭连接”按钮后的处理
void Widget::on_pushButtonClose_clicked() {
    // 显示提示信息
    QMessageBox::information(this, tr("关闭连接"), tr("已关闭连接。"));

    // 启用 IP 和端口选择分组框（重新勾选）
    ui->groupBoxIP->setChecked(true);
    ui->groupBoxPort->setChecked(true);

    // 启用“新建连接”按钮
    ui->pushButtonConnect->setEnabled(true);

    // 禁用连接操作分组框（取消勾选）
    ui->groupBoxConnection->setChecked(false);
}

void Widget::setConButtonEnabled(bool bEnable) {
    // 以 bEnable 为 true 启用时为例，如果是禁用那么输入参数设置为 false 即可
    // 启用IP和Port分组框
    ui->groupBoxIP->setChecked(bEnable);
    ui->groupBoxPort->setChecked(bEnable);
    // 启用新建连接按钮
    ui->pushButtonConnect->setEnabled(bEnable);

    // 禁用连接操作分组框
    ui->groupBoxConnection->setChecked(!bEnable);
}
