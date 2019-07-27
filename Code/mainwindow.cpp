#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_4->setHidden(true);
    ui->pushButton_5->setHidden(true);
    qbg1 = new QButtonGroup(this);
    qbg2 = new QButtonGroup(this);
    qbg3 = new QButtonGroup(this);
    qbg4 = new QButtonGroup(this);
    qbg1->addButton(ui->radioButton);
    qbg1->addButton(ui->radioButton_2);
    qbg2->addButton(ui->radioButton_3);
    qbg2->addButton(ui->radioButton_4);
    qbg3->addButton(ui->radioButton_5);
    qbg3->addButton(ui->radioButton_6);
    qbg3->addButton(ui->radioButton_7);
    qbg4->addButton(ui->radioButton_14);
    qbg4->addButton(ui->radioButton_15);
    qbg4->addButton(ui->radioButton_16);
    interpolation = 0;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //图片读入
    img = new QImage();
    filename1 = QFileDialog::getOpenFileName(this,
                                          tr("选择图像"),
                                          "",
                                          tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
    if(filename1.isEmpty())
    {
         return;
    }
    else
    {
        if(! ( img->load(filename1) ) ) //加载图像
            {
                    QMessageBox::information(this,
                                             tr("打开图像失败"),
                                             tr("打开图像失败!"));
                    return;
            }

    }
    if(img->width()>1500 || img->height()>1500)
        *img = img->scaled(1500,
                          1500,
                          Qt::KeepAspectRatio);
    //图片显示
    QImage _img = img->scaled(250,
                      250,
                      Qt::KeepAspectRatio);
    ui->label->setPixmap(QPixmap::fromImage(_img));
}

