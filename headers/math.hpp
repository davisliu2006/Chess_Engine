#include <cmath>

template <class type>
inline type sq(type x) {return x*x;}

inline double dist(double x0, double y0, double x1, double y1) {
    return sqrt(sq(x1-x0) + sq(y1-y0));
}