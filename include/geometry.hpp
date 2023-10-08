#pragma once

#include <cassert>
#include <iostream>
#include "double_funcs.hpp"

namespace geometry {

using namespace double_funcs;

enum location_t {COINCIDE, PARALLEL, INTERSECT, INTERBR};

class point_t {
    public:
        const double x_, y_, z_;

        point_t(double x = NAN, double y = NAN, double z = NAN) : x_(x), y_(y), z_(z) {};

        point_t(const point_t &pnt) : x_(pnt.x_), y_(pnt.y_), z_(pnt.z_) {};

        bool operator==(const point_t &pnt) const {
            return (equal(x_, pnt.x_) && equal(y_, pnt.y_) && equal(z_, pnt.z_));
        }

        bool operator!=(const point_t &pnt) const { return !(*this == pnt); }

        bool is_in_square(const point_t &cntr, double radius) const {
            return std::abs(x_ - cntr.x_) < radius &&
                   std::abs(y_ - cntr.y_) < radius &&
                   std::abs(z_ - cntr.z_) < radius;
        }

        bool is_valid() const {
            return !(std::isnan(x_) && std::isnan(y_) && std::isnan(z_));
        }

        void print() const {
            std::cout << "point(" << x_ << ";" << y_ << ";" << z_ << ")" << std::endl;
        }
};

class vector_t {
    double x_, y_, z_;

    public:
        vector_t(double x = NAN, double y = NAN, double z = NAN) : x_(x), y_(y), z_(z) {};

        vector_t(const point_t &pnt1, const point_t &pnt2) :
            x_(pnt1.x_ - pnt2.x_), y_(pnt1.y_ - pnt2.y_), z_(pnt1.z_ - pnt2.z_) {}

        double scalar_multiply(const vector_t &vec) const {
            return x_ * vec.x_ + y_ * vec.y_ + z_ * vec.z_;
        }

        vector_t operator+(const vector_t &vec) const  { return {x_ + vec.x_, y_ + vec.y_, z_ + vec.z_}; }

        vector_t operator-(const vector_t &vec) const { return {x_ - vec.x_, y_ - vec.y_, z_ - vec.z_}; }

        vector_t operator*(const vector_t &vec) const {
            return {y_ * vec.z_ - z_ * vec.y_,
                    z_ * vec.x_ - x_ * vec.z_,
                    x_ * vec.y_ - y_ * vec.x_};
        }

        vector_t& operator=(const vector_t & vec) {
            x_ = vec.x_;
            y_ = vec.y_;
            z_ = vec.z_;
            return *this;
        }

        vector_t operator/(double num) const { return {x_ / num, y_ / num, z_ / num}; }

        bool operator==(const point_t &vec) const {
            return (equal(x_, vec.x_) && equal(y_, vec.y_) && equal(z_, vec.z_));
        }

        bool operator!=(const point_t &vec) const { return !(*this == vec); }

        double len() const {
            assert(is_valid());
            return std::sqrt(std::pow(x_, 2) + std::pow(y_, 2) + std::pow(z_, 2));
        }

        bool is_collinear(const vector_t &vec) const {
            return equal((*this * vec).len(), 0);
        }

        bool is_co_dir(const vector_t &vec) const {
            return is_collinear(vec) && scalar_multiply(vec) > 0;
        }

        bool is_valid() const {
            return !(std::isnan(x_) && std::isnan(y_) && std::isnan(z_));
        }

        double get_x() const { return x_; }
        double get_y() const { return y_; }
        double get_z() const { return z_; }

        void print() const {
            std::cout << "vector(" << x_ << ";" << y_ << ";" << z_ << ")" << std::endl;
        }
};


class line_t {
    public:
        const point_t  pnt;
        const vector_t dir_vec;

        line_t(const point_t &pnt, const vector_t &vec) :
            pnt(pnt), dir_vec(vec) {};

        void print() const {
            std::cout << "line:" << std::endl;
            pnt.print();
            dir_vec.print();
        };

