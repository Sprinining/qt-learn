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
    void on_btnOpen_clicked();
    void on_textBrowser_backwardAvailable(bool arg1);
    void on_textBrowser_forwardAvailable(bool arg1);
    void on_textBrowser_textChanged();

  private:
    Ui::Widget* ui;
    void saveSettings();
    void loadSettings();

    // QWidget interface
  protected:
    void closeEvent(QCloseEvent* event) override;
};
#endif // WIDGET_H
