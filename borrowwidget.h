// BorrowWidget.h
#ifndef BORROWWIDGET_H
#define BORROWWIDGET_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QDate>

class BorrowWidget : public QWidget {
    Q_OBJECT
public:
    explicit BorrowWidget(QWidget *parent = nullptr);

private slots:
    void borrowBook();
    void returnBook();
    void refreshList();
    void filterOverdue(); // 筛选逾期

private:
    QLineEdit *editCardId;
    QLineEdit *editBookId; // 这里可以优化为下拉框，为了简单用ID输入
    QTableView *view;
    QSqlQueryModel *model;
};
#endif
