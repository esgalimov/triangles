#include <iostream>

#include "triangles.hpp"

int main() {
    point_t pnt1{0, 0, 0};
    point_t pnt2{1, 0, 0};
    point_t pnt3{0, 1, 0};

    line_t ln{pnt1, pnt2};

    plane_t pln{pnt1, pnt2, pnt3};

    triangle_t tr{pnt1, pnt2, pnt3};

    ln.print();
    pln.print();
    tr.print();

    return 0;
}
