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
    void on_btnLoad_clicked();

    void on_btnSave_clicked();

    void on_btnDelRow_clicked();

    void on_btnAddRow_clicked();

    void on_listWidget_currentRowChanged(int currentRow);

  private:
    Ui::Widget* ui;
};
#endif // WIDGET_H
