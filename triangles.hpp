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


class line_t {
    point_t line_pnt;
    point_t dir_vec;

    public:
        line_t(const point_t &pnt, const point_t &vec) :
            line_pnt(pnt.get_x(), pnt.get_y(), pnt.get_z()),
             dir_vec(vec.get_x(), vec.get_y(), vec.get_z()) {};

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

        line_t planes_intersection(const plane_t &pln) const
        {
            point_t line_pnt{1, 1, 1};

            line_t ret_line{line_pnt, line_pnt};

            return ret_line;
        }

        void print() const {
            std::cout << "plane: " << " a = " << a << "; b = " << b
                      << "; c = " << c << "; d = " << d << std::endl;
        }

};

