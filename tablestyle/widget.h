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
    void on_pushButtonAlternatingRowColors_clicked();

    void on_pushButtonSelectionCustom_clicked();

    void on_pushButtonItemCustom_clicked();

    void on_pushButtonCornerButtonCustom_clicked();

    void on_pushButtonHeaderCustom_clicked();

    void on_pushButtonClearStyle_clicked();

  private:
    Ui::Widget* ui;
};
#endif // WIDGET_H
