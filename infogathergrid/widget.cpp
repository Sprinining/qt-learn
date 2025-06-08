#include "widget.h"
#include "./ui_widget.h"
#include <QDateTime>
#include <QMessageBox>

// 构造函数：初始化 UI 和控件属性
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this); // 设置 UI 组件

    // 设置职业下拉框可编辑
    ui->comboBoxJob->setEditable(true);

    // 设置日期选择器为弹出日历形式
    ui->dateEdit->setCalendarPopup(true);
}

// 析构函数：释放 UI 资源
Widget::~Widget() {
    delete ui;
}

// 槽函数：当性别下拉框选择变化时触发
void Widget::on_comboBoxGender_currentIndexChanged(int index) {
    if (index < 0) return;                                       // 无效索引，直接返回
    qDebug() << "性别：" << ui->comboBoxGender->itemText(index); // 打印当前选择的性别
}

// 槽函数：当职业下拉框文字变化时触发
void Widget::on_comboBoxJob_currentTextChanged(const QString& arg1) {
    qDebug() << "职业：" << arg1; // 打印当前职业
}

// 槽函数：当生日日期选择变化时触发
void Widget::on_dateEdit_dateChanged(const QDate& date) {
    qDebug() << date.toString("yyyy-MM-dd"); // 打印新选择的日期
}

// 槽函数：当点击“提交”按钮时触发
void Widget::on_pushButton_clicked() {
    QString res;

    // 拼接用户输入的信息
    res.append(tr("姓名：%1\r\n").arg(ui->lineEdtName->text()));
    res.append(tr("性别：%1\r\n").arg(ui->comboBoxGender->currentText()));
    res.append(tr("职业：%1\r\n").arg(ui->comboBoxJob->currentText()));
    res.append(tr("生日：%1\r\n").arg(ui->dateEdit->date().toString()));

    // 计算年龄（仅按年份粗略估算）
    QDateTime dtCur = QDateTime::currentDateTime();
    int years = dtCur.date().year() - ui->dateEdit->date().year();
    res.append(tr("年龄：%1").arg(years));

    // 弹出消息框显示用户信息
    QMessageBox::information(this, tr("信息"), res);
}
