#include "widget.h"
#include "./ui_widget.h"
#include <QResizeEvent>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    // 设置窗口的最小尺寸，确保内容不会被挤压得太小
    // 计算最小宽度：10（左边距） + 54（标签宽度） + 10（间隙） + 75（第三行按钮宽度） + 10（右边距）
    // 计算最小高度：24（单行控件高度） * 3（行数） + 10（上下间隙） * 4（行间及边距数量）
    this->setMinimumSize(10 + 54 + 10 + 75 + 10, 24 * 3 + 10 * 4);
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_lineEdit_textEdited(const QString& arg1) {
    QFontMetrics fm = ui->btnDynamic->fontMetrics();

    // 计算文本的宽度（像素）
    int nTextWidth = fm.horizontalAdvance(arg1);
    // 获取动态按钮当前大小
    QSize szButtonDynamic = ui->btnDynamic->size();
    // 按文本宽度调整按钮宽度，增加10像素边距留白
    szButtonDynamic.setWidth(nTextWidth + 10);
    ui->btnDynamic->resize(szButtonDynamic);
    // 设置按钮文本和提示内容
    ui->btnDynamic->setText(arg1);
    ui->btnDynamic->setToolTip(arg1);

    // 固定按钮文本处理：如果文本宽度小于等于65，直接显示
    if (nTextWidth <= 65) {
        ui->btnFixed->setText(arg1);
    } else {
        // 否则截断文本并加省略号，确保按钮宽度不超过65像素
        QString strPart;
        QString strDot = "...";
        int nStrLen = arg1.length();
        int nNewTextWidth = 0;
        for (int i = 0; i < nStrLen; i++) {
            strPart += arg1[i];
            nNewTextWidth = fm.horizontalAdvance(strPart + strDot);
            if (nNewTextWidth >= 65) break;
        }
        ui->btnFixed->setText(strPart + strDot);
    }
    ui->btnFixed->setToolTip(arg1);
}

void Widget::resizeEvent(QResizeEvent* event) {
    // 获取窗口当前宽度和高度
    int W = event->size().width();
    int H = event->size().height();

    // 第一行控件布局
    // 标签1 固定大小54x24，垂直居中在窗口的1/4高度处
    int xLabel1 = 10;         // 左边距10像素
    int yLabel1 = H / 4 - 12; // 标签垂直居中，24高度减半为12
    ui->label_1->move(xLabel1, yLabel1);

    // 单行编辑控件布局
    int xLineEdit = xLabel1 + 54 + 10; // 紧邻标签右侧，间隙10像素
    int yLineEdit = yLabel1;
    int wLineEdit = W - xLineEdit - 10; // 编辑框宽度占剩余宽度，右边距10像素
    int hLineEdit = 24;                 // 高度固定24像素
    ui->lineEdit->setGeometry(xLineEdit, yLineEdit, wLineEdit, hLineEdit);

    // 第二行控件布局
    // 标签2 固定大小54x24，垂直居中在窗口的2/4高度处
    int xLabel2 = 10;
    int yLabel2 = 2 * H / 4 - 12;
    ui->label_2->move(xLabel2, yLabel2);
    // 动态按钮紧邻标签右侧，大小由槽函数动态调整
    int xButtonDynamic = xLabel2 + 54 + 10;
    int yButtonDynamic = yLabel2;
    ui->btnDynamic->move(xButtonDynamic, yButtonDynamic);

    // 第三行控件布局
    // 标签3 固定大小54x24，垂直居中在窗口的3/4高度处
    int xLabel3 = 10;
    int yLabel3 = 3 * H / 4 - 12;
    ui->label_3->move(xLabel3, yLabel3);
    // 固定按钮紧邻标签右侧，大小固定
    int xButtonFixed = xLabel3 + 54 + 10;
    int yButtonFixed = yLabel3;
    ui->btnFixed->move(xButtonFixed, yButtonFixed);
}
