// DbManager.h
#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

class DbManager {
public:
    static bool connect() {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        // 将数据库文件存放在文档目录下，避免权限问题
        QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        db.setDatabaseName(path + "/LibrarySystem.db");

        if (!db.open()) {
            qDebug() << "Error: Connection failed" << db.lastError();
            return false;
        }

        // 初始化表结构
        QSqlQuery query;

        // 1. 图书表
        query.exec("CREATE TABLE IF NOT EXISTS books ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "title TEXT NOT NULL, "
                   "author TEXT, "
                   "category TEXT, "
                   "isbn TEXT, "
                   "price REAL, "
                   "stock INTEGER, " // 当前库存
                   "total INTEGER)"); // 总数量

        // 2. 读者表
        query.exec("CREATE TABLE IF NOT EXISTS readers ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "name TEXT NOT NULL, "
                   "phone TEXT, "
                   "card_id TEXT UNIQUE)");

        // 3. 借阅记录表
        query.exec("CREATE TABLE IF NOT EXISTS borrow_records ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "book_id INTEGER, "
                   "reader_id INTEGER, "
                   "borrow_date TEXT, "
                   "return_date TEXT, " // 实际归还日期，为空表示未还
                   "is_returned INTEGER DEFAULT 0)");

        return true;
    }
};

#endif // DBMANAGER_H

