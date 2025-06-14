#ifndef WIDGETCALCMD5_H
#define WIDGETCALCMD5_H

// 用于计算 MD5 值
#include <QCryptographicHash>
// 用于显示错误信息弹窗
#include <QErrorMessage>
// 用于文件操作
#include <QFile>
// 用于弹出文件选择对话框
#include <QFileDialog>
// 用于显示进度对话框
#include <QProgressDialog>
// Qt 窗口基类
#include <QWidget>

// 命名空间 Ui，自动由 Qt Designer 生成，用于访问界面控件
QT_BEGIN_NAMESPACE
namespace Ui {
class WidgetCalcMD5;
}
QT_END_NAMESPACE

// 继承自 QWidget 的主窗口类，用于实现文件 MD5 值计算功能
class WidgetCalcMD5 : public QWidget {
    Q_OBJECT

  public:
    // 构造函数，父对象默认为空
    WidgetCalcMD5(QWidget* parent = nullptr);

    // 析构函数
    ~WidgetCalcMD5();

  private slots:
    // 槽函数：当“浏览文件”按钮被点击时执行
    void on_pushButtonBrowser_clicked();

    // 槽函数：当“计算 MD5”按钮被点击时执行
    void on_pushButtonCalcMD5_clicked();

  private:
    // 指向 UI 界面对象的指针，用于访问界面上的控件
    Ui::WidgetCalcMD5* ui;

    // 错误提示对话框，用于统一显示错误信息
    QErrorMessage m_dlgErrorMsg;

    // 当前选择的文件名（含完整路径）
    QString m_strFileName;

    // 计算传入文件的 MD5 值
    // 参数：
    //   fileIn：输入文件对象（已打开）
    //   nFileSize：文件大小（用于显示进度）
    // 返回值：
    //   计算得到的 MD5 值（16 字节的 QByteArray）
    QByteArray calcFileMD5(QFile& fileIn, qint64 nFileSize);
};

#endif // WIDGETCALCMD5_H
