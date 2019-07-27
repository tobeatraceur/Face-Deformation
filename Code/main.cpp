#include "mainwindow.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("人脸变形");
    w.setWindowIcon(QIcon("trump.ico"));
    w.show();


    return a.exec();
}
