#include <iostream>

#include "triangles.hpp"
#include <vector>
#include <set>

using namespace geometry;

int main() {
    int tr_num = 0;

    if (!(std::cin >> tr_num)) {
        std::cout << "Bad triangles amount" << std::endl;
        exit(1);
    }

    std::vector<triangle_t> triangles;
    triangles.reserve(tr_num);

    for (int i = 0; i < tr_num; i++) {
        double crds[9] = {};

        for (int j = 0; j < 9; j++) {
            if (!(std::cin >> crds[j])) {
                std::cout << "Bad coordinates" << std::endl;
                exit(1);
            }
        }
        triangles.push_back({{crds[0], crds[1], crds[2]},
                             {crds[3], crds[4], crds[5]},
                             {crds[6], crds[7], crds[8]}});
    }

    std::set<int> ans;

    for (int i = 0; i < tr_num; i++) {
        for (int j = i + 1; j < tr_num; j++) {
            if (triangles[i].is_intersected(triangles[j])) {
                ans.emplace(i + 1);
                ans.emplace(j + 1);
            }
        }
    }
    for (auto it = ans.begin(); it != ans.end(); it++)
        std::cout << *it << " ";



    //std::cout << "13 - " << triangles[0].is_intersected(triangles[2]) << std::endl;
    // std::cout << "23 - " << triangles[1].is_intersected(triangles[2]) << std::endl;
    // std::cout << "31 - " << triangles[2].is_intersected(triangles[0]) << std::endl;

    return 0;
}

//     triangle_t tr1{{1, 2, 3}, {1, 4, 5}, {1, 10, 11}};
//     triangle_t tr2{{1, 2, 3}, {8, 9, 10} , {16, 22, 58}};
//
//     std::cout << tr1.is_intersected(tr2) << std::endl;

//     line_segment_t lseg{point_t{3, -3, 2}, point_t{2, -2, 4}};
//     line_t          ln1{point_t{3, -3, 2},   vector_t{-1, 1, 2}};
//     line_t          ln2{point_t{-1, 4, -26}, vector_t{3, -4, 6}};
//
//     lseg.get_line_intersection(ln2).print();
//
//     //std::cout << ln1.relatival_location(ln2) << std::endl;

//====================================
//     geometry::point_t pnt1{2, 4, -2};
//     geometry::point_t pnt2{2, 6, 0};
//     geometry::point_t pnt3{2, 4, 1};
//
//     geometry::point_t pnt4{2, 4, -2};
//     geometry::point_t pnt5{2, 6, 0};
//     geometry::point_t pnt6{2, 4, 1};
//
//     geometry::plane_t pln1{pnt1, pnt2, pnt3};
//     geometry::plane_t pln2{pnt4, pnt5, pnt6};
//
//     geometry::line_segment_t lnsg{pnt1, pnt2};
//
//     lnsg.print();
//
//     pln1.print();
//     pln2.print();
//
//     std::cout << "loc = " << pln1.relatival_location(pln2, pnt5) << std::endl;
//
//     geometry::line_t ln = pln1.get_intersection(pln2);
//
//     ln.print();
