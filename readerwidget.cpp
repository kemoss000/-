// ReaderWidget.cpp
#include "ReaderWidget.h"

ReaderWidget::ReaderWidget(QWidget *parent) : QWidget(parent) {
    model = new QSqlTableModel(this);
    model->setTable("readers");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(1, Qt::Horizontal, "姓名");
    model->setHeaderData(2, Qt::Horizontal, "电话");
    model->setHeaderData(3, Qt::Horizontal, "卡号");

    view = new QTableView(this);
    view->setModel(model);
    view->hideColumn(0);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *btnLayout = new QHBoxLayout();

    QPushButton *btnAdd = new QPushButton("新增读者", this);
    QPushButton *btnDel = new QPushButton("删除读者", this);
    QPushButton *btnSave = new QPushButton("保存数据", this);
    QPushButton *btnExport = new QPushButton("导出", this);

    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnDel);
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnExport);

    layout->addLayout(btnLayout);
    layout->addWidget(view);

    connect(btnAdd, &QPushButton::clicked, this, &ReaderWidget::addReader);
    connect(btnDel, &QPushButton::clicked, this, &ReaderWidget::deleteReader);
    connect(btnSave, &QPushButton::clicked, this, &ReaderWidget::saveChanges);
    connect(btnExport, &QPushButton::clicked, this, &ReaderWidget::exportData);
}

void ReaderWidget::addReader() {
    model->insertRow(model->rowCount());
}

void ReaderWidget::deleteReader() {
    QModelIndexList selection = view->selectionModel()->selectedRows();
    for(auto &index : selection) model->removeRow(index.row());
}

void ReaderWidget::saveChanges() {
    if(!model->submitAll()) {
        QMessageBox::warning(this, "Error", model->lastError().text());
    }
}

void ReaderWidget::exportData() {
    ExportUtil::exportToCSV(model, this);
}
