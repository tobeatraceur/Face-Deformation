#include "tps.h"



//变形函数



//基函数
double base_func(double r_square)
{
    if (r_square == 0)
        return 0;
      else
        return r_square * log(r_square);
}

//计算L
void calc_L()
{

    int s = length;
    //矩阵K
    for (int i=0; i < s; i++)
      {
        for (int j=i+1; j < s; j++)
        {
          double r_square = (new_points_x[i] - new_points_x[j]) * (new_points_x[i] - new_points_x[j])
                  + (new_points_y[i] - new_points_y[j]) * (new_points_y[i] - new_points_y[j]);
          L[i][j] = L[j][i] = base_func(r_square);
        }
      }
    //矩阵P及其转置
    for (int i=0; i < s; i++)
    {
        L[i][i] = 0;
        L[i][s] = 1;
        L[i][s+1] = new_points_x[i];
        L[i][s+2] = new_points_y[i];
        L[s][i] = 1;
        L[s+1][i] = new_points_x[i];
        L[s+2][i] = new_points_y[i];
    }

    for (int i=s; i < s+3; i++)
      {
        for (int j=s; j < s+3; j++)
        {
            L[i][j] = 0;
        }
      }
}

//计算Y
void calc_Y()
{
    int s = points_x.size();
    if (s < 3)
        return;

    for (int i=0; i < s; i++)
    {
        Y[0][i] = points_x[i];
        Y[1][i] = points_y[i];
    }
    for (int i=s; i < s+3; i++)
    {
        Y[0][i] = Y[1][i] = 0;
    }

}

//关键点初始化
int points_ini(QString file1, QString file2, QVector<double> *img_x, QVector<double> *img_y, QVector<double> *target_x, QVector<double> *target_y)
{
    points_x.clear();
    new_points_x.clear();
    points_y.clear();
    new_points_y.clear();
    QFile file(file1);
    double avg_x = 0;
    double avg_y = 0;
    double var = 0;

    //从txt读入关键点信息
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            double x = line.mid(0,line.indexOf(" ")).toDouble();
            double y = line.mid(line.indexOf(" ")+1,line.indexOf("\n")-line.indexOf(" ")-1).toDouble();
            avg_x += x;
            avg_y += y;
            points_x.append(x);
            points_y.append(y);

        }
        file.close();
    }
    else
    {
        return 0;
    }
    //关键点信息返回给主窗口
    *img_x = points_x;
    *img_y = points_y;

    //计算均值
    length = points_x.size();
    avg_x = avg_x / length;
    avg_y = avg_y / length;
    //计算方差
    for(int i=0; i<length; i++)
    {
        double dx = points_x[i]-avg_x;
        double dy = points_y[i]-avg_y;
        double square = dx*dx+dy*dy;
        var += qSqrt(square);
    }
    var = var /length;

    //对目标人脸关键点的处理类似
    double avg_x2 = 0;
    double avg_y2 = 0;
    double var2 = 0;
    QFile new_file(file2);
    if (new_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!new_file.atEnd())
        {
            QByteArray line = new_file.readLine();
            double x = line.mid(0,line.indexOf(" ")).toDouble();
            double y = line.mid(line.indexOf(" ")+1,line.indexOf("\n")-line.indexOf(" ")-1).toDouble();

            avg_x2 += x;
            avg_y2 += y;
            new_points_x.append(x);
            new_points_y.append(y);
        }
        new_file.close();
    }
    else
    {
        return 0;
    }
    *target_x = new_points_x;
    *target_y = new_points_y;
    avg_x2 = avg_x2 / length;
    avg_y2 = avg_y2 / length;


    for(int i=0; i<length; i++)
    {
        var2 += qSqrt((new_points_x[i]-avg_x2) * (new_points_x[i]-avg_x2) + (new_points_y[i]-avg_y2) * (new_points_y[i]-avg_y2));
    }
    var2 = var2 /length;

    //计算中心及方差的偏差
    double delta_x = avg_x2 - avg_x;
    double delta_y = avg_y2 - avg_y;
    double delta_var;
    delta_var = var2 - var;

    //调整中心及方差
    for(int i=0; i<length; i++)
    {
        new_points_x[i] = new_points_x[i] - delta_x;
        new_points_y[i] = new_points_y[i] - delta_y;

        double delta_var_x = delta_var * (new_points_x[i] - avg_x) / var2 ;//加权伸缩
        double delta_var_y = delta_var * (new_points_y[i] - avg_y) / var2 ;
        new_points_x[i] = new_points_x[i] - delta_var_x;
        new_points_y[i] = new_points_y[i] - delta_var_y;
    }

    //计算角度偏差alpha
    double alpha = -atan((new_points_y[16] - new_points_y[0]) / (new_points_x[16] - new_points_x[0]))
                   +atan((points_y[16] - points_y[0]) / (points_x[16] - points_x[0]));
    //调整角度
    for(int i=0; i<length; i++)
    {
        double norm2 = qSqrt(((new_points_x[i] - avg_x) * (new_points_x[i] - avg_x) + (new_points_y[i] - avg_y) * (new_points_y[i] - avg_y)));
        double beta = atan((new_points_y[i] - avg_y) / (new_points_x[i] - avg_x));

        if(points_x[i] > avg_x)
            new_points_x[i] = avg_x + norm2 * fabs(cos(alpha + beta));
        else
            new_points_x[i] = avg_x - norm2 * fabs(cos(beta + alpha));
        if(points_y[i] > avg_y)
            new_points_y[i] = avg_y + norm2 * fabs(sin(alpha + beta));
        else
            new_points_y[i] = avg_y - norm2 * fabs(sin(alpha + beta));
    }


    return 1;
}

