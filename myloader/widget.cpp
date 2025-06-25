#include "widget.h"
#include <QDebug>
#include <QFile>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtUiTools/QUiLoader>

Widget::Widget(QWidget *parent) : QWidget(parent) {
    // 加载 .ui 文件
    QUiLoader loader;
    QFile file(":/widget.ui"); // 使用 qrc 资源路径或直接写文件路径
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Failed to open .ui file";
        return;
    }

    QWidget *form = loader.load(&file, this);
    file.close();

    if (!form) {
        qWarning() << "Failed to load .ui";
        return;
    }

    // 使用 layout 包装
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(form);
    setLayout(layout);

    // 连接信号槽
    QPushButton *btn = form->findChild<QPushButton *>("pushButton");
    if (btn) {
        connect(btn, &QPushButton::clicked, [] { qDebug() << "Button clicked!"; });
    }
}
