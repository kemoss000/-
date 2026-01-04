// BorrowWidget.cpp
#include "BorrowWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QGroupBox>

BorrowWidget::BorrowWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // --- 操作区 ---
    QGroupBox *grpBox = new QGroupBox("借还操作", this);
    QHBoxLayout *opLayout = new QHBoxLayout(grpBox);

    editCardId = new QLineEdit(this); editCardId->setPlaceholderText("读者卡号");
    editBookId = new QLineEdit(this); editBookId->setPlaceholderText("图书ID");

    QPushButton *btnBorrow = new QPushButton("借书", this);
    QPushButton *btnReturn = new QPushButton("还书", this);
    QPushButton *btnOverdue = new QPushButton("仅显示逾期", this);
    QPushButton *btnRefresh = new QPushButton("刷新记录", this);

    opLayout->addWidget(new QLabel("卡号:"));
    opLayout->addWidget(editCardId);
    opLayout->addWidget(new QLabel("图书ID:"));
    opLayout->addWidget(editBookId);
    opLayout->addWidget(btnBorrow);
    opLayout->addWidget(btnReturn);
    opLayout->addStretch();
    opLayout->addWidget(btnOverdue);
    opLayout->addWidget(btnRefresh);

    mainLayout->addWidget(grpBox);

    // --- 列表区 ---
    view = new QTableView(this);
    model = new QSqlQueryModel(this);
    view->setModel(model);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    mainLayout->addWidget(view);

    connect(btnBorrow, &QPushButton::clicked, this, &BorrowWidget::borrowBook);
    connect(btnReturn, &QPushButton::clicked, this, &BorrowWidget::returnBook);
    connect(btnRefresh, &QPushButton::clicked, this, &BorrowWidget::refreshList);
    connect(btnOverdue, &QPushButton::clicked, this, &BorrowWidget::filterOverdue);

    refreshList();
}

void BorrowWidget::refreshList() {
    // 多表联查：显示借阅详情
    QString sql = "SELECT br.id, r.name, b.title, br.borrow_date, br.return_date, br.is_returned "
                  "FROM borrow_records br "
                  "LEFT JOIN books b ON br.book_id = b.id "
                  "LEFT JOIN readers r ON br.reader_id = r.id "
                  "ORDER BY br.id DESC";
    model->setQuery(sql);
    model->setHeaderData(0, Qt::Horizontal, "记录ID");
    model->setHeaderData(1, Qt::Horizontal, "读者姓名");
    model->setHeaderData(2, Qt::Horizontal, "书名");
    model->setHeaderData(3, Qt::Horizontal, "借阅日期");
    model->setHeaderData(4, Qt::Horizontal, "归还日期");
    model->setHeaderData(5, Qt::Horizontal, "状态(1=已还)");
}

void BorrowWidget::filterOverdue() {
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    QString sql = QString("SELECT br.id, r.name, b.title, br.borrow_date "
                          "FROM borrow_records br "
                          "LEFT JOIN books b ON br.book_id = b.id "
                          "LEFT JOIN readers r ON br.reader_id = r.id "
                          "WHERE br.is_returned = 0 AND "
                          "julianday('%1') - julianday(br.borrow_date) > 30").arg(today);

    model->setQuery(sql);
    if(model->rowCount() == 0) QMessageBox::information(this, "提示", "没有发现逾期记录");
}

void BorrowWidget::borrowBook() {
    QString card = editCardId->text();
    QString bookId = editBookId->text();

    if(card.isEmpty() || bookId.isEmpty()) return;

    QSqlDatabase::database().transaction(); // 开启事务

    // 1. 查找读者ID
    QSqlQuery q;
    q.prepare("SELECT id FROM readers WHERE card_id = :card");
    q.bindValue(":card", card);
    if(!q.exec() || !q.next()) {
        QMessageBox::critical(this, "错误", "读者不存在");
        QSqlDatabase::database().rollback();
        return;
    }
    int readerId = q.value(0).toInt();

    // 2. 检查库存
    q.prepare("SELECT stock FROM books WHERE id = :bid");
    q.bindValue(":bid", bookId);
    if(!q.exec() || !q.next()) {
        QMessageBox::critical(this, "错误", "图书不存在");
        QSqlDatabase::database().rollback();
        return;
    }
    int stock = q.value(0).toInt();
    if(stock <= 0) {
        QMessageBox::warning(this, "提示", "库存不足！");
        QSqlDatabase::database().rollback();
        return;
    }

    // 3. 插入借阅记录
    q.prepare("INSERT INTO borrow_records(book_id, reader_id, borrow_date, is_returned) "
              "VALUES(:bid, :rid, :date, 0)");
    q.bindValue(":bid", bookId);
    q.bindValue(":rid", readerId);
    q.bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));
    q.exec();

    // 4. 减少库存
    q.prepare("UPDATE books SET stock = stock - 1 WHERE id = :bid");
    q.bindValue(":bid", bookId);
    q.exec();

    if(QSqlDatabase::database().commit()) {
        QMessageBox::information(this, "成功", "借阅成功！");
        refreshList();
    } else {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "错误", "操作失败");
    }
}

void BorrowWidget::returnBook() {
    // 获取当前选中的记录ID
    int row = view->currentIndex().row();
    if(row < 0) {
        QMessageBox::warning(this, "提示", "请先在列表中选中一条未归还记录");
        return;
    }

    int recordId = model->data(model->index(row, 0)).toInt(); // 记录ID
    int isReturned = model->data(model->index(row, 5)).toInt();

    if(isReturned == 1) {
        QMessageBox::warning(this, "提示", "这本书已经还过了");
        return;
    }

    // 获取BookID以便恢复库存
    QSqlQuery q;
    q.prepare("SELECT book_id FROM borrow_records WHERE id = :id");
    q.bindValue(":id", recordId);
    q.exec();
    q.next();
    int bookId = q.value(0).toInt();

    QSqlDatabase::database().transaction();

    // 1. 更新记录状态
    q.prepare("UPDATE borrow_records SET return_date = :date, is_returned = 1 WHERE id = :id");
    q.bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));
    q.bindValue(":id", recordId);
    q.exec();

    // 2. 增加库存
    q.prepare("UPDATE books SET stock = stock + 1 WHERE id = :bid");
    q.bindValue(":bid", bookId);
    q.exec();

    if(QSqlDatabase::database().commit()) {
        QMessageBox::information(this, "成功", "归还成功！");
        refreshList();
    } else {
        QSqlDatabase::database().rollback();
    }
}

