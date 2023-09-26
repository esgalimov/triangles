#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#define NDEBUG

namespace double_funcs {
    const double EPS = 0.00001;

    bool equal(double a, double b) {
        return std::abs(a - b) < double_funcs::EPS;
    }
}

namespace geometry {

enum location_t {COINCIDE, PARALLEL, INTERSECT, INTERBR};

class point_t {
    double x_, y_, z_;

    public:
        point_t(double x = NAN, double y = NAN, double z = NAN) : x_(x), y_(y), z_(z) {};

        point_t(const point_t &pnt) : x_(pnt.get_x()), y_(pnt.get_y()), z_(pnt.get_z()) {};

        void print() const {
            std::cout << "point(" << x_ << ";" << y_ << ";" << z_ << ")" << std::endl;
        }

        bool is_valid() const {
            return !(std::isnan(x_) && std::isnan(y_) && std::isnan(z_));
        }

        bool operator==(const point_t &pnt) const {
            return (double_funcs::equal(x_, pnt.x_) &&
                    double_funcs::equal(y_, pnt.y_) &&
                    double_funcs::equal(z_, pnt.z_));
        }

        bool operator!=(const point_t &pnt) const {
            return !(*this == pnt);
        }

        double get_x() const { return x_; }
        double get_y() const { return y_; }
        double get_z() const { return z_; }
};

class vector_t {
    double x_, y_, z_;

    public:
        vector_t(double x = NAN, double y = NAN, double z = NAN) : x_(x), y_(y), z_(z) {};

        vector_t(const point_t &pnt1, const point_t &pnt2) :
            x_(pnt1.get_x() - pnt2.get_x()),
            y_(pnt1.get_y() - pnt2.get_y()),
            z_(pnt1.get_z() - pnt2.get_z()) {}

        bool is_valid() const {
            return !(std::isnan(x_) && std::isnan(y_) && std::isnan(z_));
        }

        vector_t multiply(const vector_t &vec) const {
            assert(is_valid() && vec.is_valid());

            double x = get_y() * vec.get_z() - get_z() * vec.get_y(),
                   y = get_z() * vec.get_x() - get_x() * vec.get_z(),
                   z = get_x() * vec.get_y() - get_y() * vec.get_x();

            return vector_t{x, y, z};
        }

        double scalar_multiply(const vector_t &vec) const {
            assert(is_valid() && vec.is_valid());

            return get_x() * vec.get_x() + get_y() * vec.get_y() + get_z() * vec.get_z();
        }

        double len() const {
            assert(is_valid());

            return std::sqrt(std::pow(get_x(), 2) + std::pow(get_y(), 2) + std::pow(get_z(), 2));
        }

        bool is_collinear(const vector_t &vec) const {
            return double_funcs::equal(multiply(vec).len(), 0);
        }

        double get_x() const { return x_; }
        double get_y() const { return y_; }
        double get_z() const { return z_; }

        void print() const {
            std::cout << "vector(" << x_ << ";" << y_ << ";" << z_ << ")" << std::endl;
        }
};


class line_t {
    point_t  line_pnt;
    vector_t dir_vec;

    public:
        line_t(const point_t &pnt, const vector_t &vec) :
            line_pnt(pnt), dir_vec(vec) {};

        void print() const {
            std::cout << "line:" << std::endl;
            line_pnt.print();
            dir_vec.print();
        };

        location_t relatival_location(const line_t &ln) {
            assert(is_valid() && ln.is_valid());

            point_t ln_pnt = ln.get_line_pnt();

            vector_t cross_ln_vec{line_pnt, ln.get_line_pnt()};

            if (double_funcs::equal(dir_vec.multiply(ln.dir_vec).scalar_multiply(cross_ln_vec), 0)) {
                if (dir_vec.is_collinear(ln.dir_vec)) {
                    if (dir_vec.is_collinear(cross_ln_vec))
                        return COINCIDE;
                    else
                        return PARALLEL;
                }
                else
                    return INTERSECT;
            }
            else
                return INTERBR;
        }

        bool is_valid() const {
            return line_pnt.is_valid() && dir_vec.is_valid();
        }

        point_t get_line_pnt() const { return line_pnt; }

        vector_t get_dir_vec() const { return dir_vec; }

        double get_vec_x() const { return dir_vec.get_x(); }
        double get_vec_y() const { return dir_vec.get_y(); }
        double get_vec_z() const { return dir_vec.get_z(); }

        double get_pnt_x() const { return line_pnt.get_x(); }
        double get_pnt_y() const { return line_pnt.get_y(); }
        double get_pnt_z() const { return line_pnt.get_z(); }
};

class line_segment_t {
    point_t pnt1;
    point_t pnt2;
    line_t  line;

    public:
        line_segment_t(const point_t &p1, const point_t &p2) :
            pnt1(p1), pnt2(p2), line{p1, vector_t{p1, p2}} {}

