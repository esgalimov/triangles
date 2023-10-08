#pragma once

#include <list>
#include <set>
#include <vector>
#include "triangles.hpp"

namespace octotrees {

    using namespace triangles;

    const int CHILD_NUM = 8;

    struct max_min_crds_t
    {
        double x_min = NAN;
        double x_max = NAN;

        double y_min = NAN;
        double y_max = NAN;

        double z_min = NAN;
        double z_max = NAN;
    }

    class octotree_t {

        struct id_trian_t {
            int id;
            triangle_t tr;
        }

        class octonode_t {
            std::array<octonode_t*, 8> children_;
            std::list<id_trian_t> triangles_;
            char active_nodes_;
            octonode_t *parent_;

            public:
                const point_t center_;
                const double  radius_;
                bool is_leaf_;

                octonode_t(const point_t &center, double radius = NAN, octonode_t *parent_ = nullptr) :
                center_(center), radius_(radius), active_nodes_(0), is_leaf_(true) {
                    if (radius < 10) return;

                    double next_rad = radius / 2;

                    children_[0] = new octonode_t{{center.x_ + next_rad, center.y_ + next_rad, center.z_ + next_rad}, next_rad, this};
                    children_[1] = new octonode_t{{center.x_ + next_rad, center.y_ + next_rad, center.z_ - next_rad}, next_rad, this};
                    children_[2] = new octonode_t{{center.x_ + next_rad, center.y_ - next_rad, center.z_ - next_rad}, next_rad, this};
                    children_[3] = new octonode_t{{center.x_ + next_rad, center.y_ - next_rad, center.z_ + next_rad}, next_rad, this};
                    children_[4] = new octonode_t{{center.x_ - next_rad, center.y_ + next_rad, center.z_ + next_rad}, next_rad, this};
                    children_[5] = new octonode_t{{center.x_ - next_rad, center.y_ + next_rad, center.z_ - next_rad}, next_rad, this};
                    children_[6] = new octonode_t{{center.x_ - next_rad, center.y_ - next_rad, center.z_ - next_rad}, next_rad, this};
                    children_[7] = new octonode_t{{center.x_ - next_rad, center.y_ - next_rad, center.z_ + next_rad}, next_rad, this};

                    have_child_ = false;
                }

                ~octonode_t() {
                    if (!is_leaf_) {
                        for (int i = 0; i < CHILD_NUM; i++) {
                            children_[i]->~octonode_t();
                        }
                    }
                    delete this;
                }

                void emplace_tr(int id, const triangle_t &tr) {
                    if (!have_child_) {
                        triangles_.emplace_back({id, tr});
                        return;
                    }
                    for (int i = 0; i < CHILD_NUM; i++) {
                        if (tr.is_in_square(children_[i].center_, children_[i].radius_)) {
                            parent_->active_nodes_ |= 1 << i;
                            children_[i].emplace_tr(id, tr);
                            return;
                        }
                    }
                    triangles_.emplace_back({id, tr});
                }
        };

        octonode_t* root_ = nullptr;

        std::set<int> intersections;

        using it_tr = typename std::list<id_trian_t>::iterator;

        public:
            octotree_t(const std::vector<triangle_t> &trs, const max_min_crds_t &crds) {
                double radius1 = std::abs((crds.x_max - crds.x_min) / 2),
                       radius2 = std::abs((crds.y_max - crds.y_min) / 2),
                       radius3 = std::abs((crds.z_max - crds.z_min) / 2);

                if (radius2 > radius1) radius1 = radius2;
                if (radius3 > radius1) radius3 = radius3;

                root_ = new octonode_t{{(crds.x_max - crds.x_min) / 2,
                                       (crds.y_max - crds.y_min) / 2,
                                       (crds.z_max - crds.z_min) / 2}, radius1};

                for (int i = 0; i < trs.capacity(); i++)
                    root->emplace_tr(i, trs[i]);
            }
            ~octotree_t() { root->~octonode_t(); }

            void get_intersections() {

            }
    };


}
