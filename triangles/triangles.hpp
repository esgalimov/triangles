#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>


const double EPS = 0.00001;

class point_t {
    double x = NAN,
           y = NAN,
           z = NAN;

    public:
        point_t(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {};

        void print() const {
            std::cout << "point(" << x << ";" << y << ";" << z << ")" << std::endl;
        }

        bool is_valid() const {
            return !(std::isnan(x) && std::isnan(y) && std::isnan(z));
        }

        bool equal(const point_t &pnt) const {
            return (std::abs(x - pnt.x) < EPS &&
                    std::abs(y - pnt.y) < EPS &&
                    std::abs(z - pnt.z) < EPS);
        }

        double get_x() const { return x; }
        double get_y() const { return y; }
        double get_z() const { return z; }
};

class vector_t {
    point_t vec_coords;

    public:
        vector_t(const point_t &pnt) :
            vec_coords(pnt.get_x(), pnt.get_y(), pnt.get_z()) {};

        bool is_valid() const { return vec_coords.is_valid(); }

        vector_t multiply(const vector_t &vec) {
            assert(is_valid() && vec.is_valid());

            double x = vec_coords.get_y() * vec.vec_coords.get_z() -
                       vec_coords.get_z() * vec.vec_coords.get_y(),

                   y = vec_coords.get_z() * vec.vec_coords.get_x() -
                       vec_coords.get_x() * vec.vec_coords.get_z(),

                   z = vec_coords.get_x() * vec.vec_coords.get_y() -
                       vec_coords.get_y() * vec.vec_coords.get_x();

            return vector_t{point_t{x, y, z}};
        }

        void print() const {
            std::cout << "vector:";
            vec_coords.print();
        }
};


class line_t {
    point_t  line_pnt;
    vector_t dir_vec;

    public:
        line_t(const point_t &pnt, const vector_t &vec) :
            line_pnt(pnt.get_x(), pnt.get_y(), pnt.get_z()),
            dir_vec(vec) {};

        void print() const {
            std::cout << "line:" << std::endl;
            line_pnt.print();
            dir_vec.print();
        };
};

class triangle_t {
    std::vector<point_t> tr_pnts;

    public:
        triangle_t(const point_t &pnt1, const point_t &pnt2, const point_t &pnt3) {
            tr_pnts.emplace_back(pnt1.get_x(), pnt1.get_y(), pnt1.get_z());
            tr_pnts.emplace_back(pnt2.get_x(), pnt2.get_y(), pnt2.get_z());
            tr_pnts.emplace_back(pnt3.get_x(), pnt3.get_y(), pnt3.get_z());
        }

        void print() const {
            assert(tr_pnts.size() != 3);
            std::cout << "triangle:" << std::endl;

            for (size_t i = 0; i < 3; i++)
                tr_pnts.at(i).print();
        }
};


class plane_t {
    double a = NAN, b = NAN, c = NAN, d = NAN;

    public:
        plane_t(const point_t &pnt1, const point_t &pnt2, const point_t &pnt3) {
            assert(pnt1.is_valid() && pnt2.is_valid() && pnt3.is_valid());

            a = (pnt2.get_y() - pnt1.get_y()) * (pnt3.get_z() - pnt1.get_z()) -
                (pnt2.get_z() - pnt1.get_z()) * (pnt3.get_y() - pnt1.get_y());

            b = (pnt2.get_z() - pnt1.get_z()) * (pnt3.get_x() - pnt1.get_x()) -
                (pnt2.get_x() - pnt1.get_x()) * (pnt3.get_z() - pnt1.get_z());

            c = (pnt2.get_x() - pnt1.get_x()) * (pnt3.get_y() - pnt1.get_y()) -
                (pnt2.get_y() - pnt1.get_y()) * (pnt3.get_x() - pnt1.get_x());

            d = -pnt1.get_x() * a - pnt1.get_y() * b - pnt1.get_z() * c;
        }

        line_t planes_intersection(const plane_t &pln)
        {
            assert(is_valid());

            vector_t dir_vec = get_normal_vector().multiply(pln.get_normal_vector());

            double x = 1,
                   delta = (b * pln.c - pln.b * c),
                   y = (-pln.c * (d + a * x) + c * (pln.d + pln.a * x)) / delta,
                   z = (-b * (pln.d + pln.a * x) + pln.b * (d + a * x)) / delta;

            point_t line_pnt{x, y, z};

            line_t ret_line{line_pnt, dir_vec};

            return ret_line;
        }

        bool is_valid() {
            return !(std::isnan(a) && std::isnan(b) && std::isnan(c) && std::isnan(d));
        }

        vector_t get_normal_vector() const { return vector_t{point_t{a, b, c}}; }

        double get_a() { return a; }
        double get_b() { return b; }
        double get_c() { return c; }
        double get_d() { return d; }

        void print() const {
            std::cout << "plane: " << " a = " << a << "; b = " << b
                      << "; c = " << c << "; d = " << d << std::endl;
        }

};

