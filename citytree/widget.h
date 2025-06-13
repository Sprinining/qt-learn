#ifndef WIDGET_H
#define WIDGET_H

#include <QTreeWidget>     //树形控件头文件
#include <QTreeWidgetItem> //条目类的头文件
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
    void on_btnAddTop_clicked();

    void on_btnAddChild_clicked();

    void on_btnDelLeaf_clicked();

    void on_btnDelSubtree_clicked();

  private:
    Ui::Widget* ui;
    void removeSubtree(QTreeWidgetItem* curLevelItem);
};
#endif // WIDGET_H
