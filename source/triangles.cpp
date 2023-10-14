#include <iostream>

#include "../include/octotree.hpp"
#include <vector>
#include <set>
#include <chrono>

using namespace octotrees;

int main() {
    int tr_num = 0;

    if (!(std::cin >> tr_num)) {
        std::cerr << "Bad triangles amount" << std::endl;
        exit(1);
    }

    std::cout << sizeof(int) << std::endl;

    std::vector<triangle_t> triangles;
    max_min_crds_t max_min_crds;
    //triangles.reserve(tr_num);

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

//     triangle_t tr = triangles[11867];
//
//     for (int i = 0; i < 1000000; i++) {
//         if (i == 11867) continue;
//
//         if (tr.is_intersected(triangles[i]))
//             std::cout << "cock = " << i << std::endl;
//     }

    //std::cout << triangles[11867].is_intersected(triangles[846019]) << std::endl;



    const std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

    octotree_t octotree{triangles, max_min_crds};

    const auto octree_end = std::chrono::high_resolution_clock::now();
    std::cout << "octree time = " << std::chrono::duration_cast<std::chrono::milliseconds>(octree_end - start).count() / 1000.0;

    std::set<int> ans = octotree.get_intersections();

    for (auto it = ans.begin(); it != ans.end(); it++)
        std::cout << *it << std::endl;

    const auto end = std::chrono::high_resolution_clock::now();
    const auto mk_s = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    const double s = mk_s / 1000.0;
    std::cout << "all time in sec - " << s << std::endl;

//846019

    return 0;
}
