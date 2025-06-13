#include "widget.h"
#include "./ui_widget.h"

// 构造函数：初始化 UI 界面和控件设置
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this); // 设置 UI
    initControls();    // 初始化控件的状态和属性
}

// 析构函数：释放 UI 占用的资源
Widget::~Widget() {
    delete ui;
}

// 初始化控件
void Widget::initControls() {
    // 设置默认选中的颜色为“无背景色”
    ui->radioButtonNull->setChecked(true);

    // 配置 Line Width 的 SpinBox 范围和初始值
    ui->spinBoxLineWidth->setRange(0, 27);
    ui->spinBoxLineWidth->setValue(1);

    // 配置 Mid Line Width 的 SpinBox 范围和初始值
    ui->spinBoxMidLineWidth->setRange(0, 27);
    ui->spinBoxMidLineWidth->setValue(0);

    // Frame Shape 列表，顺序与 comboBox 条目对应
    m_listFrameShape << QFrame::NoFrame << QFrame::Box << QFrame::Panel << QFrame::WinPanel << QFrame::HLine << QFrame::VLine << QFrame::StyledPanel;

    // comboBoxFrameShape 添加各个边框形状选项
    ui->comboBoxFrameShape->addItem("NoFrame");
    ui->comboBoxFrameShape->addItem("Box");
    ui->comboBoxFrameShape->addItem("Panel");
    ui->comboBoxFrameShape->addItem("WinPanel");
    ui->comboBoxFrameShape->addItem("HLine");
    ui->comboBoxFrameShape->addItem("VLine");
    ui->comboBoxFrameShape->addItem("StyledPanel");

    // 设置默认选择 StyledPanel
    ui->comboBoxFrameShape->setCurrentIndex(6);

    // Frame Shadow 列表，顺序与 comboBox 条目对应
    m_listFrameShadow << QFrame::Plain << QFrame::Raised << QFrame::Sunken;

    // comboBoxFrameShadow 添加阴影样式选项
    ui->comboBoxFrameShadow->addItem("Plain");
    ui->comboBoxFrameShadow->addItem("Raised");
    ui->comboBoxFrameShadow->addItem("Sunken");

    // 设置默认选择 Raised（凸起样式）
    ui->comboBoxFrameShadow->setCurrentIndex(1);
}

// 设置背景为“无颜色”
void Widget::on_radioButtonNull_clicked() {
    ui->frameTest->setStyleSheet(""); // 清除样式表
}

// 设置背景为红色
void Widget::on_radioButtonRed_clicked() {
    ui->frameTest->setStyleSheet("background-color: rgb(255, 0, 0);");
}

// 设置背景为蓝色
void Widget::on_radioButtonBlue_clicked() {
    ui->frameTest->setStyleSheet("background-color: rgb(0, 0, 255);");
}

// 设置背景为绿色
void Widget::on_radioButtonGreen_clicked() {
    ui->frameTest->setStyleSheet("background-color: rgb(0, 255, 0);");
}

// 响应 Line Width 数值变化，更新 frame 边框宽度
void Widget::on_spinBoxLineWidth_valueChanged(int arg1) {
    ui->frameTest->setLineWidth(arg1);
}

// 响应 Mid Line Width 数值变化，更新 frame 中线宽度
void Widget::on_spinBoxMidLineWidth_valueChanged(int arg1) {
    ui->frameTest->setMidLineWidth(arg1);
}

// 当选择不同 Frame Shape（形状）时，更新 frameTest 的形状样式
void Widget::on_comboBoxFrameShape_currentIndexChanged(int index) {
    if (index < 0) return;
    ui->frameTest->setFrameShape(m_listFrameShape[index]);
}

// 当选择不同 Frame Shadow（阴影）时，更新 frameTest 的阴影样式
void Widget::on_comboBoxFrameShadow_currentIndexChanged(int index) {
    if (index < 0) return;
    ui->frameTest->setFrameShadow(m_listFrameShadow[index]);
}
