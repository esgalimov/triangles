#pragma once

#include <list>
#include <set>
#include <vector>
#include <limits>
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
            std::array<std::list<octonode_t>::pointer, 8> children_ = {nullptr};
            std::list<id_trian_t> triangles_;

            unsigned int active_nodes_ = 0;
            bool is_leaf_ = true;

            const point_t center_;
            const double  radius_;

            public:
                octonode_t(const point_t &center, double radius = NAN) :
                center_(center), radius_(radius) {}

                void update_trs(std::list<octonode_t> &nodes) {
                    if (triangles_.size() < MAX_TRS_NODE) return;

                    double next_rad = radius_ / 2;

                    for (int i = 0; i < CHILD_NUM; ++i) {
                        double pnt_x = center_.x_ + ((i      & 1) ? next_rad : -next_rad),
                               pnt_y = center_.y_ + ((i << 1 & 2) ? next_rad : -next_rad),
                               pnt_z = center_.z_ + ((i << 2 & 4) ? next_rad : -next_rad);

                        nodes.emplace_back(point_t{pnt_x, pnt_y, pnt_z}, next_rad);
                        children_[i] = &nodes.back();
                    }

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
                            children_[i]->update_trs(nodes);
                        }
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

                        if (!par_tr.tr.is_part_in_cube(children_[i]->center_, children_[i]->radius_)) continue;

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

        std::list<octonode_t>::pointer root_ = nullptr;

        std::list<octonode_t> nodes_;

        ans_set_t intersections_;

        public:
            octotree_t(const std::vector<triangle_t> &trs, const max_min_crds_t &crds) {
                double radius1 = std::abs((crds.x_max - crds.x_min) / 2),
                       radius2 = std::abs((crds.y_max - crds.y_min) / 2),
                       radius3 = std::abs((crds.z_max - crds.z_min) / 2);

                if (radius2 > radius1) radius1 = radius2;
                if (radius3 > radius1) radius1 = radius3;

                nodes_.emplace_back(point_t{(crds.x_max + crds.x_min) / 2,
                                            (crds.y_max + crds.y_min) / 2,
                                            (crds.z_max + crds.z_min) / 2}, radius1);

                root_ = &nodes_.back();

                for (int i = 0; i < trs.capacity(); i++)
                    root_->add_tr(i, trs[i]);
                root_->update_trs(nodes_);
            }

            ans_set_t get_intersections() {
                root_->get_intersections(intersections_);
                return intersections_;
            }
    };
}
