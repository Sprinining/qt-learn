#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

private slots:
    void on_loginBtn_clicked();
    void on_exitBtn_clicked();

private:
    Ui::Widget* ui;
    // 用户名字符串
    QString name;
    // 存储密码 hash 值
    QByteArray pwdHash;
};
#endif // WIDGET_H
