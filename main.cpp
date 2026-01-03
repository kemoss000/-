// main.cpp
#include <QApplication>
#include "MainWindow.h"
#include "DbManager.h"
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 1. 连接数据库
    if (!DbManager::connect()) {
        QMessageBox::critical(nullptr, "Fatal Error", "数据库连接失败！");
        return -1;
    }

    // 2. 启动主界面
    MainWindow w;
    w.show();

    return a.exec();
}
