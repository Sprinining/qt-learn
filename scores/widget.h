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
    void on_tableWidget_itemSelectionChanged();

    void on_btnFind_clicked();

    void on_btnAddLines_clicked();

    void on_btnDelLines_clicked();

    void on_btnDesSelectedItems_clicked();

  private:
    Ui::Widget* ui;
    // 初始化表格函数
    void InitScoresTable();
    // 设置某行的行首和四个单元格
    void SetTableRow(int nRow, QString strName, QString strChinese, QString strMath, QString strForeignLanguage, QString strFood);
};
#endif // WIDGET_H
