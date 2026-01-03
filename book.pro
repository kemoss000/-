QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code more readable by using the following paths
SOURCES += \
    main.cpp \
    MainWindow.cpp \
    BookWidget.cpp \
    ReaderWidget.cpp \
    BorrowWidget.cpp

HEADERS += \
    MainWindow.h \
    DbManager.h \
    BookWidget.h \
    ReaderWidget.h \
    BorrowWidget.h \
    StatsWidget.h \
    ExportUtil.h
