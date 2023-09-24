#include <iostream>

#include "triangles.hpp"

int main() {
    point_t pnt1{0, 0, 0};
    point_t pnt2{1, 0, 0};
    point_t pnt3{1, 1, 1};

    point_t pnt4{2, 1, 1};
    point_t pnt5{1, 3, 4};
    point_t pnt6{9, 2, 0};

    plane_t pln1{pnt1, pnt2, pnt3};
    plane_t pln2{pnt4, pnt5, pnt6};

    line_t ln = pln1.planes_intersection(pln2);

    ln.print();

    return 0;
}
