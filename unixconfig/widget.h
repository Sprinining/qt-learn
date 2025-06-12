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
    void on_btnBrowseSrc_clicked();

    void on_btnLoad_clicked();

    void on_btnBrowseDst_clicked();

    void on_btnSave_clicked();

  private:
    Ui::Widget* ui;
    void AnalyzeOneLine(const QByteArray& baLine);
};
#endif // WIDGET_H
