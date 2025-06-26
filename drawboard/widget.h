#pragma once

#include <QWidget>

class DrawingArea;

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);

private:
    DrawingArea *drawingArea;
};
