#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "formchangepassword.h" // 子窗口类，用于修改用户密码
#include <QCryptographicHash>   // 用于生成密码的哈希值
#include <QListWidget>          // UI 中的列表控件
#include <QListWidgetItem>      // 列表中的每一项
#include <QMap>                 // 用于保存用户名与密码哈希值的映射
#include <QWidget>              // QWidget 是所有窗口部件的基类

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

/**
 * @brief MainWidget 是应用的主窗口类，提供用户管理界面
 *
 * 功能包括添加用户、删除用户、修改用户密码，并将用户信息存储在 QMap 中。
 */
class MainWidget : public QWidget {
    Q_OBJECT // 启用 Qt 的元对象系统（支持信号和槽）

  public :
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为 nullptr
     */
    MainWidget(QWidget* parent = nullptr);

    /// 析构函数，自动释放资源
    ~MainWidget();

    /**
     * @brief 刷新用户列表显示
     * 将当前 m_mapUserAndHash 中的用户显示到 QListWidget 中
     */
    void updateListShow();

  signals:
    /**
     * @brief 向子窗口发送当前用户名及旧密码哈希
     * 用于修改密码时子窗口进行旧密码验证
     */
    void sendOldUserPassword(QString strUser, QByteArray baOldHash);

  private slots:
    /**
     * @brief 添加用户按钮点击处理函数
     * 获取用户名和密码，生成哈希并存入映射表
     */
    void on_pushButtonAddUser_clicked();

    /**
     * @brief 修改密码按钮点击处理函数
     * 弹出子窗口并发送当前用户信息
     */
    void on_pushButtonChangePassword_clicked();

    /**
     * @brief 删除用户按钮点击处理函数
     * 从映射表中移除选中的用户
     */
    void on_pushButtonDelUser_clicked();

  public slots:
    /**
     * @brief 接收来自子窗口的新密码哈希
     * @param strUser 用户名
     * @param baNewHash 新的密码哈希值
     * 替换 m_mapUserAndHash 中该用户的密码
     */
    void recvNewUserPassword(QString strUser, QByteArray baNewHash);

  private:
    /// UI 指针，管理由 Qt Designer 创建的界面元素
    Ui::MainWidget* ui;

    /// 保存所有用户名和其对应密码哈希的映射表
    QMap<QString, QByteArray> m_mapUserAndHash;

    /// 指向修改密码子窗口的指针
    FormChangePassword* m_pFormChild;

    /**
     * @brief 初始化主窗口设置
     * 包括界面初始化、信号槽连接等
     */
    void init();
};

#endif // MAINWIDGET_H
