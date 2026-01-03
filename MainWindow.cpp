// MainWindow.cpp
#include "MainWindow.h"
#include "BookWidget.h"
#include "ReaderWidget.h"
#include "BorrowWidget.h"
#include "StatsWidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setWindowTitle("图书与借阅管理系统");
    this->resize(1000, 700);

    QTabWidget *tabWidget = new QTabWidget(this);
    this->setCentralWidget(tabWidget);

    tabWidget->addTab(new BookWidget(this), "图书信息管理");
    tabWidget->addTab(new ReaderWidget(this), "读者档案管理");
    tabWidget->addTab(new BorrowWidget(this), "借还书流程");
    tabWidget->addTab(new StatsWidget(this), "数据统计");
}
