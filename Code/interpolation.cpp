#include "interpolation.h"



//插值函数



//最近邻插值
QRgb nearest(double x,double y,QImage* img)
{
    int new_x,new_y;

    if (x >= img->width()-1)//超出图片范围
        new_x = img->width()-1;
    else if (x < 0)
        new_x = 0;
    else if (x-int(x) > 0.5)
        new_x = int(x)+1;
    else
        new_x = int(x);

    if (y >= img->height()-1)
        new_y = img->height()-1;
    else if (y < 0)
        new_y = 0;
    else if (y-int(y) > 0.5)
        new_y = int(y)+1;
    else
        new_y = int(y);

    return img->pixel(new_x,new_y);
}

//双线性插值
QRgb bilinear(double x,double y,QImage* img)
{
    QVector<double> color_x1(3),color_x2(3);
    QVector<int> color_final(3);

    if (int(x)<=0 || int(y)<=0 || int(x)>=img->width()-1 || int(y)>=img->height()-1)//超出范围
        return nearest(x,y,img);
    else
    {
        //在x方向上线性插值
        color_x1[0] = img->pixelColor(int(x),int(y)).red() * (1-x+int(x)) +
                img->pixelColor(int(x)+1,int(y)).red() * (x-int(x));
        color_x1[1] = img->pixelColor(int(x),int(y)).green() * (1-x+int(x)) +
                img->pixelColor(int(x)+1,int(y)).green() * (x-int(x));
        color_x1[2] = img->pixelColor(int(x),int(y)).blue() * (1-x+int(x)) +
                img->pixelColor(int(x)+1,int(y)).blue() * (x-int(x));

        color_x2[0] = img->pixelColor(int(x),int(y)+1).red() * (1-x+int(x)) +
                img->pixelColor(int(x)+1,int(y)+1).red() * (x-int(x));
        color_x2[1] = img->pixelColor(int(x),int(y)+1).green() * (1-x+int(x)) +
                img->pixelColor(int(x)+1,int(y)+1).green() * (x-int(x));
        color_x2[2] = img->pixelColor(int(x),int(y)+1).blue() * (1-x+int(x)) +
                img->pixelColor(int(x)+1,int(y)+1).blue() * (x-int(x));

        //在y方向上线性插值
        color_final[0] = color_x1[0] * (1-y+int(y)) +
                color_x2[0] * (y-int(y));
        color_final[1] = color_x1[1] * (1-y+int(y)) +
                color_x2[1] * (y-int(y));
        color_final[2] = color_x1[2] * (1-y+int(y)) +
                color_x2[2] * (y-int(y));

        //计算结果超出RGB范围用最近邻近似
        if(int(color_final[0])>255 || int(color_final[0])<0 || int(color_final[1])>255 || int(color_final[1])<0 || int(color_final[2])>255 || int(color_final[2])<0)
            return nearest(x,y,img);
        else
            return QColor(color_final[0],color_final[1],color_final[2]).rgb();
    }

}

//双三次插值基函数
double S(double x)
{
    if (fabs(x) <= 1)
        return 1-2*x*x+fabs(x*x*x);
    else if (fabs(x)>1 && fabs(x)<2)
        return 4-8*fabs(x)+5*x*x-fabs(x*x*x);
    else
        return 0;
}

//双三次插值
QRgb bicubic(double x,double y,QImage* img)
{
    int i = int(x);
    int j = int(y);
    double u = x - i;
    double v = y - j;
    QVector<double> temp_r(4,0),temp_g(4,0),temp_b(4,0),color_final(3,0);

    if(i-1>0 && i+2<img->width() && j-1>0 && j-1+4 <img->height())
    {
        for (int f=0; f<4; f++)
        {
             for (int k=1; k>=-2; k--)
             {
                 temp_r[f] += S(v+k) * img->pixelColor(i-k,j-1+f).red();
                 temp_g[f] += S(v+k) * img->pixelColor(i-k,j-1+f).green();
                 temp_b[f] += S(v+k) * img->pixelColor(i-k,j-1+f).blue();
             }
        }

        for (int m=0; m<4; m++)
        {
            color_final[0] += temp_r[m] * S(u-m+1);
            color_final[1] += temp_g[m] * S(u-m+1);
            color_final[2] += temp_b[m] * S(u-m+1);
        }
        //计算结果超出RGB范围用最近邻近似
        if(int(color_final[0])>255 || int(color_final[0])<0 || int(color_final[1])>255 || int(color_final[1])<0 || int(color_final[2])>255 || int(color_final[2])<0)
            return nearest(x,y,img);
        else
            return QColor(int(color_final[0]),int(color_final[1]),int(color_final[2])).rgb();
    }
    else
        return nearest(x,y,img);//超出范围
}
