#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include "tps.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QImage* img;
    QImage* target;
    QImage* result;
    QButtonGroup* qbg1;
    QButtonGroup* qbg2;
    QButtonGroup* qbg3;
    QButtonGroup* qbg4;
    QString filename1;
    QString filename2;
    int interpolation;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_5_clicked();

    void on_radioButton_7_clicked();

    void on_radioButton_6_clicked();

    void on_pushButton_4_clicked();

    void on_radioButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
