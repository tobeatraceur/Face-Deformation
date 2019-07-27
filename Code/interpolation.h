#ifndef INTERPOLATION_H
#define INTERPOLATION_H
#include <QImage>
QRgb nearest(double,double,QImage*);

QRgb bilinear(double,double,QImage*);

double S(double);

QRgb bicubic(double,double,QImage*);
#endif // INTERPOLATION_H
