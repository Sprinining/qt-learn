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

public slots:
    // 自定义的槽函数
    void printText(const QString& text);

private:
    Ui::Widget* ui;
};
#endif // WIDGET_H
