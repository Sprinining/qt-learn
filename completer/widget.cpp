#include "widget.h"
#include "./ui_widget.h"
#include <QCompleter>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // 创建星期几列表
    QStringList listDayOfWeek;
    listDayOfWeek << "Monday" << "Tuesday" << "Wednesday"
                  << "Thursday" << "Friday" << "Saturday" << "Sunday";

    // 创建星期几自动补全器，设置为弹出匹配列表（PopupCompletion），默认模式
    QCompleter* cpDayOfWeek = new QCompleter(listDayOfWeek);
    cpDayOfWeek->setCaseSensitivity(Qt::CaseInsensitive);  // 不区分大小写
    cpDayOfWeek->setCompletionMode(QCompleter::PopupCompletion);  // 弹出匹配列表
    ui->edtDay->setCompleter(cpDayOfWeek);  // 设置到对应的编辑框

    // 创建年份列表
    QStringList listYear;
    listYear << "2016" << "2015"
             << "2008" << "2006"
             << "1999" << "1991";

    listYear.sort();  // 排序，方便补全匹配

    // 创建年份自动补全器，设置为内联补全（InlineCompletion）
    QCompleter* cpYear = new QCompleter(listYear);
    cpYear->setCompletionMode(QCompleter::InlineCompletion);  // 在编辑框内自动补全剩余部分
    ui->edtYear->setCompleter(cpYear);

    // 创建卡萨相关字符串列表
    QStringList listHeXi;
    listHeXi << "卡萨甲" << "卡萨乙" << "卡萨丙" << "卡萨丁"
             << "卡萨甲2" << "卡萨乙3" << "卡萨乙2" << "卡萨丁2";

    // 创建卡萨补全器，设置为不过滤，弹出所有项（UnfilteredPopupCompletion）
    QCompleter* cpHexi = new QCompleter(listHeXi);
    cpHexi->setCompletionMode(QCompleter::UnfilteredPopupCompletion);  // 弹出全部项，不过滤匹配
    ui->edtKasa->setCompleter(cpHexi);

    QFrame *frame = new QFrame(this);
    frame->setFrameStyle(QFrame::Box | QFrame::Sunken);
    frame->setLineWidth(2);
    frame->setMidLineWidth(1);
    frame->setFixedSize(200, 100);

    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);  // 水平线
    line->setFrameShadow(QFrame::Raised);
}

Widget::~Widget() {
    delete ui;
}

// 以下槽函数用于接收编辑框文本变化的信号，打印当前文本内容方便调试

void Widget::on_edtDay_textChanged(const QString& arg1) {
    qDebug() << "星期几: " << arg1;
}

void Widget::on_edtYear_textChanged(const QString& arg1) {
    qDebug() << "哪一年: " << arg1;
}

void Widget::on_edtKasa_textChanged(const QString& arg1) {
    qDebug() << "卡萨什么: " << arg1;
}
