#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>
#include <QFile>

// 构造函数：初始化 UI 和加载地图数据
Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    loadData();     // 加载地图和省份坐标数据
    initControls(); // 初始化控件和布局
}

// 析构函数：释放 UI 资源
Widget::~Widget() {
    delete ui;
}

// 加载地图图片和省份坐标文本数据
void Widget::loadData() {
    // 加载地图图片资源（china.png）
    m_map.load(":/china.png");

    // 打开文本文件资源（china.txt），用于读取省份名与坐标
    QFile fileIn(":/china.txt");

    // 清空已有数据
    m_listProvinces.clear();
    m_listPoints.clear();

    // 以只读文本方式打开文件
    fileIn.open(QIODevice::ReadOnly | QIODevice::Text);

    // 逐行读取文件内容
    while (!fileIn.atEnd()) {
        QByteArray baLine = fileIn.readLine();       // 读取一行字节数据
        QString strLine = QString::fromUtf8(baLine); // 转为 UTF-8 字符串
        QStringList liParts = strLine.split('\t');   // 每行格式：[省份名]\t[x]\t[y]

        QPoint pt;
        m_listProvinces << liParts[0]; // 保存省份名
        pt.setX(liParts[1].toInt());   // 提取并设置横坐标
        pt.setY(liParts[2].toInt());   // 提取并设置纵坐标
        m_listPoints << pt;            // 保存坐标点
    }

    // 打印加载信息（调试用途）
    qDebug() << m_listProvinces.size() << m_listProvinces;
    qDebug() << m_listPoints.size() << m_listPoints;
}

// 初始化界面控件及布局
void Widget::initControls() {
    // 1. 创建 QLabel 显示地图，并设置地图图像
    m_labelMap = new QLabel();
    m_labelMap->setPixmap(m_map); // 地图图像设置到标签上

    // 2. 创建左侧滚动区域（显示地图）
    m_saLeftMap = new QScrollArea();
    m_saLeftMap->setWidget(m_labelMap); // 将地图标签嵌入滚动区域

    // 3. 创建右侧按钮容器及垂直布局器（用于省份选择按钮）
    QWidget* pWidRight = new QWidget();
    QVBoxLayout* pLayoutRight = new QVBoxLayout();

    QRadioButton* curButton = nullptr;
    int nCount = m_listProvinces.size(); // 省份总数量

    // 4. 遍历每个省份，创建对应的单选按钮
    for (int i = 0; i < nCount; ++i) {
        curButton = new QRadioButton(m_listProvinces[i]);

        // 使用 lambda 表达式直接传递当前索引到槽函数
        // 避免使用 QSignalMapper，代码更简洁且类型安全
        connect(curButton, &QRadioButton::clicked, this, [=]() {
            showProvince(i); // 点击按钮时，跳转地图视图到对应省份
        });

        pLayoutRight->addWidget(curButton); // 添加按钮到右侧布局器
    }

    // 5. 设置右侧按钮容器布局，并包裹进滚动区域
    pWidRight->setLayout(pLayoutRight);
    m_saRightButtons = new QScrollArea();
    m_saRightButtons->setWidget(pWidRight); // 设置内容为按钮容器

    // 6. 创建主布局器，左右两个滚动区域并排放置
    QHBoxLayout* pMainLayout = new QHBoxLayout();
    pMainLayout->addWidget(m_saLeftMap);      // 左侧地图区域
    pMainLayout->addWidget(m_saRightButtons); // 右侧按钮区域
    pMainLayout->setStretch(0, 4);            // 左边宽一些
    pMainLayout->setStretch(1, 1);            // 右边窄一些

    // 7. 设置主窗口布局器
    setLayout(pMainLayout);

    // 8. 设置窗口初始大小
    resize(800, 600);

    // 9. 默认选中最后一个按钮（如“台湾”）
    curButton->setChecked(true);

    // 10. 确保该按钮在右侧滚动区域中可见
    m_saRightButtons->ensureWidgetVisible(curButton);

    // 11. 滚动地图区域，让最后一个省份坐标显示在可见视口
    // 参数含义：目标坐标（x, y），额外偏移（xMargin, yMargin）
    m_saLeftMap->ensureVisible(m_listPoints[nCount - 1].x(), m_listPoints[nCount - 1].y(), 200, 200);
}

// 槽函数：点击某个省份按钮后调用，滚动地图视图到该省份坐标
void Widget::showProvince(int index) {
    // 滚动地图视口，确保该省份的位置处于可视范围内
    m_saLeftMap->ensureVisible(m_listPoints[index].x(), m_listPoints[index].y(), 200, 200); // 设置边缘留白，提升可视性
}
