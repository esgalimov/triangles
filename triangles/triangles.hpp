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

        point_t(const point_t &pnt) : x_(pnt.x_), y_(pnt.y_), z_(pnt.z_) {};

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

            double x = y_ * vec.z_ - z_ * vec.y_,
                   y = z_ * vec.x_ - x_ * vec.z_,
                   z = x_ * vec.y_ - y_ * vec.x_;

            return {x, y, z};
        }

        double scalar_multiply(const vector_t &vec) const {
            assert(is_valid() && vec.is_valid());

            return x_ * vec.x_ + y_ * vec.y_ + z_ * vec.z_;
        }

        vector_t operator+(const vector_t &vec) const { return {x_ + vec.x_, y_ + vec.y_, z_ + vec.z_}; }

        vector_t operator-(const vector_t &vec) const { return {x_ - vec.x_, y_ - vec.y_, z_ - vec.z_}; }

        vector_t operator*(const vector_t &vec) const {
            double x = y_ * vec.z_ - z_ * vec.y_,
                   y = z_ * vec.x_ - x_ * vec.z_,
                   z = x_ * vec.y_ - y_ * vec.x_;

            return {x, y, z};
        }

        vector_t operator/(double num) const { return {x_ / num, y_ / num, z_ / num}; }

        double len() const {
            assert(is_valid());

            return std::sqrt(std::pow(x_, 2) + std::pow(y_, 2) + std::pow(z_, 2));
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

        location_t relatival_location(const line_t &ln) const {
            assert(is_valid() && ln.is_valid());

            point_t ln_pnt = ln.get_line_pnt();

            vector_t cross_ln_vec{line_pnt, ln.get_line_pnt()};

            //std::cout << "mult in rel loc = " << dir_vec.multiply(ln.dir_vec).scalar_multiply(cross_ln_vec) << std::endl;

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
        line_segment_t(const point_t &p1 = point_t{}, const point_t &p2 = point_t{}) :
            pnt1(p1), pnt2(p2), line({p1, {p1, p2}}) {}

        line_segment_t get_line_intersection(const line_t &ln) const {
            assert(is_valid() && ln.is_valid());

            location_t rel_loc = line.relatival_location(ln);

            //std::cout << "rel_loc = " << rel_loc << std::endl;

            if (rel_loc == COINCIDE) return *this;

            else if (rel_loc == INTERSECT) {
                double det_12 = -line.get_vec_x() * ln.get_vec_y() + line.get_vec_y() * ln.get_vec_x();
                double det_23 = -line.get_vec_y() * ln.get_vec_z() + line.get_vec_z() * ln.get_vec_y();
                double det_13 = -line.get_vec_x() * ln.get_vec_z() + line.get_vec_z() * ln.get_vec_x();

                double r_side_1 = ln.get_pnt_x() - line.get_pnt_x();
                double r_side_2 = ln.get_pnt_y() - line.get_pnt_y();
                double r_side_3 = ln.get_pnt_z() - line.get_pnt_z();

                double t_par = NAN;

                if (!double_funcs::equal(det_12, 0)) {
                    t_par = (r_side_1 * (-ln.get_vec_y()) - r_side_2 * (-ln.get_vec_x())) / det_12;
                }
                else if (!double_funcs::equal(det_23, 0)) {
                    t_par = (r_side_2 * (-ln.get_vec_z()) - r_side_3 * (-ln.get_vec_y())) / det_23;
                }
                else {
                    t_par = (r_side_1 * (-ln.get_vec_z()) - r_side_3 * (-ln.get_vec_x())) / det_13;
                }

                point_t inter_pnt{line.get_vec_x() * t_par + line.get_pnt_x(),
                                  line.get_vec_y() * t_par + line.get_pnt_y(),
                                  line.get_vec_z() * t_par + line.get_pnt_z()};

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
            return double_funcs::equal(std::abs(pnt.get_x()  - pnt1.get_x()) +
                                       std::abs(pnt.get_x()  - pnt2.get_x()),
                                       std::abs(pnt1.get_x() - pnt2.get_x()))  &&

                   double_funcs::equal(std::abs(pnt.get_y()  - pnt1.get_y()) +
                                       std::abs(pnt.get_y()  - pnt2.get_y()),
                                       std::abs(pnt1.get_y() - pnt2.get_y()))  &&

                   double_funcs::equal(std::abs(pnt.get_z()  - pnt1.get_z()) +
                                       std::abs(pnt.get_z()  - pnt2.get_z()),
                                       std::abs(pnt1.get_z() - pnt2.get_z()));
        }

        bool is_zero_len() const { return pnt1 == pnt2; }

        point_t get_pnt1() const { return pnt1; }

        point_t get_pnt2() const { return pnt2; }

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
    double a_, b_, c_, d_;
    vector_t norm_vec;

    public:
        plane_t(double a = NAN, double b = NAN, double c = NAN, double d = NAN) :
            a_(a), b_(b), c_(c), d_(d) {};

        plane_t(const point_t &pnt1, const point_t &pnt2, const point_t &pnt3) :
            a_((pnt2.get_y() - pnt1.get_y()) * (pnt3.get_z() - pnt1.get_z()) -
               (pnt2.get_z() - pnt1.get_z()) * (pnt3.get_y() - pnt1.get_y())),

            b_((pnt2.get_z() - pnt1.get_z()) * (pnt3.get_x() - pnt1.get_x()) -
               (pnt2.get_x() - pnt1.get_x()) * (pnt3.get_z() - pnt1.get_z())),

            c_((pnt2.get_x() - pnt1.get_x()) * (pnt3.get_y() - pnt1.get_y()) -
               (pnt2.get_y() - pnt1.get_y()) * (pnt3.get_x() - pnt1.get_x())),

            d_(-pnt1.get_x() * a_ - pnt1.get_y() * b_ - pnt1.get_z() * c_),

            norm_vec({a_, b_, c_})
        {
            norm_vec = norm_vec / std::sqrt(std::pow(a_, 2) + std::pow(b_, 2) + std::pow(c_, 2));
        }

        location_t relatival_location(const plane_t &pln, const point_t &pnt)
        {
            assert(is_valid() && pln.is_valid() && pnt.is_valid());

            vector_t norm_vecs_mul = norm_vec.multiply(pln.norm_vec);

            if (double_funcs::equal(norm_vecs_mul.len(), 0)) {
                if (double_funcs::equal(a_ * pnt.get_x() + b_ * pnt.get_y() +
                                         c_ * pnt.get_z() + d_, 0)) {
                    return COINCIDE;
                }
                return PARALLEL;
            }
            return INTERSECT;
        }

        line_t get_intersection(const plane_t &pln) const {
            assert(is_valid() && pln.is_valid());

            vector_t dir_vec = norm_vec.multiply(pln.norm_vec);

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

        vector_t get_normal_vector() const { return norm_vec; }

        double get_pnt_pos(const point_t& pnt) const {
            return a_ * pnt.get_x() + b_ * pnt.get_y() + c_ * pnt.get_z() + d_;
        }

        double get_a() { return a_; }
        double get_b() { return b_; }
        double get_c() { return c_; }
        double get_d() { return d_; }

        void print() const {
            std::cout << "plane: " << " a = " << a_ << "; b = " << b_
                      << "; c = " << c_ << "; d = " << d_ << std::endl;
        }

};

class triangle_t {
    using PntArr = typename std::array<point_t, 3>;
    using SegArr = typename std::array<line_segment_t, 3>;

    PntArr pnts;
    SegArr segs;
    plane_t pln;

    bool is_intersected_on_same_pln(const triangle_t &tr) const {
        for (int i = 0; i < 3; i++) {
            for (int j = i; j < 3; j++) {
                if (segs[i].is_ln_seg_intersected(tr.segs[j]))
                    return true;
            }
        }
        return false;
    }

    bool is_intersected_on_inter_pln(const triangle_t &tr) const {
        line_t inter_line = pln.get_intersection(tr.pln);

        //inter_line.print();

        line_segment_t inter_tr1 = get_line_intersection(inter_line),
                       inter_tr2 = tr.get_line_intersection(inter_line);

        // inter_tr1.print();
        // inter_tr2.print();

        if (inter_tr1.is_zero_len() && inter_tr2.is_zero_len()) return false;

        if (inter_tr1.is_zero_len())
            return inter_tr2.is_line_pnt_on_ln_seg(inter_tr1.get_pnt1());

        if (inter_tr2.is_zero_len())
            return inter_tr1.is_line_pnt_on_ln_seg(inter_tr2.get_pnt1());

        return inter_tr1.is_line_pnt_on_ln_seg(inter_tr2.get_pnt1()) ||
               inter_tr1.is_line_pnt_on_ln_seg(inter_tr2.get_pnt2()) ||
               inter_tr2.is_line_pnt_on_ln_seg(inter_tr1.get_pnt1()) ||
               inter_tr2.is_line_pnt_on_ln_seg(inter_tr1.get_pnt2());
    }

    line_segment_t get_line_intersection(const line_t& ln) const {
        line_segment_t inter_12 = segs[0].get_line_intersection(ln),
                       inter_23 = segs[1].get_line_intersection(ln),
                       inter_31 = segs[2].get_line_intersection(ln);

        // inter_12.print();
        // inter_23.print();
        // inter_31.print();

        if (inter_12.is_valid() && !inter_12.is_zero_len())
            return inter_12;
        if (inter_23.is_valid() && !inter_23.is_zero_len())
            return inter_23;
        if (inter_31.is_valid() && !inter_31.is_zero_len())
            return inter_31;

        if (inter_12.is_valid() && inter_23.is_valid())
            return {inter_12.get_pnt1(), inter_23.get_pnt1()};
        if (inter_23.is_valid() && inter_31.is_valid())
            return {inter_23.get_pnt1(), inter_31.get_pnt1()};
        if (inter_12.is_valid() && inter_31.is_valid())
            return {inter_12.get_pnt1(), inter_31.get_pnt1()};

        return {};
    }

    public:
        triangle_t(const point_t &pnt1, const point_t &pnt2, const point_t &pnt3) :
            pnts({pnt1, pnt2, pnt3}),
            segs({{{pnt1, pnt2}, {pnt2, pnt3}, {pnt3, pnt1}}}),
            pln(pnt1, pnt2, pnt3) {}

        bool is_intersected(const triangle_t &tr) {
            if (tr.is_not_inter_pln(pln)) return false;

            location_t rel_loc = pln.relatival_location(tr.pln, tr.pnts[0]);

            switch (rel_loc) {
                case COINCIDE:
                    return is_intersected_on_same_pln(tr);
                case PARALLEL:
                    return false;
                case INTERSECT:
                    return is_intersected_on_inter_pln(tr);
                default: return false;
            }
        }

        bool is_not_inter_pln(const plane_t &pl) const {
            double pnt1_pos = pl.get_pnt_pos(pnts[0]),
                   pnt2_pos = pl.get_pnt_pos(pnts[1]),
                   pnt3_pos = pl.get_pnt_pos(pnts[2]);

            return (pnt1_pos > 0 && pnt2_pos > 0 && pnt3_pos > 0) ||
                   (pnt1_pos < 0 && pnt2_pos < 0 && pnt3_pos < 0);
        }

        void print() const {
            std::cout << "triangle:" << std::endl;

            for (size_t i = 0; i < 3; i++)
                pnts.at(i).print();
        }
    };
}
