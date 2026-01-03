// BookWidget.h
#ifndef BOOKWIDGET_H
#define BOOKWIDGET_H

#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "ExportUtil.h"

class BookWidget : public QWidget {
    Q_OBJECT
public:
    explicit BookWidget(QWidget *parent = nullptr);

private slots:
    void addBook();
    void deleteBook();
    void searchBook();
    void saveChanges();
    void exportData();

private:
    QSqlTableModel *model;
    QTableView *view;
    QLineEdit *searchEdit;
};
#endif // BOOKWIDGET_H
