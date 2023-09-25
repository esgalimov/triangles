#include <iostream>

#include "triangles.hpp"

int main() {
    geometry::point_t pnt1{3, 4, -2};
    geometry::point_t pnt2{2, 4, 0};
    geometry::point_t pnt3{1, 4, 1};

    geometry::point_t pnt4{1, 1, 1};
    geometry::point_t pnt5{1, 3, 4};
    geometry::point_t pnt6{1, 2, 0};

    geometry::plane_t pln1{pnt1, pnt2, pnt3};
    geometry::plane_t pln2{pnt4, pnt5, pnt6};

    geometry::line_segment_t lnsg{pnt1, pnt2};

    lnsg.print();

    pln1.print();
    pln2.print();

    std::cout << pln1.relatival_location(pln2, pnt5) << std::endl;

    geometry::line_t ln = pln1.get_intersection(pln2);

    ln.print();

    return 0;
}
