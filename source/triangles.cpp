#include <iostream>

#include "../include/octotree.hpp"
#include <vector>
#include <set>

using namespace octotrees;

int main() {
    int tr_num = 0;

    if (!(std::cin >> tr_num)) {
        std::cerr << "Bad triangles amount" << std::endl;
        exit(1);
    }

    std::vector<triangle_t> triangles;
    max_min_crds_t max_min_crds;
    triangles.reserve(tr_num);

    for (int i = 0; i < tr_num; i++) {
        double crds[9] = {};

        for (int j = 0; j < 9; j++) {
            if (!(std::cin >> crds[j])) {
                std::cerr << "Bad coordinates" << std::endl;
                exit(1);
            }
        }
        max_min_crds.update(crds[0], crds[1], crds[2]);
        max_min_crds.update(crds[3], crds[4], crds[5]);
        max_min_crds.update(crds[6], crds[7], crds[7]);

        triangles.push_back({{crds[0], crds[1], crds[2]},
                             {crds[3], crds[4], crds[5]},
                             {crds[6], crds[7], crds[8]}});
    }

    octotree_t octotree{triangles, max_min_crds};

    //octotree.print();

    octotree.get_intersections();
    octotree.print_intersections();

//     std::set<int> ans;
//
//     for (int i = 0; i < tr_num; i++) {
//         for (int j = i + 1; j < tr_num; j++) {
//             if (triangles[i].is_intersected(triangles[j])) {
//                 ans.emplace(i);
//                 ans.emplace(j);
//             }
//         }
//     }
//     for (auto it = ans.begin(); it != ans.end(); it++)
//         std::cout << *it << " ";

    return 0;
}
