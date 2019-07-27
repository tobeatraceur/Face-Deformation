#ifndef TPS_H
#define TPS_H

#include <math.h>
#include <QVector>
#include "decomposition.h"
#include <QFile>
#include <QIODevice>
#include <QImage>
#include <QColor>
#include <qmath.h>
#include <QMessageBox>
#include "interpolation.h"
#include <QDebug>

void solve_equ();

int points_ini(QString file1,QString file2,QVector<double>*,QVector<double>*,QVector<double>*,QVector<double>*);

void calc_Y();

void calc_L();

double base_func(double r_square);

double map_x(int x, int y);

double map_y(int x, int y);

void B_spline(QImage*, QImage *result, int interpolation);

void B_spline_whole(int N, QImage *img, QImage *result, int direction);

static double* coex;//径向基函数中坐标x的系数
static double* coey;//径向基函数中坐标y的系数
static QVector<double> points_x;//待修改人脸的关键点坐标x
static QVector<double> points_y;//待修改人脸的关键点坐标y
static QVector<double> new_points_x;//目标人脸的关键点坐标x
static QVector<double> new_points_y;//目标人脸的关键点坐标y
static int length;//关键点个数
static double **L;//TPS方程中的系数矩阵
static double **Y;

#endif  TPS_H
