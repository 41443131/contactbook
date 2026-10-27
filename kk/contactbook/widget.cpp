#include "widget.h"
#include "ui_widget.h"
#include <QLineEdit>
#include <algorithm>
#include <QStringList>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 設定您指定的存放路徑
    mFilename = "C:/Users/User/OneDrive/Desktop/kk/kk/contactbook_1.csv";

    // 設定 TableWidget 欄位與標頭
    ui->tableWidget->setColumnCount(4);
    QStringList ColTotle;
    ColTotle << QStringLiteral("學號")
             << QStringLiteral("班級")
             << QStringLiteral("姓名")
             << QStringLiteral("電話");
    ui->tableWidget->setHorizontalHeaderLabels(ColTotle);
}

Widget::~Widget()
{
    delete ui;
}

// 自定義 Write 方法
void Widget::Write(QString Filename, QString str)
{
    QFileInfo fileInfo(Filename);
    QDir().mkpath(fileInfo.absolutePath());

    QFile mFile(Filename); // 宣告 QFile 函數
    if(!mFile.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "could not open file for write";
        return;
    }

    QTextStream out(&mFile);
    out << str;      // 將內容輸出至 csv 檔
    mFile.flush();   // 刷新 mFile
    mFile.close();   // 關閉 mFile
}

// 新增按鈕 (pushButton) 點擊事件
void Widget::on_pushButton_clicked()
{
    QList<QLineEdit *> lineEdits = this->findChildren<QLineEdit *>();
    if (lineEdits.size() < 4) return;

    // 自動根據輸入框在畫面上「由上到下」的 Y 座標進行精確排序
    std::sort(lineEdits.begin(), lineEdits.end(), [](QLineEdit *a, QLineEdit *b) {
        return a->y() < b->y();
    });

    QTableWidgetItem *inputRow0 = new QTableWidgetItem(lineEdits[0]->text());
    QTableWidgetItem *inputRow1 = new QTableWidgetItem(lineEdits[1]->text());
    QTableWidgetItem *inputRow2 = new QTableWidgetItem(lineEdits[2]->text());
    QTableWidgetItem *inputRow3 = new QTableWidgetItem(lineEdits[3]->text());

    int currentRow = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(currentRow); // 加入新的一列

    ui->tableWidget->setItem(currentRow, 0, inputRow0); // 設定欄位資料
    ui->tableWidget->setItem(currentRow, 1, inputRow1);
    ui->tableWidget->setItem(currentRow, 2, inputRow2);
    ui->tableWidget->setItem(currentRow, 3, inputRow3);

    for (QLineEdit *le : lineEdits) {
        le->setText(""); // 清除輸入框
    }
}

// 匯出按鈕 (pushButton_2) 點擊事件 (單列水平連續 CSV 格式)
void Widget::on_pushButton_2_clicked()
{
    QString saveFile = ""; // 存檔內容

    // 取出表格內所有欄位資料，用逗號串接在同一列
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            if (ui->tableWidget->item(i, j)) {
                saveFile += ui->tableWidget->item(i, j)->text() + ",";
            }
        }
    }

    // 呼叫 Write 方法將資料寫入指定路徑
    Write(mFilename, saveFile);
}

// 匯入按鈕 (pushButton_3) 點擊事件
void Widget::on_pushButton_3_clicked()
{
    // 開啟對話框選擇您的 csv 檔案
    QString fileName = QFileDialog::getOpenFileName(this, "選擇 CSV 檔案", "C:/Users/User/OneDrive/Desktop/kk/kk", "CSV Files (*.csv);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "could not open file for read";
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    file.close();

    // 修正此處：使用 Qt::SkipEmptyParts
    QStringList fields = line.split(",", Qt::SkipEmptyParts);

    ui->tableWidget->setRowCount(0); // 清空現有表格（建置時 widget 中無資料）

    // 每 4 個欄位還原一筆資料至對應欄位
    for (int i = 0; i < fields.size() - 3; i += 4) {
        int currentRow = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(currentRow);

        ui->tableWidget->setItem(currentRow, 0, new QTableWidgetItem(fields[i]));
        ui->tableWidget->setItem(currentRow, 1, new QTableWidgetItem(fields[i+1]));
        ui->tableWidget->setItem(currentRow, 2, new QTableWidgetItem(fields[i+2]));
        ui->tableWidget->setItem(currentRow, 3, new QTableWidgetItem(fields[i+3]));
    }
}