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
    void on_pushButtonFixed_clicked();

  private:
    Ui::Widget* ui;
    QWidget* widget = nullptr;
    void createWidget();
};
#endif // WIDGET_H
