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

    Q_PROPERTY(QString nickName READ nickName WRITE setNickName NOTIFY nickNameChanged)
    Q_PROPERTY(int count MEMBER m_count READ count WRITE setCount NOTIFY countChanged)
    Q_PROPERTY(double value MEMBER m_value NOTIFY valueChanged)

    const QString& nickName() const;
    int count() const;

signals:
    void nickNameChanged(const QString& strNewName);
    void countChanged(int nNewCount);
    void valueChanged(double dblNewValue);

public slots:
    void setNickName(const QString& strNewName);
    void setCount(int nNewCount);

private:
    Ui::Widget* ui;
    QString m_nickName;
    int m_count;
    double m_value;
};

#endif // WIDGET_H
