// ReaderWidget.h
#ifndef READERWIDGET_H
#define READERWIDGET_H
// ... includes 类似 BookWidget ...
#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QSqlError>
#include "ExportUtil.h"

class ReaderWidget : public QWidget {
    Q_OBJECT
public:
    explicit ReaderWidget(QWidget *parent = nullptr);

private slots:
    void addReader();
    void deleteReader();
    void saveChanges();
    void exportData();

private:
    QSqlTableModel *model;
    QTableView *view;
};
#endif
