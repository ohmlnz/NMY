#include "Math.h"

double easeOutBounce(double t) {
    return 1 - pow(2, -6 * t) * fabs(cos(t * PI * 3.5));
}

double easeInElastic(double t) {
    double t2 = t * t;
    return t2 * t2 * sin(t * PI * 4.5);
}