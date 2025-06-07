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

signals:
    void sendVoid(); // 没有参数，所以能和按钮的 clicked 信号匹配，实现信号到信号的关联

private:
    Ui::Widget* ui;
};
#endif // WIDGET_H
