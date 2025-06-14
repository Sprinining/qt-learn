#include "resizeimagedialog.h"
#include "ui_resizeimagedialog.h"

// 构造函数：初始化 UI，并调用自定义初始化函数
ResizeImageDialog::ResizeImageDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ResizeImageDialog) {
    ui->setupUi(this); // 绑定 UI 组件
    init();            // 执行自定义初始化
}

// 析构函数：释放 UI 对象资源
ResizeImageDialog::~ResizeImageDialog() {
    delete ui;
}

// 初始化函数：设置界面控件属性
void ResizeImageDialog::init() {
    // 设置旧尺寸输入框为只读，不允许用户修改
    ui->lineEditOldSize->setReadOnly(true);
    ui->lineEditOldSize->setStyleSheet("background-color: lightgray;");

    // 设置新的宽度和高度输入范围，防止用户输入非法值
    ui->spinBoxWidthNew->setRange(1, 10000);
    ui->spinBoxHeightNew->setRange(1, 10000);

    // 设置窗口标题
    setWindowTitle(tr("缩放图片尺寸"));
}

// 接收来自主窗口的旧图片尺寸并显示，同时设置默认新尺寸
void ResizeImageDialog::recvOldSize(int nOldWidth, int nOldHeight) {
    // 构造旧尺寸字符串，如 "800 X 600"
    QString strOldSize = tr("%1 X %2").arg(nOldWidth).arg(nOldHeight);
    ui->lineEditOldSize->setText(strOldSize); // 显示旧尺寸

    // 将新尺寸的 spinBox 默认值设置为当前尺寸
    ui->spinBoxWidthNew->setValue(nOldWidth);
    ui->spinBoxHeightNew->setValue(nOldHeight);
}

// 点击“确定设置新尺寸”按钮时的槽函数
void ResizeImageDialog::on_pushButtonSetNewSize_clicked() {
    // 获取用户输入的新宽度和高度
    int nNewWidth = ui->spinBoxWidthNew->value();
    int nNewHeight = ui->spinBoxHeightNew->value();

    // 发射信号，将新尺寸传回主窗口进行图片缩放
    emit sendNewSize(nNewWidth, nNewHeight);

    // 更新旧尺寸框显示为新的尺寸值，供用户参考
    QString strSize = tr("%1 X %2").arg(nNewWidth).arg(nNewHeight);
    ui->lineEditOldSize->setText(strSize);
}
