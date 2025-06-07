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

signals: // 信号默认强制规定为公有类型，这样才能保证其他对象能接收到信号
    void sendMsg(QString str); // 信号只需要声明，而不要写实体代码

public slots:
    void buttonClicked();

private:
    Ui::Widget* ui;
};
#endif // WIDGET_H