        line_segment_t get_intersection(const line_t &ln) {
            assert(is_valid() && ln.is_valid());

            location_t rel_loc = line.relatival_location(ln);

            if (rel_loc == COINCIDE) return *this;

            else if (rel_loc == INTERSECT) {
                double det_12 = line.get_vec_x() * ln.get_vec_y() - line.get_vec_y() * ln.get_vec_x();
                double det_23 = line.get_vec_y() * ln.get_vec_z() - line.get_vec_z() * ln.get_vec_y();
                double det_13 = line.get_vec_x() * ln.get_vec_z() - line.get_vec_z() * ln.get_vec_x();

                double r_side_1 = line.get_pnt_x() - ln.get_pnt_x();
                double r_side_2 = line.get_pnt_y() - ln.get_pnt_y();
                double r_side_3 = line.get_pnt_z() - ln.get_pnt_z();

                double t_par = NAN;

                if (!double_funcs::equal(det_12, 0))
                    t_par = (r_side_1 * ln.get_vec_y() - ln.get_vec_x() * r_side_2) / det_12;

                else if (!double_funcs::equal(det_23, 0))
                    t_par = (r_side_2 * ln.get_vec_y() - ln.get_vec_z() * r_side_3) / det_12;
                else

                    t_par = (r_side_1 * ln.get_vec_x() - ln.get_vec_z() * r_side_3) / det_12;

                point_t inter_pnt{line.get_vec_x() * t_par - line.get_pnt_x(),
                                  line.get_vec_y() * t_par - line.get_pnt_y(),
                                  line.get_vec_z() * t_par - line.get_pnt_z()};

                return line_segment_t{inter_pnt, inter_pnt};
            }
            else return line_segment_t{point_t{}, point_t{}};
        }

        bool is_valid() const {
            return pnt1.is_valid() && pnt2.is_valid() &&
                   pnt1 != pnt2 && line.is_valid();
        }
        void print() const {
            pnt1.print();
            pnt2.print();
            line.print();
        }


};

class triangle_t {
    std::array<point_t, 3> tr_pnts;

    public:
        triangle_t(const point_t &pnt1, const point_t &pnt2, const point_t &pnt3) {
            tr_pnts[0] = point_t{pnt1.get_x(), pnt1.get_y(), pnt1.get_z()};
            tr_pnts[1] = point_t{pnt2.get_x(), pnt2.get_y(), pnt2.get_z()};
            tr_pnts[2] = point_t{pnt3.get_x(), pnt3.get_y(), pnt3.get_z()};
        }

        void print() const {
            std::cout << "triangle:" << std::endl;

            for (size_t i = 0; i < 3; i++)
                tr_pnts.at(i).print();
        }
};


class plane_t {
    double a_, b_, c_, d_;

    public:
        plane_t(double a = NAN, double b = NAN, double c = NAN, double d = NAN) :
            a_(a), b_(b), c_(c), d_(d) {};

        plane_t(const point_t &pnt1, const point_t &pnt2, const point_t &pnt3) {
            assert(pnt1.is_valid() && pnt2.is_valid() && pnt3.is_valid());

            a_ = (pnt2.get_y() - pnt1.get_y()) * (pnt3.get_z() - pnt1.get_z()) -
                (pnt2.get_z() - pnt1.get_z()) * (pnt3.get_y() - pnt1.get_y());

            b_ = (pnt2.get_z() - pnt1.get_z()) * (pnt3.get_x() - pnt1.get_x()) -
                (pnt2.get_x() - pnt1.get_x()) * (pnt3.get_z() - pnt1.get_z());

            c_ = (pnt2.get_x() - pnt1.get_x()) * (pnt3.get_y() - pnt1.get_y()) -
                (pnt2.get_y() - pnt1.get_y()) * (pnt3.get_x() - pnt1.get_x());

            d_ = -pnt1.get_x() * a_ - pnt1.get_y() * b_ - pnt1.get_z() * c_;
        }

        location_t relatival_location(const plane_t &pln, const point_t &pnt)
        {
            assert(is_valid() && pln.is_valid() && pnt.is_valid());

            vector_t norm_vecs_mul = get_normal_vector().multiply(pln.get_normal_vector());

            if (double_funcs::equal(norm_vecs_mul.len(), 0)) {
                if (double_funcs::equal(a_ * pnt.get_x() + b_ * pnt.get_y() +
                                         c_ * pnt.get_z() + d_, 0)) {
                    return COINCIDE;
                }
                return PARALLEL;
            }
            return INTERSECT;
        }

        line_t get_intersection(const plane_t &pln) {
            assert(is_valid() && pln.is_valid());

            vector_t dir_vec = get_normal_vector().multiply(pln.get_normal_vector());

            double x = NAN, y = NAN, z = NAN, delta = NAN;

            if (!double_funcs::equal(b_ * pln.c_ - pln.b_ * c_, 0)) {
                x = 1,
                delta = (b_ * pln.c_ - pln.b_ * c_),
                y = (-pln.c_ * (d_ + a_ * x) + c_ * (pln.d_ + pln.a_ * x)) / delta,
                z = (-b_ * (pln.d_ + pln.a_ * x) + pln.b_ * (d_ + a_ * x)) / delta;
            }
            else if (!double_funcs::equal(a_ * pln.c_ - pln.a_ * c_, 0)) {
                y = 1,
                delta = (a_ * pln.c_ - pln.a_ * c_),
                x = (-pln.c_ * (d_ + b_ * y) + c_ * (pln.d_ + pln.b_ * y)) / delta,
                z = (-a_ * (pln.d_ + pln.b_ * y) + pln.a_ * (d_ + b_ * y)) / delta;
            }
            else if (!double_funcs::equal(a_ * pln.b_ - pln.a_ * b_, 0)) {
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

        vector_t get_normal_vector() const { return vector_t{a_, b_, c_}; }

        double get_a() { return a_; }
        double get_b() { return b_; }
        double get_c() { return c_; }
        double get_d() { return d_; }

        void print() const {
            std::cout << "plane: " << " a = " << a_ << "; b = " << b_
                      << "; c = " << c_ << "; d = " << d_ << std::endl;
        }

};
}
