#include "widget.h"      // 包含自定义窗口类的头文件
#include "./ui_widget.h" // 包含由 uic 工具生成的 UI 类定义（ui->setupUi 会用到）
#include <QDebug>        // 用于调试输出
#include <QIcon>         // 用于设置图标
#include <QMessageBox>   // 弹出提示框
#include <QPixmap>       // 图标图像处理类

// 构造函数，初始化 UI
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this); // 构建 UI 界面（自动连接 .ui 文件中定义的控件）

    // 默认选中“男”单选按钮
    ui->radioButtonMan->setChecked(true);

    // 向下拉框中添加图标 + 描述文字（性格特质）
    ui->comboBox->addItem(QIcon(":/images/triangle.png"), tr("锐意进取"));
    ui->comboBox->addItem(QIcon(":/images/rectangle.png"), tr("大方得体"));
    ui->comboBox->addItem(QIcon(":/images/polygon.png"), tr("灵活善变"));
    ui->comboBox->addItem(QIcon(":/images/ellipse.png"), tr("精明圆滑"));
}

// 析构函数，释放 UI 内存
Widget::~Widget() {
    delete ui;
}

// 提交按钮被点击后的槽函数（展示用户填写的信息）
void Widget::on_btnCommit_clicked() {
    QString res;

    // 添加姓名信息
    res.append(tr("姓名：%1\r\n").arg(ui->lineEditName->text()));

    // 添加性别信息
    res.append(ui->radioButtonMan->isChecked() ? tr("性别：男\r\n") : tr("性别：女\r\n"));

    // 添加性格特质信息
    res.append(tr("性格特质：%1\r\n").arg(ui->comboBox->currentText()));

    // 获取当前选中项对应的图标
    int index = ui->comboBox->currentIndex();    // 当前选中项索引
    QIcon icon = ui->comboBox->itemIcon(index);  // 获取该项的图标
    QPixmap pixmap = icon.pixmap(QSize(32, 32)); // 转换为指定大小的 Pixmap 图像

    // 构建消息框并显示结果
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("人员信息"));      // 标题
    msgBox.setText(res);                        // 文本内容
    msgBox.setStandardButtons(QMessageBox::Ok); // 只有“确定”按钮
    msgBox.setIconPixmap(pixmap);               // 设置图标（性格特质图标）
    msgBox.exec();                              // 显示消息框（阻塞）
}

// 取消按钮被点击后的槽函数（提示并退出）
void Widget::on_btnCancel_clicked() {
    // 弹出确认框，询问用户是否退出
    int ret = QMessageBox::information(this,
                                       tr("退出"),                        // 标题
                                       tr("您确定要退出程序吗？"),        // 内容
                                       QMessageBox::Yes | QMessageBox::No // 提供“是/否”选项
                                       );

    // 如果用户选择“是”，则关闭窗口（退出程序）
    if (QMessageBox::Yes == ret) this->close();
}
