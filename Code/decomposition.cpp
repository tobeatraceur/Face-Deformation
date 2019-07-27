#include "decomposition.h"
#include "decomposition.h"
#include <iostream>
#include <cmath>

using namespace std;



//解方程函数


//分解
void qr(double** a, double* d, int n){
    int i = 0, j = 0, k = 0, l = 0;
    double tempro = 0.0, norm = 0.0, *temp = new double[n];

    for(i = 0; i < n - 1; ++i){
        norm = 0.0;
        for(j = i; j < n; ++j){
            norm += a[j][i] * a[j][i];
        }

        if(a[i][i] > 0){
            norm = 0.0 - sqrt(norm);
        }
        else{
            norm = sqrt(norm);
        }

        tempro = 0.0;
        *(d + i) = norm;
        a[i][i] = a[i][i] - norm;

        for(j = i; j <= n - 1; ++j){
            tempro += a[j][i] * a[j][i];
        }
        tempro = sqrt(tempro);

        for(j = i; j <= n - 1; ++j){
            a[j][i] /= tempro;
        }

        for(k = i + 1; k < n; ++k){
            for(j = i; j < n; j++){
                tempro = 0.0;
                for(l = i; l < n; ++l){
                    tempro += a[j][i] * a[l][i] * a[l][k];
                }
                *(temp + j) = a[j][k] - 2 * tempro;
            }
            for(j = i; j < n; ++j){
                a[j][k] = *(temp + j);
            }
        }
    }
    *(d + n - 1) = a[n-1][n-1];
    delete [] temp;
}

//求解
bool hshld(double** qr, double const* d, double* b, int n){
    int i = 0, j = 0, l = 0;
    double buffer = 0.0, *temp = new double[n];

    for(i = 0; i < n - 1; ++i){
        for(j = i; j < n; ++j){
            buffer = 0.0;
            for(l = i; l < n; ++l){
                buffer += qr[l][i] * qr[j][i] * *(b + l);
            }
            *(temp + j) = *(b + j) - 2 * buffer;
        }

        for(j = i; j < n; ++j){
            *(b + j) = *(temp + j);
        }
    }

    for(j = n - 1; j > -1; --j){
        for(l = n - 1; l != j; --l){
            *(b + j) -= *(b + l) * qr[j][l];
        }
        if(*(d + j) == 0.0){
            return true;
        }
        *(b + j) /= *(d + j);
    }

    return false;
}
