#ifndef WIDGET_H
#define WIDGET_H

#include "calcpi.h"
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
    void on_btnCalcPI_clicked();

    void on_btnCalcAll_clicked();

  private:
    Ui::Widget* ui;
    // 用于计算pi的类对象
    CalcPI m_calcPI;
    // 获取单选按钮对应的PI长度
    int getPILength();
};
#endif // WIDGET_H