        location_t relatival_location(const line_t &ln) const {
            assert(is_valid() && ln.is_valid());

            point_t ln_pnt = ln.pnt;

            vector_t cross_ln_vec{pnt, ln.pnt};

            if (equal((dir_vec * ln.dir_vec).scalar_multiply(cross_ln_vec), 0)) {
                if (dir_vec.is_collinear(ln.dir_vec)) {
                    if (dir_vec.is_collinear(cross_ln_vec)) return COINCIDE;

                    else return PARALLEL;
                }
                else return INTERSECT;
            }
            else return INTERBR;
        }

        bool is_valid() const {
            return pnt.is_valid() && dir_vec.is_valid();
        }
};

class line_segment_t {
    public:
        const point_t pnt1;
        const point_t pnt2;
        const line_t  line;

        line_segment_t(const point_t &p1 = point_t{}, const point_t &p2 = point_t{}) :
            pnt1(p1), pnt2(p2), line({p1, {p1, p2}}) {}

        line_segment_t get_line_intersection(const line_t &ln) const {
            assert(is_valid() && ln.is_valid());

            location_t rel_loc = line.relatival_location(ln);

            if (rel_loc == COINCIDE) return *this;

            else if (rel_loc == INTERSECT) {
                double det_12 = -line.dir_vec.get_x() * ln.dir_vec.get_y() + line.dir_vec.get_y() * ln.dir_vec.get_x();
                double det_23 = -line.dir_vec.get_y() * ln.dir_vec.get_z() + line.dir_vec.get_z() * ln.dir_vec.get_y();
                double det_13 = -line.dir_vec.get_x() * ln.dir_vec.get_z() + line.dir_vec.get_z() * ln.dir_vec.get_x();

                double r_side_1 = ln.pnt.x_ - line.pnt.x_;
                double r_side_2 = ln.pnt.y_ - line.pnt.y_;
                double r_side_3 = ln.pnt.z_ - line.pnt.z_;

                double t_par = NAN;

                if (!equal(det_12, 0)) {
                    t_par = (r_side_1 * (-ln.dir_vec.get_y()) - r_side_2 * (-ln.dir_vec.get_x())) / det_12;
                }
                else if (!equal(det_23, 0)) {
                    t_par = (r_side_2 * (-ln.dir_vec.get_z()) - r_side_3 * (-ln.dir_vec.get_y())) / det_23;
                }
                else {
                    t_par = (r_side_1 * (-ln.dir_vec.get_z()) - r_side_3 * (-ln.dir_vec.get_x())) / det_13;
                }

                point_t inter_pnt{line.dir_vec.get_x() * t_par + line.pnt.x_,
                                  line.dir_vec.get_y() * t_par + line.pnt.y_,
                                  line.dir_vec.get_z() * t_par + line.pnt.z_};

                if (is_line_pnt_on_ln_seg(inter_pnt))
                    return {inter_pnt, inter_pnt};
                else
                    return {};
            }
            else return {};
        }

        bool is_ln_seg_intersected(const line_segment_t &ln_seg) const {
            line_segment_t ln_seg_int = get_line_intersection(ln_seg.line);

            if (ln_seg_int.is_valid()) {
                if (ln_seg_int.pnt1 == ln_seg_int.pnt2)
                    return ln_seg.is_line_pnt_on_ln_seg(pnt1);

                return is_line_pnt_on_ln_seg(ln_seg.pnt1) ||
                        is_line_pnt_on_ln_seg(ln_seg.pnt2);
            }
            return false;
        }

        bool is_line_pnt_on_ln_seg(const point_t &pnt) const {
            return equal(std::abs(pnt.x_  - pnt1.x_) +
                         std::abs(pnt.x_  - pnt2.x_),
                         std::abs(pnt1.x_ - pnt2.x_))  &&

                   equal(std::abs(pnt.y_  - pnt1.y_) +
                         std::abs(pnt.y_  - pnt2.y_),
                         std::abs(pnt1.y_ - pnt2.y_))  &&

                   equal(std::abs(pnt.z_  - pnt1.z_) +
                         std::abs(pnt.z_  - pnt2.z_),
                         std::abs(pnt1.z_ - pnt2.z_));
        }

        bool is_zero_len() const { return pnt1 == pnt2; }