//求出TPS的参数
void solve_equ()
{
    int n = length + 3;

    L = new double*[n];
    Y = new double*[2];
    for (int i = 0; i < n; ++i)
    {
        L[i] = new double[n];
    }
    for (int i = 0; i < 2; ++i)
    {
        Y[i] = new double[n];
    }
    calc_L();
    calc_Y();

    double *p = new double[n];
    double *b = *Y;
    double *c = *(Y+1);
    //解方程
    qr(L,p,n);
    hshld(L,p,b,n);
    hshld(L,p,c,n);
    //参数赋值
    coex = b;
    coey = c;

}

//TPS新人脸中点的映射
double map_x(int x,int y)
{
    int n = length;
    double sum = 0;
    for(int i=0; i<n; ++i)
    {
        double r_square = (new_points_x[i] - x) * (new_points_x[i] - x)
                + (new_points_y[i] - y) * (new_points_y[i] - y);
        sum += coex[i] * base_func(r_square);
    }
    double new_x = coex[n] + coex[n+1] * x + coex[n+2] * y + sum;
    return new_x;
}
double map_y(int x,int y)
{
    int n = length;
    double sum = 0;
    for(int i=0; i<n; ++i)
    {
        double r_square = (new_points_x[i] - x) * (new_points_x[i] - x)
                + (new_points_y[i] - y) * (new_points_y[i] - y);
        sum += coey[i] * base_func(r_square);
    }
    double new_y = coey[n] + coey[n+1] * x + coey[n+2] * y + sum;
    return new_y;
}

//三次B样条基函数
double G_3(int i,double j)
{
    double Gij;
    switch (i)
    {
    case 0:
        Gij = 1.0 / 6.0*(3 * j*j - j*j*j - 3 * j + 1);
        break;
    case 1:
        Gij = 1.0 / 6.0*(3 * j*j*j - 6 * j*j + 4);
        break;
    case 2:
        Gij = 1.0 / 6.0*(3 * j*j - 3 * j*j*j + 3 * j + 1);
        break;
    case 3:
        Gij = 1.0 / 6.0*j*j*j;
        break;
    default:
        break;
    }
    return Gij;
}

