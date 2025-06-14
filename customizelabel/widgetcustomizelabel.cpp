#include "widgetcustomizelabel.h"
#include "./ui_widgetcustomizelabel.h"
#include <QDebug>

// 构造函数
WidgetCustomizeLabel::WidgetCustomizeLabel(QWidget* parent) : QWidget(parent), ui(new Ui::WidgetCustomizeLabel) {
    ui->setupUi(this); // 初始化 UI
    init();            // 初始化逻辑
}

// 析构函数，释放资源
WidgetCustomizeLabel::~WidgetCustomizeLabel() {
    delete m_pDlgForeground; // 释放前景色对话框资源
    m_pDlgForeground = nullptr;
    delete ui;
}

// 初始化函数
void WidgetCustomizeLabel::init() {
    // 设置默认背景色为浅灰色，前景色为黑色
    m_clrBackground = QColor(240, 240, 240);
    m_clrForeground = QColor(0, 0, 0);
    m_strText = tr("显示样例"); // 默认显示文本

    // 创建颜色选择对话框，用于选择前景色
    m_pDlgForeground = new QColorDialog(this);
    m_pDlgForeground->setOptions(QColorDialog::NoButtons); // 去除对话框按钮
    m_pDlgForeground->setModal(false);                     // 设置为非模态（不阻塞主窗口）

    // 当颜色发生变化时，更新标签前景色
    connect(m_pDlgForeground, &QColorDialog::currentColorChanged, this, &WidgetCustomizeLabel::recvAndSetForegroundColor);
}

// 点击“前景色”按钮，弹出颜色选择对话框
void WidgetCustomizeLabel::on_pushButtonForeground_clicked() {
    m_pDlgForeground->show();  // 显示对话框
    m_pDlgForeground->raise(); // 提升窗口层级，防止被遮挡
}

// 点击“背景色”按钮，使用静态函数获取颜色
void WidgetCustomizeLabel::on_pushButtonBackground_clicked() {
    QColor clr = QColorDialog::getColor(); // 弹出颜色选择框
    if (!clr.isValid()) return;            // 用户取消则不处理

    m_clrBackground = clr; // 保存新背景色

    // 设置 label 的样式表（前景 + 背景）
    QString strQSS = tr("color: %1; background-color: %2;").arg(m_clrForeground.name()).arg(m_clrBackground.name());
    ui->labelSample->setStyleSheet(strQSS);
}

// 点击“字体”按钮，弹出字体选择对话框
void WidgetCustomizeLabel::on_pushButtonFont_clicked() {
    bool bOK = false;
    QFont ft = QFontDialog::getFont(&bOK, m_font); // 获取字体
    if (!bOK) return;                              // 用户取消则不处理

    m_font = ft;                      // 保存新字体
    ui->labelSample->setFont(m_font); // 应用新字体到 label
}

// 点击“文本”按钮，弹出多行文本输入框
void WidgetCustomizeLabel::on_pushButtonText_clicked() {
    bool bOK = false;
    QString strText = QInputDialog::getMultiLineText(this, tr("设置文本"), tr("请输入文本："), m_strText, &bOK);
    if (!bOK) return; // 用户取消则不处理

    m_strText = strText;                 // 保存文本
    ui->labelSample->setText(m_strText); // 显示新文本
}

// 槽函数：接收前景色并更新 label 样式
void WidgetCustomizeLabel::recvAndSetForegroundColor(QColor color) {
    if (!color.isValid()) return; // 判断颜色合法性

    m_clrForeground = color; // 保存新前景色

    // 应用前景+背景样式
    QString strQSS = tr("color: %1; background-color: %2;").arg(m_clrForeground.name()).arg(m_clrBackground.name());
    ui->labelSample->setStyleSheet(strQSS);
}
