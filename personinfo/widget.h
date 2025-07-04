#ifndef WIDGET_H
#define WIDGET_H

#include <QTableWidgetItem>
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
    void on_tableWidget_currentItemChanged(QTableWidgetItem* current, QTableWidgetItem* previous);

    void on_btnAdd_clicked();

    void on_btnDel_clicked();

  private:
    Ui::Widget* ui;
};
#endif // WIDGET_H
