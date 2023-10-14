#pragma once

#include <list>
#include <set>
#include <vector>
#include<limits>
#include "triangles.hpp"

namespace octotrees {

    using namespace triangles;

    const char CHILD_NUM = 8;
    const int  MAX_TRS_NODE = 1000;

    struct max_min_crds_t
    {
        double x_min = std::numeric_limits<double>::infinity();
        double x_max = -x_min;

        double y_min = std::numeric_limits<double>::infinity();
        double y_max = -y_min;

        double z_min = std::numeric_limits<double>::infinity();
        double z_max = -z_min;

        void update(double x, double y, double z) {
            if (x < x_min) x_min = x;
            if (x > x_max) x_max = x;
            if (y < y_min) y_min = y;
            if (y > y_max) y_max = y;
            if (z < z_min) z_min = z;
            if (z > z_max) z_max = z;
        }
    };

    class octotree_t {

        struct id_trian_t {
            int id;
            triangle_t tr;
            id_trian_t(int id_, triangle_t tr_) : id(id_), tr(tr_) {}
        };

        using ans_set_t = typename std::set<int>;

        class octonode_t {
            std::array<octonode_t*, 8> children_ = {nullptr};
            std::list<id_trian_t> triangles_;

            unsigned int active_nodes_ = 0;
            bool is_leaf_ = true;

            const point_t center_;
            const double  radius_;

            public:
                octonode_t(const point_t &center, double radius = NAN) :
                center_(center), radius_(radius) {}

                ~octonode_t() {
                    if (!is_leaf_) {
                        for (int i = 0; i < CHILD_NUM; i++) {
                            delete children_[i];
                        }
                    }
                }

                void update_trs() {
                    if (triangles_.size() < MAX_TRS_NODE) return;

                    double next_rad = radius_ / 2;

                    children_[0] = new octonode_t{{center_.x_ + next_rad, center_.y_ + next_rad, center_.z_ + next_rad}, next_rad};
                    children_[1] = new octonode_t{{center_.x_ + next_rad, center_.y_ + next_rad, center_.z_ - next_rad}, next_rad};
                    children_[2] = new octonode_t{{center_.x_ + next_rad, center_.y_ - next_rad, center_.z_ - next_rad}, next_rad};
                    children_[3] = new octonode_t{{center_.x_ + next_rad, center_.y_ - next_rad, center_.z_ + next_rad}, next_rad};
                    children_[4] = new octonode_t{{center_.x_ - next_rad, center_.y_ + next_rad, center_.z_ + next_rad}, next_rad};
                    children_[5] = new octonode_t{{center_.x_ - next_rad, center_.y_ + next_rad, center_.z_ - next_rad}, next_rad};
                    children_[6] = new octonode_t{{center_.x_ - next_rad, center_.y_ - next_rad, center_.z_ - next_rad}, next_rad};
                    children_[7] = new octonode_t{{center_.x_ - next_rad, center_.y_ - next_rad, center_.z_ + next_rad}, next_rad};

                    int sz_before = triangles_.size();
                    auto it = triangles_.begin();

                    while (it != triangles_.end()) {
                        bool tr_remind = true;
                        for (int i = 0; i < CHILD_NUM; i++)
                            if (it->tr.is_in_cube(children_[i]->center_, children_[i]->radius_)) {
                                children_[i]->triangles_.emplace_back(*it);
                                triangles_.erase(it++);
                                tr_remind = false;
                                break;
                            }
                        if (tr_remind) it++;
                    }
                    if (sz_before - triangles_.size()) is_leaf_ = false;

                    for (int i = 0; i < CHILD_NUM; i++) {
                        if (!children_[i]->triangles_.empty()) {
                            active_nodes_ |= (1 << i);
                            children_[i]->update_trs();
                        }
                        else delete children_[i];
                    }
                }

                void add_tr(int id, const triangle_t &tr) {
                    triangles_.emplace_back(id, tr);
                }

                void get_intersections(ans_set_t &ans) {
                    for (auto it = triangles_.begin(); it != triangles_.end(); it++) {
                        for (auto jt = std::next(it); jt != triangles_.end(); jt++) {
                            if (it->tr.is_intersected(jt->tr)) {
                                ans.insert({it->id, jt->id});
                            }
                        }
                    }
                    if (is_leaf_ || !active_nodes_) return;

                    for (auto it = triangles_.begin(); it != triangles_.end(); it++) {
                        get_children_intersections(*it, ans);
                    }

                    for (int i = 0; i < CHILD_NUM; i++) {
                        if (!(active_nodes_ & (1 << i))) continue;

                        children_[i]->get_intersections(ans);
                    }
                }

                void get_children_intersections(const id_trian_t& par_tr, ans_set_t &ans) {
                    if (is_leaf_ || !active_nodes_) return;

                    int sz_before = ans.size();

                    for (int i = 0; i < CHILD_NUM; i++) {
                        if (!(active_nodes_ & (1 << i))) continue;

                        for (auto it = children_[i]->triangles_.begin(); it != children_[i]->triangles_.end(); it++) {
                            if (par_tr.tr.is_intersected(it->tr)) {
                                ans.emplace(it->id);
                            }
                        }
                        children_[i]->get_children_intersections(par_tr, ans);
                    }
                    if (sz_before - ans.size()) ans.emplace(par_tr.id);
                }
        };

        octonode_t* root_ = nullptr;

        ans_set_t intersections;

        using it_tr = typename std::list<id_trian_t>::iterator;

        public:
            octotree_t(const std::vector<triangle_t> &trs, const max_min_crds_t &crds) {
                double radius1 = std::abs((crds.x_max - crds.x_min) / 2),
                       radius2 = std::abs((crds.y_max - crds.y_min) / 2),
                       radius3 = std::abs((crds.z_max - crds.z_min) / 2);

                if (radius2 > radius1) radius1 = radius2;
                if (radius3 > radius1) radius1 = radius3;

                root_ = new octonode_t{{(crds.x_max + crds.x_min) / 2,
                                        (crds.y_max + crds.y_min) / 2,
                                        (crds.z_max + crds.z_min) / 2}, radius1};

                for (int i = 0; i < trs.capacity(); i++)
                    root_->add_tr(i, trs[i]);
                root_->update_trs();
            }
            ~octotree_t() { delete root_; }

            ans_set_t get_intersections() {
                root_->get_intersections(intersections);
                return intersections;
            }
    };
}
