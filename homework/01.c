#include <stdio.h>

double integrate(double (*f)(double),double a,double b) {//函数指针，指向一函数，有一double参数，返回值为double
    double step = 0.001;
    double sum = 0.0;
    for(double x=a;x<b;x+=step){
        sum+= f(x)*step;
    }
    return sum;
}

double square(double x) {
    return x*x;
}

int main() {
    printf("integrate(square, 0.0, 2.0)=%f\n", integrate(square, 0.0, 2.0));//0~2的積分
}
