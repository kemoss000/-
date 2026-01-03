// BookWidget.cpp
#include "BookWidget.h"
#include <QMessageBox>
#include <QSqlError>

BookWidget::BookWidget(QWidget *parent) : QWidget(parent) {
    // 1. 初始化 Model
    model = new QSqlTableModel(this);
    model->setTable("books");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit); // 手动提交，防止误操作
    model->select();

    // 设置表头名称
    model->setHeaderData(1, Qt::Horizontal, "书名");
    model->setHeaderData(2, Qt::Horizontal, "作者");
    model->setHeaderData(3, Qt::Horizontal, "分类");
    model->setHeaderData(4, Qt::Horizontal, "ISBN");
    model->setHeaderData(5, Qt::Horizontal, "价格");
    model->setHeaderData(6, Qt::Horizontal, "库存");
    model->setHeaderData(7, Qt::Horizontal, "总量");

    // 2. 初始化 View
    view = new QTableView(this);
    view->setModel(model);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setAlternatingRowColors(true);
    view->hideColumn(0); // 隐藏ID列

    // 3. UI 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *topLayout = new QHBoxLayout();

    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("按书名搜索...");
    QPushButton *btnSearch = new QPushButton("搜索", this);
    QPushButton *btnAdd = new QPushButton("添加图书", this);
    QPushButton *btnDel = new QPushButton("删除选中", this);
    QPushButton *btnSave = new QPushButton("提交修改", this);
    QPushButton *btnExport = new QPushButton("导出CSV", this);

    topLayout->addWidget(new QLabel("查找:"));
    topLayout->addWidget(searchEdit);
    topLayout->addWidget(btnSearch);
    topLayout->addStretch();
    topLayout->addWidget(btnAdd);
    topLayout->addWidget(btnDel);
    topLayout->addWidget(btnSave);
    topLayout->addWidget(btnExport);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(view);

    // 4. 信号槽连接
    connect(btnSearch, &QPushButton::clicked, this, &BookWidget::searchBook);
    connect(btnAdd, &QPushButton::clicked, this, &BookWidget::addBook);
    connect(btnDel, &QPushButton::clicked, this, &BookWidget::deleteBook);
    connect(btnSave, &QPushButton::clicked, this, &BookWidget::saveChanges);
    connect(btnExport, &QPushButton::clicked, this, &BookWidget::exportData);
}

void BookWidget::addBook() {
    int row = model->rowCount();
    model->insertRow(row);
    // 可以在这里设置默认值
    model->setData(model->index(row, 6), 1); // 默认库存1
    model->setData(model->index(row, 7), 1); // 默认总量1
}

void BookWidget::deleteBook() {
    QModelIndexList selection = view->selectionModel()->selectedRows();
    for(int i=0; i< selection.count(); i++) {
        model->removeRow(selection.at(i).row());
    }
}

void BookWidget::saveChanges() {
    if(model->submitAll()) {
        QMessageBox::information(this, "提示", "保存成功！");
    } else {
        QMessageBox::warning(this, "错误", model->lastError().text());
    }
}

void BookWidget::searchBook() {
    QString text = searchEdit->text();
    // 简单的模糊查询
    model->setFilter(QString("title LIKE '%%1%'").arg(text));
    model->select();
}

void BookWidget::exportData() {
    ExportUtil::exportToCSV(model, this);
}
