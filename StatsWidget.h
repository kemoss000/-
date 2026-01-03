// StatsWidget.h
#ifndef STATSWIDGET_H
#define STATSWIDGET_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QTableView>
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>

class StatsWidget : public QWidget {
    Q_OBJECT
public:
    explicit StatsWidget(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *title = new QLabel("图书借阅热度排行榜 (Top 10)", this);
        QFont font = title->font();
        font.setBold(true);
        font.setPointSize(12);
        title->setFont(font);

        QTableView *view = new QTableView(this);
        QSqlQueryModel *model = new QSqlQueryModel(this);

        // 聚合查询：统计每本书在借阅表中出现的次数
        QString sql = "SELECT b.title, b.author, COUNT(br.id) as borrow_count "
                      "FROM borrow_records br "
                      "JOIN books b ON br.book_id = b.id "
                      "GROUP BY br.book_id "
                      "ORDER BY borrow_count DESC "
                      "LIMIT 10";

        model->setQuery(sql);
        model->setHeaderData(0, Qt::Horizontal, "书名");
        model->setHeaderData(1, Qt::Horizontal, "作者");
        model->setHeaderData(2, Qt::Horizontal, "借阅次数");

        view->setModel(model);
        view->horizontalHeader()->setStretchLastSection(true);

        layout->addWidget(title);
        layout->addWidget(view);
    }
};
#endif

