#include "rotateimagedialog.h"
#include "ui_rotateimagedialog.h"

// 构造函数：初始化 UI 并进行控件设置
RotateImageDialog::RotateImageDialog(QWidget* parent) : QDialog(parent), ui(new Ui::RotateImageDialog) {
    ui->setupUi(this); // 加载 UI 布局
    init();            // 自定义初始化
}

// 析构函数：释放 UI 对象资源
RotateImageDialog::~RotateImageDialog() {
    delete ui;
}

// 初始化函数：设置角度选择框和窗口标题
void RotateImageDialog::init() {
    // 设置旋转角度选择框的数值范围为 0~360 度
    ui->spinBoxAngle->setRange(0, 360);

    // 设置 spinBox 的后缀为“°”，表示单位为度
    ui->spinBoxAngle->setSuffix(tr("°"));

    // 设置窗口标题
    setWindowTitle(tr("旋转图片"));
}

// “确定旋转”按钮点击事件处理函数
void RotateImageDialog::on_pushButtonRotating_clicked() {
    // 获取用户设置的角度值
    int nAngle = ui->spinBoxAngle->value();
    // 调用 done() 结束对话框并返回角度值，供主窗口通过 exec() 获取
    done(nAngle);
}
