#pragma once

#include <list>
#include <set>
#include <vector>
#include<limits>
#include "triangles.hpp"

namespace octotrees {

    using namespace triangles;

    const char CHILD_NUM = 8;

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

        class octonode_t {
            std::array<octonode_t*, 8> children_ = {nullptr};
            std::list<id_trian_t> triangles_;
            unsigned int active_nodes_;
            octonode_t *parent_;
            bool is_leaf_;

            using ans_set_t = typename std::set<int>;

            public:
                const point_t center_;
                const double  radius_;

                int cal = 0;

                octonode_t(const point_t &center, double radius = NAN, octonode_t *parent = nullptr) :
                center_(center), radius_(radius), active_nodes_(0), is_leaf_(true), parent_(parent) {
                    // сделать адекватный рассчет минимальной ячейки
                    if (radius < 1000) return;

                    double next_rad = radius / 2;

                    children_[0] = new octonode_t{{center.x_ + next_rad, center.y_ + next_rad, center.z_ + next_rad}, next_rad, this};
                    children_[1] = new octonode_t{{center.x_ + next_rad, center.y_ + next_rad, center.z_ - next_rad}, next_rad, this};
                    children_[2] = new octonode_t{{center.x_ + next_rad, center.y_ - next_rad, center.z_ - next_rad}, next_rad, this};
                    children_[3] = new octonode_t{{center.x_ + next_rad, center.y_ - next_rad, center.z_ + next_rad}, next_rad, this};
                    children_[4] = new octonode_t{{center.x_ - next_rad, center.y_ + next_rad, center.z_ + next_rad}, next_rad, this};
                    children_[5] = new octonode_t{{center.x_ - next_rad, center.y_ + next_rad, center.z_ - next_rad}, next_rad, this};
                    children_[6] = new octonode_t{{center.x_ - next_rad, center.y_ - next_rad, center.z_ - next_rad}, next_rad, this};
                    children_[7] = new octonode_t{{center.x_ - next_rad, center.y_ - next_rad, center.z_ + next_rad}, next_rad, this};

                    is_leaf_ = false;
                }

                ~octonode_t() {
                    if (!is_leaf_) {
                        for (int i = 0; i < CHILD_NUM; i++) {
                            delete children_[i];
                        }
                    }
                }

                void emplace_tr(int id, const triangle_t &tr) {
                    if (is_leaf_) {
                        triangles_.emplace_back(id, tr);
                        return;
                    }
                    for (int i = 0; i < CHILD_NUM; i++) {
                        if (tr.is_in_square(children_[i]->center_, children_[i]->radius_)) {
                                active_nodes_ |= (1 << i);
                            children_[i]->emplace_tr(id, tr);
                            return;
                        }
                    }
                    triangles_.emplace_back(id, tr);
                }

                ans_set_t get_intersections() {
                    ans_set_t ans;

                    for (auto it = triangles_.begin(); it != triangles_.end(); it++) {
                        for (auto jt = (++it)--; jt != triangles_.end(); jt++) {
                            if (it->tr.is_intersected(jt->tr)) {
                                ans.emplace(it->id);
                                ans.emplace(jt->id);
                            }
                        }
                    }
                    if (is_leaf_ || !active_nodes_) return ans;

                    ans_set_t ch_inter;

                    for (auto it = triangles_.begin(); it != triangles_.end(); it++) {
                        ch_inter = get_children_intersections(*it);
                        ans.insert(ch_inter.begin(), ch_inter.end());
                    }

                    for (int i = 0; i < CHILD_NUM; i++) {

                        if (!(active_nodes_ & (1 << i))) continue;

                        ch_inter = children_[i]->get_intersections();
                        ans.insert(ch_inter.begin(), ch_inter.end());
                    }

                    return ans;
                }

                ans_set_t get_children_intersections(const id_trian_t& par_tr) {
                    ans_set_t ans;

                    if (is_leaf_ || !active_nodes_) return ans;

                    for (int i = 0; i < CHILD_NUM; i++) {
                        if (!(active_nodes_ & (1 << i))) continue;

                        for (auto it = children_[i]->triangles_.begin(); it != children_[i]->triangles_.end(); it++) {
                            if (par_tr.tr.is_intersected(it->tr)) {
                                ans.emplace(it->id);
                            }
                        }
                        ans_set_t ch_inter = children_[i]->get_children_intersections(par_tr);
                        ans.insert(ch_inter.begin(), ch_inter.end());
                    }
                    if (!ans.empty()) ans.emplace(par_tr.id);

                    return ans;
                }

                void print() const {
                    std::cout << "center: " << std::endl;
                    center_.print();
                    std::cout << "radius = " << radius_ << std::endl;
                }

                int cnt() const {
                    if (is_leaf_) return triangles_.size();
                    int cock = 0;
                    for (int i = 0; i < CHILD_NUM; i++) {
                        if (!(active_nodes_ & (1 << i))) continue;

                        cock += children_[i]->cnt();
                    }
                    return cock + triangles_.size();
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
                if (radius3 > radius1) radius1 = radius3;

                root_ = new octonode_t{{(crds.x_max + crds.x_min) / 2,
                                        (crds.y_max + crds.y_min) / 2,
                                        (crds.z_max + crds.z_min) / 2}, radius1};

                for (int i = 0; i < trs.capacity(); i++)
                    root_->emplace_tr(i, trs[i]);
            }
            ~octotree_t() { delete root_; }

            void get_intersections() {
                intersections = root_->get_intersections();
            }

            void print_intersections() const {
                for (auto it = intersections.begin(); it != intersections.end(); it++)
                    std::cout << *it << " ";
            }

            void print() const { root_->print(); }
    };


}
