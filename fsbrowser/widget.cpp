#include "widget.h"
#include "./ui_widget.h"
#include <QDateTime> // 日期时间处理
#include <QDebug>
#include <QDesktopServices> // 打开本地文件或 URL
#include <QMessageBox>      // 消息框

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // 设置只读信息显示框
    ui->plainTextEditInfo->setReadOnly(true);

    // 加载图标资源
    iconDriver = QIcon(":/images/driver.png");
    iconFolder = QIcon(":/images/folder.png");
    iconFile = QIcon(":/images/file.png");

    // 默认显示驱动器列表
    on_btnDrivers_clicked();
}

Widget::~Widget() {
    delete ui;
}

/**
 * @brief 根据目录内容更新列表控件
 * @param dir 当前目录对象
 */
void Widget::showItems(const QDir& dir) {
    if (!dir.exists()) return;

    // 获取目录内容，目录优先排序
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::NoFilter, QDir::DirsFirst);
    ui->listWidget->clear();

    for (const QFileInfo& fi : fileInfoList) {
        QString name = fi.fileName();

        if (fi.isDir()) {
            // 文件夹项，type自定义为IFolder
            QListWidgetItem* itemFolder = new QListWidgetItem(iconFolder, name, nullptr, IFolder);
            ui->listWidget->addItem(itemFolder);
        } else {
            // 普通文件项，type自定义为IFile
            QListWidgetItem* itemFile = new QListWidgetItem(iconFile, name, nullptr, IFile);
            ui->listWidget->addItem(itemFile);
        }
    }
}

/**
 * @brief 点击“进入”按钮，切换到输入路径的目录
 */
void Widget::on_btnEnter_clicked() {
    QString strNewDir = ui->lineEditDir->text();
    QDir dirNew(strNewDir);

    if (dirNew.exists()) {
        dirCur = QDir(dirNew.canonicalPath());
        showItems(dirCur);
        ui->lineEditDir->setText(dirCur.absolutePath());
        ui->plainTextEditInfo->setPlainText(tr("进入成功"));
    } else {
        QMessageBox::warning(this, tr("目录不存在"), tr("目录 %1 不存在").arg(strNewDir));
    }
}

/**
 * @brief 点击“驱动器”按钮，显示系统分区根目录
 */
void Widget::on_btnDrivers_clicked() {
    // 获取磁盘根分区
    QFileInfoList drives = QDir::drives();
    // 手动添加程序资源根路径
    drives.append(QFileInfo(":/"));

    ui->listWidget->clear();

    for (const QFileInfo& fi : drives) {
        QString strPath = fi.absolutePath();
        QListWidgetItem* item = new QListWidgetItem(iconDriver, strPath, nullptr, IDriver);
        ui->listWidget->addItem(item);
    }

    dirCur = QDir::root();
    ui->lineEditDir->setText(dirCur.absolutePath());
    ui->plainTextEditInfo->setPlainText(tr("已获取分区根"));
}

/**
 * @brief 点击“上一级”按钮，切换到上级目录
 */
void Widget::on_btnParent_clicked() {
    if (dirCur.cdUp()) {
        ui->lineEditDir->setText(dirCur.absolutePath());
        showItems(dirCur);
        ui->plainTextEditInfo->setPlainText(tr("进入父目录成功"));
    } else {
        ui->plainTextEditInfo->setPlainText(tr("已到根目录"));
    }
}

/**
 * @brief 双击列表项时触发，进入目录或打开文件
 */
