#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>

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
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_listWidget_itemChanged(QListWidgetItem *item);
    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_btnViewMode_clicked();
    void on_btnLoad_clicked();
    void on_btnSave_clicked();
    void on_btnEditToolTip_clicked();

  private:
    Ui::Widget* ui;
};
#endif // WIDGET_H
