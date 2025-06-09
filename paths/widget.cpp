#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

// 构造函数：初始化 UI，设置文本框为只读
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    ui->plainTextEditResult->setReadOnly(true); // 结果显示区域不允许手动编辑
}

// 析构函数：释放 UI 对象
Widget::~Widget() {
    delete ui;
}

// 获取当前工作路径（工作目录），显示到 lineEditWorkPath 控件中
void Widget::on_btnGetWP_clicked() {
    QString strWorkPath = QDir::currentPath();  // 获取当前工作路径
    ui->lineEditWorkPath->setText(strWorkPath); // 显示路径
}

// 设置当前工作路径为 lineEditWorkPath 中输入的路径
void Widget::on_btnSetWP_clicked() {
    QString strNewPath = ui->lineEditWorkPath->text(); // 获取用户输入路径
    if (strNewPath.length() < 1) return;               // 如果输入为空，则不处理

    QDir dirNew(strNewPath);
    QString strResult;

    if (dirNew.exists()) {                           // 如果路径存在
        bool bRes = QDir::setCurrent(dirNew.path()); // 尝试设置为当前工作路径
        if (!bRes) {
            strResult = tr("设置工作路径为 %1 失败。").arg(strNewPath);
            QMessageBox::warning(this, tr("设置错误"), strResult); // 设置失败时弹出警告
        } else {
            strResult = tr("设置工作路径成功，新的路径为：\r\n%1").arg(QDir::currentPath());
        }
    } else {
        // 如果路径不存在，弹出错误提示
        strResult = tr("设置工作路径为 %1 失败，该路径不存在！").arg(strNewPath);
        QMessageBox::warning(this, tr("路径不存在"), strResult);
    }

    ui->plainTextEditResult->setPlainText(strResult); // 显示操作结果
}

// 获取 Qt 提供的几个常用路径信息：工作目录、可执行文件目录、用户主目录、根目录、临时目录
void Widget::on_btnGetEP_clicked() {
    QString strWorkPath = QDir::currentPath();                   // 当前工作路径
    QString strAppPath = QCoreApplication::applicationDirPath(); // 可执行程序所在目录
    QString strHomePath = QDir::homePath();                      // 用户主目录
    QString strRootPath = QDir::rootPath();                      // 根目录（Windows 是 C:/，Linux 是 /）
    QString strTempPath = QDir::tempPath();                      // 临时目录路径

    QString strResult;
    strResult += tr("工作路径：%1\r\n").arg(strWorkPath);
    strResult += tr("可执行程序目录：%1\r\n\r\n").arg(strAppPath);
    strResult += tr("用户主文件夹：%1\r\n").arg(strHomePath);
    strResult += tr("系统根目录：%1\r\n").arg(strRootPath);
    strResult += tr("临时目录：%1\r\n").arg(strTempPath);

    ui->plainTextEditResult->setPlainText(strResult); // 显示所有路径信息
}

// 判断 lineEditTestPath 中填写的路径是否存在
void Widget::on_btnExist_clicked() {
    QString strTestPath = ui->lineEditTestPath->text(); // 获取用户输入路径
    if (strTestPath.length() < 1) return;

    QDir dirWork;
    QString strResult;

    if (dirWork.exists(strTestPath)) { // 判断路径是否存在（可以是相对路径）
        strResult = tr("路径 %1 是存在的。").arg(strTestPath);
    } else {
        strResult = tr("路径 %1 不存在。").arg(strTestPath);
    }

    ui->plainTextEditResult->setPlainText(strResult); // 显示判断结果
}

// 判断 lineEditTestPath 输入的路径是相对路径还是绝对路径
void Widget::on_btnRelative_clicked() {
    QString strTestPath = ui->lineEditTestPath->text(); // 获取路径
    if (strTestPath.length() < 1) return;

    QDir dirTest(strTestPath);
    QString strResult;

    if (dirTest.isRelative()) { // 判断是否是相对路径
        strResult = tr("路径 %1 是相对路径").arg(strTestPath);
    } else {
        strResult = tr("路径 %1 是绝对路径").arg(strTestPath);
    }

    ui->plainTextEditResult->setPlainText(strResult); // 显示路径类型
}

// 显示 lineEditTestPath 路径的绝对路径
void Widget::on_btnAbsolute_clicked() {
    QString strTestPath = ui->lineEditTestPath->text(); // 获取路径
    if (strTestPath.length() < 1) return;

    QDir dirTest(strTestPath);
    QString strResult;

    // 获取该路径的绝对路径表示形式
    strResult = tr("测试路径 %1 的绝对路径为：%2").arg(strTestPath).arg(dirTest.absolutePath());

    ui->plainTextEditResult->setPlainText(strResult); // 显示结果
}
