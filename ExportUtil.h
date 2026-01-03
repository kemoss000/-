// ExportUtil.h
#ifndef EXPORTUTIL_H
#define EXPORTUTIL_H

#include <QString>
#include <QAbstractItemModel>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

class ExportUtil {
public:
    static void exportToCSV(QAbstractItemModel *model, QWidget *parent) {
        QString fileName = QFileDialog::getSaveFileName(parent, "导出数据", "", "CSV Files (*.csv)");
        if (fileName.isEmpty()) return;

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(parent, "错误", "无法保存文件");
            return;
        }

        QTextStream out(&file);
        // 写表头
        for (int i = 0; i < model->columnCount(); ++i) {
            out << model->headerData(i, Qt::Horizontal).toString() << ",";
        }
        out << "\n";

        // 写数据
        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount(); ++col) {
                QString data = model->data(model->index(row, col)).toString();
                // 处理数据中的逗号，避免CSV格式错误
                data.replace(",", " ");
                out << data << ",";
            }
            out << "\n";
        }
        file.close();
        QMessageBox::information(parent, "成功", "数据导出成功！");
    }
};

#endif // EXPORTUTIL_H
