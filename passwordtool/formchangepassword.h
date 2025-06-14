#ifndef FORMCHANGEPASSWORD_H
#define FORMCHANGEPASSWORD_H

#include <QCryptographicHash> // 用于计算密码的哈希值
#include <QWidget>            // QWidget 是所有窗口部件的基类

// 前向声明 UI 命名空间中的 FormChangePassword 类（由 Qt Designer 生成）
namespace Ui {
class FormChangePassword;
}

/**
 * @brief FormChangePassword 是一个用于修改密码的独立窗口类
 *
 * 该窗口接收旧密码的哈希值，用户输入新密码后进行验证并发送新密码哈希值。
 */
class FormChangePassword : public QWidget {
    Q_OBJECT // 启用 Qt 的元对象系统（支持信号和槽）

  public :
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为 nullptr（独立窗口）
     */
    explicit FormChangePassword(QWidget* parent = nullptr);

    /// 析构函数，自动释放 UI 指针
    ~FormChangePassword();

  signals:
    /**
     * @brief 向主窗口发送新的密码哈希
     * @param strUser 用户名
     * @param baNewHash 新密码的哈希值（使用 QByteArray 存储）
     */
    void sendNewUserPassword(QString strUser, QByteArray baNewHash);

  private slots:
    /**
     * @brief 当“修改密码”按钮被点击时执行的槽函数
     * 用于检查旧密码是否正确，并发送新密码哈希。
     */
    void on_pushButtonChange_clicked();

  public slots:
    /**
     * @brief 接收旧密码哈希值的槽函数（由主窗口调用）
     * @param strUser 用户名
     * @param baOldHash 旧密码的哈希值
     */
    void recvOldUserPassword(QString strUser, QByteArray baOldHash);

  private:
    /// 指向 UI 界面对象的指针
    Ui::FormChangePassword* ui;

    /// 初始化窗口的一些默认设置（如标题、提示等）
    void init();

    /// 保存当前用户名
    QString m_strUser;

    /// 保存旧密码的哈希值，用于验证用户输入的旧密码是否正确
    QByteArray m_baOldHash;
};

#endif // FORMCHANGEPASSWORD_H