        bool is_valid() const {
            return pnt1.is_valid() && pnt2.is_valid() &&
                   line.is_valid();
        }
        void print() const {
            std::cout << "line segment: " << std::endl;
            pnt1.print();
            pnt2.print();
        }
};

class plane_t {
    public:
        const double a_, b_, c_, d_;
        const vector_t norm_vec;

        plane_t(double a = NAN, double b = NAN, double c = NAN, double d = NAN) :
            a_(a), b_(b), c_(c), d_(d) {};

        plane_t(const point_t &pnt1, const point_t &pnt2, const point_t &pnt3) :
            a_((pnt2.y_ - pnt1.y_) * (pnt3.z_ - pnt1.z_) - (pnt2.z_ - pnt1.z_) * (pnt3.y_ - pnt1.y_)),
            b_((pnt2.z_ - pnt1.z_) * (pnt3.x_ - pnt1.x_) - (pnt2.x_ - pnt1.x_) * (pnt3.z_ - pnt1.z_)),
            c_((pnt2.x_ - pnt1.x_) * (pnt3.y_ - pnt1.y_) - (pnt2.y_ - pnt1.y_) * (pnt3.x_ - pnt1.x_)),
            d_(-pnt1.x_ * a_ - pnt1.y_ * b_ - pnt1.z_ * c_),

            norm_vec(vector_t{a_, b_, c_} / std::sqrt(std::pow(a_, 2) + std::pow(b_, 2) + std::pow(c_, 2)))
        {}

        location_t relatival_location(const plane_t &pln, const point_t &pnt)
        {
            assert(is_valid() && pln.is_valid() && pnt.is_valid());

            vector_t norm_vecs_mul = norm_vec * pln.norm_vec;

            if (equal(norm_vecs_mul.len(), 0)) {
                if (equal(a_ * pnt.x_ + b_ * pnt.y_ +
                                         c_ * pnt.z_ + d_, 0)) {
                    return COINCIDE;
                }
                return PARALLEL;
            }
            return INTERSECT;
        }

        line_t get_intersection(const plane_t &pln) const {
            assert(is_valid() && pln.is_valid());

            vector_t dir_vec = norm_vec * pln.norm_vec;

            double x = NAN, y = NAN, z = NAN, delta = NAN;

            if (!equal(b_ * pln.c_ - pln.b_ * c_, 0)) {
                x = 1,
                delta = (b_ * pln.c_ - pln.b_ * c_),
                y = (-pln.c_ * (d_ + a_ * x) + c_ * (pln.d_ + pln.a_ * x)) / delta,
                z = (-b_ * (pln.d_ + pln.a_ * x) + pln.b_ * (d_ + a_ * x)) / delta;
            }
            else if (!equal(a_ * pln.c_ - pln.a_ * c_, 0)) {
                y = 1,
                delta = (a_ * pln.c_ - pln.a_ * c_),
                x = (-pln.c_ * (d_ + b_ * y) + c_ * (pln.d_ + pln.b_ * y)) / delta,
                z = (-a_ * (pln.d_ + pln.b_ * y) + pln.a_ * (d_ + b_ * y)) / delta;
            }
            else if (!equal(a_ * pln.b_ - pln.a_ * b_, 0)) {
                z = 1,
                delta = (a_ * pln.b_ - pln.a_ * b_),
                x = (-pln.b_ * (d_ + c_ * z) + b_ * (pln.d_ + pln.c_ * z)) / delta,
                y = (-a_ * (pln.d_ + pln.c_ * z) + pln.a_ * (d_ + c_ * z)) / delta;
            }

            point_t line_pnt{x, y, z};

            line_t ret_line{line_pnt, dir_vec};

            return ret_line;
        }

        bool is_valid() const {
            return !(std::isnan(a_) && std::isnan(b_) && std::isnan(c_) && std::isnan(d_));
        }

        vector_t get_norm_vec() const { return norm_vec; }

        double get_pnt_pos(const point_t& pnt) const {
            return a_ * pnt.x_ + b_ * pnt.y_ + c_ * pnt.z_ + d_;
        }

        void print() const {
            std::cout << "plane: " << " a = " << a_ << "; b = " << b_
                      << "; c = " << c_ << "; d = " << d_ << std::endl;
        }

};
}
