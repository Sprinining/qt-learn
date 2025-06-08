#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>

// 构造函数
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // 输出 UI 中已有按钮的 sizeHint 和 minimumSizeHint
    qDebug() << tr("Preferred 按钮：") << ui->pushButtonPreferred->sizeHint() << ui->pushButtonPreferred->minimumSizeHint();
    qDebug() << tr("Expanding 按钮：") << ui->pushButtonExpanding->sizeHint() << ui->pushButtonExpanding->minimumSizeHint();

    // 创建演示不同 size policy 的小窗口
    createWidget();
}

// 析构函数
Widget::~Widget() {
    if (widget != nullptr) {
        delete widget;
        widget = nullptr;
    }
    delete ui;
}

// 创建演示窗口
void Widget::createWidget() {
    // 创建一个新窗口用于展示不同 QLineEdit 的尺寸策略行为
    widget = new QWidget(this, Qt::Window);
    widget->resize(480, 360);
    widget->setWindowTitle(tr("单行编辑器的布局"));

    // 使用垂直布局作为主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(widget);

    // -------- 第一行：Fixed 和 Preferred --------
    QLineEdit* leFixed = new QLineEdit(widget);
    leFixed->setText(tr("Fixed"));

    // 设置为 Fixed，表示：不参与布局器的尺寸调整，保持原大小
    QSizePolicy sp = leFixed->sizePolicy();
    sp.setHorizontalPolicy(QSizePolicy::Fixed);
    leFixed->setSizePolicy(sp);

    QLineEdit* lePreferred = new QLineEdit(widget);
    lePreferred->setText(tr("Preferred"));

    // 设置为 Preferred，表示：布局器会参考控件的 sizeHint，但在空间足够时允许变大
    sp = lePreferred->sizePolicy();
    sp.setHorizontalPolicy(QSizePolicy::Preferred);
    lePreferred->setSizePolicy(sp);

    // 水平布局，加入两个控件
    QHBoxLayout* lay1 = new QHBoxLayout();
    lay1->addWidget(leFixed);     // 尺寸固定
    lay1->addWidget(lePreferred); // 尺寸可拉伸（被动）

    mainLayout->addLayout(lay1);

    // -------- 第二行：Preferred 与 Minimum --------
    QLineEdit* lePreferred2 = new QLineEdit(widget);
    lePreferred2->setText(tr("Preferred2"));

    sp = lePreferred->sizePolicy();
    sp.setHorizontalPolicy(QSizePolicy::Preferred);
    lePreferred2->setSizePolicy(sp);

    QLineEdit* leMinimum = new QLineEdit(widget);
    leMinimum->setText(tr("Minimum"));

    // 设置为 Minimum：优先保持 minimumSizeHint，但允许空间不足时被压缩
    sp = leMinimum->sizePolicy();
    sp.setHorizontalPolicy(QSizePolicy::Minimum);
    leMinimum->setSizePolicy(sp);

    QHBoxLayout* lay2 = new QHBoxLayout();
    lay2->addWidget(lePreferred2); // 可以被动拉伸
    lay2->addWidget(leMinimum);    // 更倾向于压缩至最小

    mainLayout->addLayout(lay2);

    // -------- 第三行：Minimum 与 Expanding --------
    QLineEdit* leMinimum2 = new QLineEdit(widget);
    leMinimum2->setText(tr("Minimum2"));

    sp = leMinimum2->sizePolicy();
    sp.setHorizontalPolicy(QSizePolicy::Minimum);
    leMinimum2->setSizePolicy(sp);

    QLineEdit* leExpanding = new QLineEdit(widget);
    leExpanding->setText(tr("Expanding"));

    // 设置为 Expanding：主动参与布局器的空间分配，尽可能拉伸占满可用空间
    sp = leExpanding->sizePolicy();
    sp.setHorizontalPolicy(QSizePolicy::Expanding);
    leExpanding->setSizePolicy(sp);

    QHBoxLayout* lay3 = new QHBoxLayout();
    lay3->addWidget(leMinimum2);  // 保持最小但可以拉伸
    lay3->addWidget(leExpanding); // 会尽量抢占剩余空间

    mainLayout->addLayout(lay3);

    // 设置主布局
    widget->setLayout(mainLayout);

    // 输出各控件的建议尺寸
    qDebug() << tr("Fixed 编辑器建议尺寸：") << leFixed->sizeHint();
    qDebug() << tr("Preferred 编辑器建议尺寸：") << lePreferred->sizeHint();
    qDebug() << tr("Preferred 编辑器最小建议尺寸：") << lePreferred->minimumSizeHint();
    qDebug() << tr("Minimum 编辑器建议尺寸：") << leMinimum->sizeHint();
    qDebug() << tr("Expanding 编辑器建议尺寸：") << leExpanding->sizeHint();
    qDebug() << tr("Expanding 编辑器最小建议尺寸：") << leExpanding->minimumSizeHint();
}

// 按钮点击槽函数：弹出演示窗口
void Widget::on_pushButtonFixed_clicked() {
    if (widget != nullptr) widget->show();
}
