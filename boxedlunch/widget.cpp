#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QSlider>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    ui->comboBox->addItem(tr("番茄鸡蛋"), 8.50);
    ui->comboBox->addItem(tr("土豆烧肉"), 10.00);
    ui->comboBox->addItem(tr("鱼香肉丝"), 10.00);
    ui->comboBox->addItem(tr("青椒鸡蛋"), 8.50);
    ui->comboBox->addItem(tr("地三鲜"), 9.00);

    ui->doubleSpinBoxPrice->setRange(0.00, 30.00);
    ui->doubleSpinBoxPrice->setSingleStep(1.00);
    ui->doubleSpinBoxPrice->setSuffix(tr(" 元"));

    ui->spinBoxCount->setRange(0, 100);
    ui->spinBoxCount->setSingleStep(1);
    ui->horizontalSlider->setRange(0, 100);
    ui->horizontalSlider->setSingleStep(1);

    ui->horizontalSlider->setTickPosition(QSlider::TicksBothSides);
    ui->horizontalSlider->setTickInterval(10);

    connect(ui->spinBoxCount, &QSpinBox::valueChanged, ui->horizontalSlider, &QSlider::setValue);
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->spinBoxCount, &QSpinBox::setValue);
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_comboBox_currentIndexChanged(int index) {
    if (index < 0) return;
    double dblPrice = ui->comboBox->itemData(index).toDouble();
    ui->doubleSpinBoxPrice->setValue(dblPrice);
    qDebug() << ui->comboBox->currentText() << "\t" << dblPrice;
}

void Widget::on_btnCalc_clicked() {
    double dblTotal = ui->doubleSpinBoxPrice->value() * ui->spinBoxCount->value();
    ui->lineEditTotal->setText(tr("%1 元").arg(dblTotal));
}