//B样条变形_局部
void B_spline(QImage *img,QImage *result,int interpolation)
{

    QImage temp_img = *img;//每个关键点变形后暂存图片
    int N = 0,N_x = 0,N_y = 0;
    for(int k=0; k<length; k++)
    {
       double distance_x = (new_points_x[k] - points_x[k]) ;
       double distance_y = (new_points_y[k] - points_y[k]) ;

       if(k == 0 || k == 17 || k == 27 || k == 31 ||  k == 48 ||k == 22 )//除眼睛外的其他部位起始点
       {
          N_x = (abs(points_x[k] - points_x[k+1]) ) / 1;
          N_y = (abs(points_y[k] - points_y[k+1]) ) / 1;
          N = N_x > N_y ? N_x : N_y;
          N_x = N_y = N;
       }
       else if(k >= 36 && k < 47)//眼睛
       {
           N_x = (abs(points_x[k] - points_x[k+1]) ) / 2;
           N_y = (abs(points_y[k] - points_y[k+1]) ) / 2;
       }
       else
       {
           N_x = (abs(points_x[k] - points_x[k-1]) / 1 ) ;
           N_y = (abs(points_y[k] - points_y[k-1]) / 1);
           N = N_x > N_y ? N_x : N_y;
           N_x = N_y = N;
       }


        //遍历控制区域
       for (int i = points_x[k] - 2 * N_x + 1; i < points_x[k] + 2 * N_x; i++)
       {
           if(i <0)
           {
               i = 0;
               continue;
           }
           if(i >= img->width())
               break;
           for (int j = points_y[k] - 2 * N_y + 1; j < points_y[k] + 2 * N_y; j++)
           {
               if(j <0)
               {
                   j = 0;
                   continue;
               }
               if(j >= img->height())
                   break;

               int red = 0, green = 0, blue = 0;

               int l = 1 - (int)floor((i - points_x[k]) / N_x);
               int m = 1 - (int)floor((j - points_y[k]) / N_y);

               double u = (double)l + 1.0*((double)i - points_x[k]) / (double)N_x - 1;
               double v = (double)m + 1.0*((double)j - points_y[k]) / (double)N_y - 1;

               double x = (double)i-(distance_x)*G_3(l, u)*G_3(m, v);
               double y = (double)j-(distance_y)*G_3(l, u)*G_3(m, v);

                //选择插值方式
               switch (interpolation) {
               case 0:
                   red = QColor(nearest(x,y,&temp_img)).red() ;
                   green = QColor(nearest(x,y,&temp_img)).green() ;
                   blue = QColor(nearest(x,y,&temp_img)).blue() ;
                   break;
               case 1:
                   red = QColor(bilinear(x,y,&temp_img)).red() ;
                   green = QColor(bilinear(x,y,&temp_img)).green() ;
                   blue = QColor(bilinear(x,y,&temp_img)).blue() ;
                   break;
               case 2:
                   red = QColor(bicubic(x,y,&temp_img)).red() ;
                   green = QColor(bicubic(x,y,&temp_img)).green() ;
                   blue = QColor(bicubic(x,y,&temp_img)).blue() ;
                   break;
               default:
                   red = QColor(nearest(x,y,&temp_img)).red() ;
                   green = QColor(nearest(x,y,&temp_img)).green() ;
                   blue = QColor(nearest(x,y,&temp_img)).blue() ;
                   break;
               }

               result->setPixel(i,j,QColor(red,green,blue).rgb());
           }
       }

       //更新暂存图片
       temp_img = *result;
       for(int i=0; i<img->width(); i++)
       {
           for(int j=0; j<img->height(); j++)
           {
               if(temp_img.pixelColor(i,j).red() == 0)
                   temp_img.setPixel(i,j,img->pixel(i,j));
           }
       }

    }
}

