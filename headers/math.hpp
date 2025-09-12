#include <cmath>

template <class type>
inline type sq(type x) {return x*x;}

inline double dist(double dx, double dy) {
    return sqrt(sq(dx) + sq(dy));
}