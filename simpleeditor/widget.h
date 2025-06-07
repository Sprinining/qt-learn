#ifndef WIDGET_H
#define WIDGET_H

#include <QTextCharFormat> // 文本格式类
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
    void on_btnItalic_clicked(bool checked);
    void on_btnBold_clicked(bool checked);
    void on_btnUnderline_clicked(bool checked);
    void on_btnColor_clicked();
    void on_btnBGColor_clicked();
    void on_edtFontSize_editingFinished();
    void on_txtEdt_currentCharFormatChanged(const QTextCharFormat& format);
    void on_txtEdt_textChanged();

private:
    Ui::Widget* ui;
};
#endif // WIDGET_H
