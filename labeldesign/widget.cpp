#include "widget.h"
#include "./ui_widget.h"
#include <QColor>
#include <QDebug>
#include <QFont>
#include <QMessageBox>

// 构造函数，初始化界面和控件
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this); // 加载 UI 文件生成的界面
    initControls();    // 初始化控件配置
}

// 析构函数，释放 UI 对象内存
Widget::~Widget() {
    delete ui;
}

// 初始化控件属性和样式
void Widget::initControls() {
    // 设置字号旋钮框的范围为 4 到 100
    ui->spinBoxSize->setRange(4, 100);
    // 默认字号设置为 9
    ui->spinBoxSize->setValue(9);

    // 获取所有可用颜色名称列表（Qt内置的标准颜色）
    QStringList colorNames = QColor::colorNames();

    // 将颜色名称添加到前景色组合框
    ui->comboBoxFGColor->addItems(colorNames);
    // 设置默认前景色为黑色
    ui->comboBoxFGColor->setCurrentText("black");

    // 将颜色名称添加到背景色组合框
    ui->comboBoxBGColor->addItems(colorNames);
    // 设置默认背景色为浅灰色
    ui->comboBoxBGColor->setCurrentText("lightgray");

    // 设置 ToolBox 组件的样式表
    // ::tab 选择器修改工具箱标签页的背景色为紫红色（magenta）
    // 给特定页面 QWidget 指定名字，设置不同的背景色以便区分
    QString strCSS = "::tab{ background-color: magenta; }"
                     "QWidget#pageText{ background-color: green; }"
                     "QWidget#pageFont{ background-color: cyan; }"
                     "QWidget#pageColor{ background-color: yellow; }";
    ui->toolBox->setStyleSheet(strCSS);
}

// 点击编辑按钮时，将输入框的文本设置到显示标签上
void Widget::on_pushButtonEditText_clicked() {
    QString strText = ui->lineEditText->text(); // 获取输入框内容
    ui->labelShow->setText(strText);            // 设置标签显示内容
}

// 字体选择框内容改变时，更新标签的字体（字体名 + 当前字号）
void Widget::on_fontComboBox_currentTextChanged(const QString& arg1) {
    QFont txtFont(arg1, ui->spinBoxSize->value()); // 创建字体，字号用旋钮框当前值
    ui->labelShow->setFont(txtFont);               // 应用字体到标签
}

// 字号旋钮框数值改变时，更新标签字体（当前字体名 + 新字号）
void Widget::on_spinBoxSize_valueChanged(int arg1) {
    QFont txtFont(ui->fontComboBox->currentText(), arg1); // 创建字体，字号用旋钮框新值
    ui->labelShow->setFont(txtFont);                      // 应用字体到标签
}

// 当前景色组合框的文本改变时，更新标签的前景色和背景色样式
void Widget::on_comboBoxFGColor_currentTextChanged(const QString& arg1) {
    QString strFGColor = arg1;                               // 新前景色
    QString strBGColor = ui->comboBoxBGColor->currentText(); // 当前背景色
    // 拼接样式表字符串，设置字体颜色和背景颜色
    QString strCSS = QString("color: %1; background-color: %2;").arg(strFGColor).arg(strBGColor);
    ui->labelShow->setStyleSheet(strCSS); // 应用样式
}

// 当背景色组合框文本改变时，更新标签的背景色和前景色样式
void Widget::on_comboBoxBGColor_currentTextChanged(const QString& arg1) {
    QString strFGColor = ui->comboBoxFGColor->currentText(); // 当前前景色
    QString strBGColor = arg1;                               // 新背景色
    // 拼接样式表字符串，设置字体颜色和背景颜色
    QString strCSS = QString("color: %1; background-color: %2;").arg(strFGColor).arg(strBGColor);
    ui->labelShow->setStyleSheet(strCSS); // 应用样式
}
