#ifndef WIDGET_H
#define WIDGET_H

// 引入 Qt 中操作目录和文件的类
#include <QDir>        // QDir 提供目录的创建、遍历、路径操作等功能
#include <QFileInfo>   // QFileInfo 用于获取文件/目录的详细信息
#include <QIcon>       // QIcon 用于设置图标
#include <QListWidget> // QListWidget 是列表控件，用于显示文件列表
#include <QWidget>     // QWidget 是所有窗口部件的基类

// Qt UI 命名空间宏（防止命名冲突）
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget; // 前向声明 UI 类
}
QT_END_NAMESPACE

// Widget 类是主窗口类，继承自 QWidget
class Widget : public QWidget {
    Q_OBJECT // 启用 Qt 的元对象系统（支持信号槽）

  public :
    // 构造函数和析构函数
    Widget(QWidget* parent = nullptr); // 构造函数，默认无父窗口
    ~Widget();                           // 析构函数

  private slots:
    // 槽函数：用于处理 UI 中的信号

    // 点击“进入”按钮，跳转到输入路径
    void on_btnEnter_clicked();

    // 点击“驱动器”按钮，列出系统中的所有磁盘（如 C:\ D:\）
    void on_btnDrivers_clicked();

    // 点击“上级目录”按钮，跳转到当前目录的上一级
    void on_btnParent_clicked();

    // 双击列表项时触发（如果是文件夹，则进入）
    void on_listWidget_itemDoubleClicked(QListWidgetItem* item);

    // 当列表当前选中项发生改变时触发（用于显示文件信息等）
    void on_listWidget_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

  private:
    Ui::Widget* ui; // 自动生成的 UI 类指针，用于访问界面控件

    // 三个图标：磁盘图标、文件夹图标、文件图标
    QIcon iconDriver;
    QIcon iconFolder;
    QIcon iconFile;

    QDir dirCur; // 当前目录对象，表示用户正在浏览的目录

    // 显示指定目录中的文件和子目录
    void showItems(const QDir& dir);

    // 获取某个文件的信息（如大小、修改时间等）
    QString getFileInfo(const QFileInfo& fi);

    // 获取某个文件夹的信息（如子文件个数等）
    QString getFolderInfo(const QFileInfo& fi);

    enum ItemType {
        IDriver = QListWidgetItem::UserType + 1,
        IFolder,
        IFile
    };
};

#endif // WIDGET_H
