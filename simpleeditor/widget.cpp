#include "widget.h"
#include "./ui_widget.h"
#include <QColorDialog>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // 设置按钮为可切换状态（点击后可保持按下或弹起）
    ui->btnBold->setCheckable(true);
    ui->btnItalic->setCheckable(true);
    ui->btnUnderline->setCheckable(true);

    // 设置字号输入框为只能输入 0~72 的整数
    QIntValidator* vali = new QIntValidator(0, 72);
    ui->edtFontSize->setValidator(vali);

    // 设置默认字号为 9
    ui->edtFontSize->setText(QString::number(9));

    // 字体下拉框内容变化时设置 QTextEdit 的字体家族（用户选择字体时触发）
    connect(ui->fontBox, &QFontComboBox::currentTextChanged, ui->txtEdt, &QTextEdit::setFontFamily);

    // 初始化富文本内容（显示各种字体效果）
    ui->txtEdt->setHtml("<b>粗体字的行<br></b>"
                        "<i>斜体字的行<br></i>"
                        "<u>下划线的行<br></u>"
                        "<font style=\"color:red;\">文本前景色<br></font>"
                        "<font style=\"background:yellow;\">文字背景色<br></font>"
                        "<font style=\"font-size:18pt;\">字号大小变化的行<br></font>"
                        "<font style=\"font-family:黑体;\">字体家族变化的行<br></font>");
}

Widget::~Widget() {
    delete ui;
}

// ✔ 粗体按钮点击时触发（无论是用户点击或代码 setChecked 都会触发）
void Widget::on_btnBold_clicked(bool checked) {
    if (checked)
        ui->txtEdt->setFontWeight(QFont::Bold);
    else
        ui->txtEdt->setFontWeight(QFont::Normal);
}

// ✔ 斜体按钮点击时触发（设置当前字体是否为斜体）
void Widget::on_btnItalic_clicked(bool checked) {
    ui->txtEdt->setFontItalic(checked);
}

// ✔ 下划线按钮点击时触发（设置当前字体是否有下划线）
void Widget::on_btnUnderline_clicked(bool checked) {
    ui->txtEdt->setFontUnderline(checked);
}

// ✔ 当前槽函数在点击“前景色”按钮时触发（即按钮的 clicked() 信号）
// 功能：弹出颜色选择对话框，获取用户选取的颜色，
// 如果颜色有效，则将该颜色设置为 QTextEdit 当前选中文本（或光标处后续输入）的前景色（文字颜色）
// 同时，把该颜色同步显示在按钮的背景上，作为视觉提示
void Widget::on_btnColor_clicked() {
    // 弹出 QColorDialog 颜色选择对话框，默认颜色为黑色
    QColor clr = QColorDialog::getColor(Qt::black);

    // 判断用户是否选择了有效颜色（点击“取消”则颜色无效）
    if (clr.isValid()) {
        // 设置 QTextEdit 中当前光标所在位置（或选中文本）的前景色为用户选择的颜色
        // 如果没有选中文本，则设置的是“插入格式”，即后续输入的文字将采用该颜色
        ui->txtEdt->setTextColor(clr);

        // 构造用于设置按钮样式的字符串，比如 "color: #ff0000"
        QString str = tr("color: %1").arg(clr.name());

        // 设置按钮的样式，使按钮上显示的颜色和选中的前景色保持一致（增强交互感）
        ui->btnColor->setStyleSheet(str);
    }
}

// ✔ 背景色按钮点击时触发（弹出颜色选择对话框，设置字体背景色）
void Widget::on_btnBGColor_clicked() {
    QColor bgclr = QColorDialog::getColor(Qt::white);
    if (bgclr.isValid()) {
        ui->txtEdt->setTextBackgroundColor(bgclr);
        // 同步按钮颜色样式显示
        QString str = tr("background: %1").arg(bgclr.name());
        ui->btnBGColor->setStyleSheet(str);
    }
}

// ✔ 用户在字体大小输入框中编辑完成（按下 Enter 或焦点移出）时触发
void Widget::on_edtFontSize_editingFinished() {
    int fontSize = ui->edtFontSize->text().toInt();
    ui->txtEdt->setFontPointSize(fontSize);
}

// ✔ QTextEdit 中文本光标位置发生变化时触发（用于同步更新按钮状态和样式）
void Widget::on_txtEdt_currentCharFormatChanged(const QTextCharFormat& format) {
    // 同步加粗按钮状态
    if (format.fontWeight() == QFont::Bold)
        ui->btnBold->setChecked(true);
    else
        ui->btnBold->setChecked(false);

    // 同步斜体、下划线按钮状态
    ui->btnItalic->setChecked(format.fontItalic());
    ui->btnUnderline->setChecked(format.fontUnderline());

    // 当 QTextEdit 的光标位置变化，或选中的字符格式发生变化时，更新“前景色”按钮的样式显示，使其反映当前文字的前景色（字体颜色）
    // 获取当前字符格式中的前景色信息（即文字颜色）
    // foreground() 返回的是一个 QBrush 类型，可能包含颜色、渐变等信息
    QBrush brushText = format.foreground();

    // 判断 brushText 是否有效（非空画刷）
    // Qt::NoBrush 表示未设置颜色；此处要避免在按钮上显示无效颜色
    if (brushText != Qt::NoBrush) {
        // 提取画刷中的颜色值（QColor 类型）
        QColor clrText = brushText.color();

        // 构造用于设置按钮样式的字符串，如 "color: #ff0000"
        // 这里使用的是 CSS 样式格式，让按钮文字颜色与当前光标文本颜色一致
        QString str = tr("color: %1").arg(clrText.name());

        // 设置“前景色”按钮的样式，使其显示当前字体颜色（文字变色，增强交互体验）
        ui->btnColor->setStyleSheet(str);
    } else {
        // 如果没有设置前景色（例如默认样式），则清空按钮样式，使其恢复原状
        ui->btnColor->setStyleSheet("");
    }

    // 更新背景色按钮显示样式
    QBrush brushBG = format.background();
    if (brushBG != Qt::NoBrush) {
        QColor clrBG = brushBG.color();
        QString str = tr("background: %1").arg(clrBG.name());
        ui->btnBGColor->setStyleSheet(str);
    } else {
        ui->btnBGColor->setStyleSheet("");
    }

    // 同步字体大小到输入框
    QFont curFont = format.font();
    int fontSize = curFont.pointSize();
    if (-1 == fontSize) fontSize = (int)(curFont.pixelSize() * 9.0 / 12.0); // 兼容某些格式
    ui->edtFontSize->setText(QString::number(fontSize));

    // 同步字体家族到下拉框
    QString strFontFamily = curFont.family();
    ui->fontBox->setCurrentText(strFontFamily);
}

// ✔ QTextEdit 文本变化时触发（内容有任何编辑都会调用，主要用于调试）
void Widget::on_txtEdt_textChanged() {
    qDebug() << ui->txtEdt->toHtml() << Qt::endl;
}