//B样条变形_正向与逆向
void B_spline_whole(int N,QImage *img, QImage *result,int direction)
{
    int row = ceil((double)img->height() / (double)N) + 1;
    int col = ceil((double)img->width() / (double)N) + 1;
    QVector<double> delta_P_x = QVector<double>(row * col, 0);//网格点位移
    QVector<double> delta_P_y = QVector<double>(row * col, 0);
    QVector<double> distance_x = QVector<double>(length, 0);//关键点距离
    QVector<double> distance_y = QVector<double>(length, 0);
    QVector<double> origin_x ;
    QVector<double> target_x;
    QVector<double> origin_y;
    QVector<double> target_y ;
    int time = 0;
    int threshold =200;//精度

    if(direction == 0)//正向
    {

        origin_x = (points_x);
        target_x =  (new_points_x);
        origin_y =  (points_y);
        target_y =  (new_points_y);
    }
    else if(direction == 1)//逆向
    {
        origin_x =  (new_points_x);
        target_x =  (points_x);
        origin_y =  (new_points_y);
        target_y =  (points_y);
    }
    while(1)
    {
        double distance_sum = 0;
        for(int m=0; m<length; m++)
        {
            int i = floor(origin_x[m] / (double)N) - 1;
            int j = floor(origin_y[m] / (double)N) - 1;
            double u = origin_x[m] / (double)N -i - 2;
            double v = origin_y[m] / (double)N -j - 2;
            double delta_x= 0, delta_y = 0;

            //计算关键点位移
            for(int k=0; k<4; k++)
                for(int l=0; l<4; l++)
                {
                    if((i+k-1)*col+j+l-1 >= row * col)
                    {
                        delta_x += G_3(k,u) * G_3(l,v) * delta_P_x[row * col-1];
                        delta_y += G_3(k,u) * G_3(l,v) * delta_P_y[row * col-1];

                    }
                    else if((i+k-1)*col+j+l-1 < 0)
                    {
                        delta_x += G_3(k,u) * G_3(l,v) * delta_P_x[0];
                        delta_y += G_3(k,u) * G_3(l,v) * delta_P_y[0];
                    }
                    else
                    {
                        delta_x += G_3(k,u) * G_3(l,v) * delta_P_x[(i+k-1)*col+j+l-1];
                        delta_y += G_3(k,u) * G_3(l,v) * delta_P_y[(i+k-1)*col+j+l-1];
                    }

                }

            distance_x[m] = target_x[m] - origin_x[m] - delta_x;
            distance_y[m] = target_y[m] - origin_y[m] - delta_y;
            distance_sum += distance_x[m] * distance_x[m] + distance_y[m] * distance_y[m];

            //更新网格点位移
            for(int k=0; k<4; k++)
                for(int l=0; l<4; l++)
                {
                    double change_x = (double)distance_x[m] * G_3(k,u) * G_3(l,v) / 2;
                    double change_y = (double)distance_y[m] * G_3(k,u) * G_3(l,v) / 2;

                    int index;
                    if((i+k-1)*col+j+l-1 >= row * col)
                        index = row * col - 1;
                    else if((i+k-1)*col+j+l-1 < 0)
                        index = 0;
                    else
                    {
                        index =  (i+k-1)*col+j+l-1;
                    }

                    delta_P_x[index] += change_x;
                    delta_P_y[index] += change_y;

                    if(delta_P_x[index] > N)
                        delta_P_x[index] = N;
                    else if(delta_P_x[index] < -N)
                        delta_P_x[index] = -N;

                    if(delta_P_y[index] > N)
                        delta_P_y[index] = N;
                    else if(delta_P_y[index] < -N)
                        delta_P_y[index] = -N;

                }

        }
        //退出循环
        if(distance_sum < threshold)
        {
            //qDebug() << threshold;
             break;
        }

        else if(time < 1000)
        {
            time ++;
        }
        else
        {
            time = 0;
            threshold += 200 ;//降低精度要求
        }
    }

    //映射
    for (int x=0; x<img->width(); x++)
    {
        for(int y=0; y<img->height(); y++)
        {
            int i = floor(x / (double)N) - 1;
            int j = floor(y / (double)N) - 1;
            double u = x / (double)N -i - 2;
            double v = y / (double)N -j - 2;
            double new_x = x, new_y = y;

            for(int k=0; k<4; k++)
                for(int l=0; l<4; l++)
                {

                    if((i+k-1)*col+j+l-1 >= 0 && (i+k-1)*col+j+l-1 < row * col)
                    {
                        new_x += G_3(k,u) * G_3(l,v) * (1.0 *delta_P_x[(i+k-1)*col+j+l-1]);
                        new_y += G_3(k,u) * G_3(l,v) * (1.0 *delta_P_y[(i+k-1)*col+j+l-1]);
                    }

                }

            if(direction == 1)//反向
                result->setPixel(x,y,nearest(new_x,new_y,img));
            else if(direction == 0)//正向
            {
                //寻找最近邻的点
                    int nearest_x,nearest_y;

                    if (new_x >= img->width()-1)//超出图片范围
                        nearest_x = img->width()-1;
                    else if (new_x < 0)
                        nearest_x = 0;
                    else if (new_x-int(new_x) > 0.5)
                        nearest_x = int(new_x)+1;
                    else
                        nearest_x = int(new_x);

                    if (new_y >= img->height()-1)
                        nearest_y = img->height()-1;
                    else if (new_y < 0)
                        nearest_y = 0;
                    else if (new_y-int(new_y) > 0.5)
                        nearest_y = int(new_y)+1;
                    else
                        nearest_y = int(new_y);

                    if(result->pixelColor(nearest_x, nearest_y).red() == 0)
                        result->setPixel(nearest_x, nearest_y,img->pixel(x,y));
                    else
                    {
                        //如果曾经映射过，求均值
                        int red = (img->pixelColor(x,y).red() + result->pixelColor(nearest_x, nearest_y).red()) / 2;
                        int blue = (img->pixelColor(x,y).blue() + result->pixelColor(nearest_x, nearest_y).blue()) / 2;
                        int green = (img->pixelColor(x,y).green() + result->pixelColor(nearest_x, nearest_y).green()) / 2;
                        result->setPixel(nearest_x, nearest_y,
                                         QColor(red,green,blue).rgb()
                                         );
                    }


            }
        }
    }

    //正向映射处理没有映射到的区域
    if(direction == 0)
    {
        QColor temp;
        for (int x=0; x<img->width(); x++)
        {
            for(int y=0; y<img->height(); y++)
            {
                if(result->pixelColor(x,y).red() == 0)
                {
                    //求周围点均值
                    int red=0,green=0,blue=0,count=0;
                    for(int i=-2; i<3; i++)
                        for(int j=-2; j<3; j++)
                        {
                            if(x+i>0 && x+i<img->width() && y+j>0 && y+j<img->height())
                                if(!result->pixelColor(x+i,y+j).red() == 0)
                            {
                                red += result->pixelColor(x+i,y+j).red();
                                green += result->pixelColor(x+i,y+j).green();
                                blue += result->pixelColor(x+i,y+j).blue();
                                count ++;
                            }
                        }
                    if(count > 0)
                    {
                        red = red / count;
                        green = green / count;
                        blue = blue / count;
                        if(red>255 || red<0 || green>255 || green<0 || blue>255 || blue<0)
                            result->setPixel(x,y,temp.rgb());
                        else
                        {
                            //若计算失败，就用上一个有RGB值的点代替它
                            result->setPixel(x,y,QColor(red,green,blue).rgb());
                            temp = QColor(red,green,blue);
                        }
                    }
                    else
                        result->setPixel(x,y,temp.rgb());

                }
                else
                {
                    temp = result->pixelColor(x,y);
                }
            }
        }
    }

}
