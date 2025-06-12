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
    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_btnExportM3U_clicked();
    void on_btnFind_clicked();
    void on_checkBoxAutoSort_clicked(bool checked);
    void on_checkBoxReverse_clicked(bool checked);

  private:
    Ui::Widget* ui;
};
#endif // WIDGET_H
