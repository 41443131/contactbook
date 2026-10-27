#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    // 自定義 Write 方法將內容寫入 csv 檔
    void Write(QString Filename, QString str);

private slots:
    void on_pushButton_clicked();   // 新增按鈕
    void on_pushButton_2_clicked(); // 匯出按鈕
    void on_pushButton_3_clicked(); // 匯入按鈕

private:
    Ui::Widget *ui;
    QString mFilename; // 存檔路徑
};

#endif // WIDGET_H