void Widget::on_listWidget_itemDoubleClicked(QListWidgetItem* item) {
    if (!item) return;

    int theType = item->type();

    if (theType == IDriver) {
        // 进入分区根目录
        QString strFullPath = item->text();
        dirCur = QDir(strFullPath);
        ui->lineEditDir->setText(dirCur.absolutePath());
        showItems(dirCur);

    } else if (theType == IFolder) {
        QString strName = item->text();
        if (strName == tr(".")) {
            // 当前目录，无需操作
            return;
        } else if (strName == tr("..")) {
            // 进入上级目录
            on_btnParent_clicked();
            return;
        }
        // 进入子目录
        QString strFullPath = QDir::cleanPath(dirCur.absolutePath() + "/" + strName);
        dirCur = QDir(strFullPath);
        ui->lineEditDir->setText(dirCur.absolutePath());
        showItems(dirCur);

    } else {
        // 打开普通文件
        QString strFilePath = QDir::cleanPath(dirCur.absolutePath() + "/" + item->text());

        // 不是资源文件，打开本地文件
        if (!strFilePath.startsWith(":/")) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(strFilePath));
        }
    }
}

/**
 * @brief 当前列表项改变时，显示详细信息
 */
void Widget::on_listWidget_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous) {
    Q_UNUSED(previous);
    if (!current) return;

    QString strResult;
    int theType = current->type();
    QString strName = current->text();

    QFileInfo fi;
    if (theType == IDriver) {
        fi = QFileInfo(strName);
        if (strName.startsWith(":/")) {
            strResult = tr("资源根 %1").arg(strName);
        } else {
            strResult = tr("分区根 %1").arg(strName);
        }
    } else if (theType == IFolder) {
        QString strFullPath = QDir::cleanPath(dirCur.absolutePath() + "/" + strName);
        strResult = tr("文件夹 %1\r\n").arg(strFullPath);
        fi = QFileInfo(strFullPath);
        strResult += getFolderInfo(fi);
    } else {
        QString strFilePath = QDir::cleanPath(dirCur.absolutePath() + "/" + strName);
        strResult = tr("文件 %1\r\n").arg(strFilePath);
        fi = QFileInfo(strFilePath);
        strResult += getFileInfo(fi);
    }

    ui->plainTextEditInfo->setPlainText(strResult);
}

/**
 * @brief 获取文件夹详细信息
 * @param fi 文件信息对象
 * @return 格式化的文件夹信息字符串
 */
QString Widget::getFolderInfo(const QFileInfo& fi) {
    QString strResult;

    // 读写权限
    strResult += tr("可读：%1\r\n").arg(fi.isReadable() ? tr("是") : tr("否"));
    strResult += tr("可写：%1\r\n").arg(fi.isWritable() ? tr("是") : tr("否"));

    // 创建时间 & 修改时间 (Qt 5.10及以上支持 birthTime)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    QDateTime dtCreate = fi.birthTime();
    strResult += tr("创建时间：%1\r\n").arg(dtCreate.toString("yyyy-MM-dd HH:mm:ss"));
#else
    strResult += tr("创建时间：不可用（Qt版本低于5.10）\r\n");
#endif

    QDateTime dtModify = fi.lastModified();
    strResult += tr("修改时间：%1\r\n").arg(dtModify.toString("yyyy-MM-dd HH:mm:ss"));

    return strResult;
}

/**
 * @brief 获取文件详细信息
 * @param fi 文件信息对象
 * @return 格式化的文件信息字符串
 */
QString Widget::getFileInfo(const QFileInfo& fi) {
    QString strResult;

    // 读写及执行权限
    strResult += tr("可读：%1\r\n").arg(fi.isReadable() ? tr("是") : tr("否"));
    strResult += tr("可写：%1\r\n").arg(fi.isWritable() ? tr("是") : tr("否"));
    strResult += tr("可执行：%1\r\n").arg(fi.isExecutable() ? tr("是") : tr("否"));

    // 类型和大小
    strResult += tr("类型：%1\r\n").arg(fi.suffix());
    strResult += tr("大小：%1 B\r\n").arg(fi.size());

#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    QDateTime dtCreate = fi.birthTime();
    strResult += tr("创建时间：%1\r\n").arg(dtCreate.toString("yyyy-MM-dd HH:mm:ss"));
#else
    strResult += tr("创建时间：不可用（Qt版本低于5.10）\r\n");
#endif

    QDateTime dtModify = fi.lastModified();
    strResult += tr("修改时间：%1\r\n").arg(dtModify.toString("yyyy-MM-dd HH:mm:ss"));

    return strResult;
}
