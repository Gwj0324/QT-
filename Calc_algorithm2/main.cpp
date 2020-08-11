#define AUTO_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#include "widget.h"
#include <QApplication>
#include <QFile>
#include <QSettings>

void SetMyAppAutoRun(bool isstart)
{
    /* 获取应用名称 */
    QString application_name = QApplication::applicationName();
    /* 创建QSetting, 需要添加QSetting头文件 */
    QSettings *settings = new QSettings(AUTO_RUN, QSettings::NativeFormat);
    if(isstart){
        /* 找到当前运行目录的目录 */
        QString application_path = QApplication::applicationFilePath();
        /* 写入注册表 */
        settings->setValue(application_name, application_path.replace("/", "\\"));
    }
    else {
        /* 移除 */
        settings->remove(application_name);
    }
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //加载样式表
    QFile file(":/new/prefix1/StyleSheet/style.css");
    file.open(QIODevice::ReadOnly);
    QString css =file.readAll();
    qApp->setStyleSheet(css);
    file.close();
    //设置开机自启
    SetMyAppAutoRun(true);

    Widget w;


    return a.exec();
}