void MainWindow::on_pushButton_2_clicked()
{
    target = new QImage();
    filename2=QFileDialog::getOpenFileName(this,
                                          tr("选择图像"),
                                          "",
                                          tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
    if(filename2.isEmpty())
    {
         return;
    }
    else
    {
        if(! ( target->load(filename2) ) ) //加载图像
            {
                    QMessageBox::information(this,
                                             tr("打开图像失败"),
                                             tr("打开图像失败!"));
                    //delete target;
                    return;
            }

    }
    if(target->width()>1500 || target->height()>1500)
        *target = target->scaled(1500,
                          1500,
                          Qt::KeepAspectRatio);
    QImage _target = target->scaled(250,
                      250,
                      Qt::KeepAspectRatio);
     ui->label_2->setPixmap(QPixmap::fromImage(_target));
}

//生成新人脸
void MainWindow::on_pushButton_3_clicked()
{
    if(img == NULL || target == NULL)
    {
        QMessageBox::information(this,
                                 tr("请选择图片"),
                                 tr("请选择图片!"));
        return;
    }
    if(result != NULL)
        delete result;

    result = new QImage(img->width(),img->height(),QImage::Format_RGB32);

    QString name1 = filename1.mid(filename1.lastIndexOf('/')+1);//获取图片编号
    QString name2 = filename2.mid(filename2.lastIndexOf('/')+1);
    name1 = name1.left(name1.indexOf('.'));
    name2 = name2.left(name2.indexOf('.'));
    QVector<double> img_points_x;
    QVector<double> img_points_y;
    QVector<double> target_points_x;
    QVector<double> target_points_y;

    //提醒等待窗口
    QMessageBox msgBox;
    msgBox.setWindowTitle("程序运行中，请耐心等待");
    msgBox.setText("程序运行中，请耐心等待");
    msgBox.show();


    if(!points_ini(name1 + ".txt",name2 + ".txt",&img_points_x,&img_points_y,&target_points_x,&target_points_y))//初始化关键点
    {
        QMessageBox::information(this,
                                 tr("打开关键点文件失败"),
                                 tr("打开关键点文件失败!"));
        return;
    }
    solve_equ();//求出TPS的参数

    //生成新人脸
    double x = 0;
    double y = 0;

    //B样条
    if(ui->radioButton_4->isChecked())
    {
        if(ui->radioButton_16->isChecked())//局部
        {
                B_spline(img,result,interpolation);
                for(int i=0; i<img->width(); i++)
                {
                    for(int j=0; j<img->height(); j++)
                    {
                        if(result->pixelColor(i,j).red() == 0)
                            result->setPixel(i,j,img->pixel(i,j));
                    }
                }
        }
        else if(ui->radioButton_15->isChecked())
            B_spline_whole(30,img,result,1);//逆向
        else
            B_spline_whole(30,img,result,0);//正向
    }
    else//TPS
    {
            for(int i=0; i<img->width(); i++)
            {
                for(int j=0; j<img->height(); j++)
                {
                    x = map_x(i,j);
                    y = map_y(i,j);
                    switch (interpolation) {//选择插值方式
                    case 0:
                        result->setPixel(i,j,nearest(x,y,img));
                        break;
                    case 1:
                        result->setPixel(i,j,bilinear(x,y,img));
                        break;
                    case 2:
                        result->setPixel(i,j,bicubic(x,y,img));
                        break;
                    default:
                        result->setPixel(i,j,nearest(x,y,img));
                        break;
                    }


                }
            }
    }

    msgBox.setText("已生成新人脸");

    //图片显示
    QImage _result = *result;
    _result = result->scaled(250,
                      250,
                      Qt::KeepAspectRatio);

    ui->label_3->setPixmap(QPixmap::fromImage(_result));
    ui->pushButton_4->setText("保存图片");
    ui->pushButton_4->setVisible(true);
    ui->pushButton_5->setText("查看大图");
    ui->pushButton_5->setVisible(true);

    //显示关键点
    if(ui->radioButton_2->isChecked())
    {
        QImage _img = *img;
        QImage _target = *target;
            for(int i=0; i<68; i++)
            {
                for(int j=-3; j<3; j++)
                {
                    for(int k=-3; k<3; k++)
                    {
                        _img.setPixel((int)img_points_x[i]+j,(int)img_points_y[i]+k,QColor(255,255,255).rgb());

                        _target.setPixel((int)target_points_x[i]+j,(int)target_points_y[i]+k,QColor(255,255,255).rgb());
                    }
                }
            }
            _img = _img.scaled(250,
                              250,
                              Qt::KeepAspectRatio);
            ui->label->setPixmap(QPixmap::fromImage(_img));
            _target = _target.scaled(250,
                              250,
                              Qt::KeepAspectRatio);
             ui->label_2->setPixmap(QPixmap::fromImage(_target));
    }
    //不显示关键点
    if(ui->radioButton->isChecked())
    {
        QImage _img = *img;
        QImage _target = *target;
        _img = _img.scaled(250,
                          250,
                          Qt::KeepAspectRatio);
        ui->label->setPixmap(QPixmap::fromImage(_img));
        _target = _target.scaled(250,
                          250,
                          Qt::KeepAspectRatio);
         ui->label_2->setPixmap(QPixmap::fromImage(_target));
    }

}

//选择插值方式
void MainWindow::on_radioButton_5_clicked()
{
    interpolation = 0;
}

void MainWindow::on_radioButton_7_clicked()
{
    interpolation = 1;
}

void MainWindow::on_radioButton_6_clicked()
{
    interpolation = 2;
}

void MainWindow::on_radioButton_2_clicked()
{

}

void MainWindow::on_pushButton_4_clicked()
{
    if(result->save(filename1 + "_to_" + filename2.mid(filename2.lastIndexOf('/')+1),"BMP"))
    {
        QMessageBox::information(this,
                                 tr("保存成功"),
                                 tr("保存成功!"));
    }
    else
    {
        QMessageBox::information(this,
                                 tr("保存失败"),
                                 tr("保存失败!"));
    }



}

void MainWindow::on_radioButton_4_clicked()
{

}

void MainWindow::on_pushButton_5_clicked()
{
    QWidget * widget = new QWidget;
    //widget->setAtuoFillBackground(true);
    widget->setMaximumSize(result->width(),result->height());

    QPalette palette;
    palette.setBrush(this->backgroundRole(),QBrush(*result));
    widget->setPalette(palette);
    widget->show();
}
