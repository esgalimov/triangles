#pragma once

#include <array>
#include "geometry.hpp"
#include "double_funcs.hpp"

#define NDEBUG

namespace triangles {

using namespace geometry;
using namespace double_funcs;

class triangle_t {
    using PntArr = typename std::array<point_t, 3>;
    using SegArr = typename std::array<line_segment_t, 3>;

    PntArr pnts;
    SegArr segs;
    plane_t pln;
    point_t cntr;

    int longest_seg_id;
    const double dist_radius;

    bool is_not_inter_pln(const plane_t &pl) const {
            double pnt1_pos = pl.get_pnt_pos(pnts[0]),
                   pnt2_pos = pl.get_pnt_pos(pnts[1]),
                   pnt3_pos = pl.get_pnt_pos(pnts[2]);

            return (pnt1_pos > 0 && pnt2_pos > 0 && pnt3_pos > 0) ||
                   (pnt1_pos < 0 && pnt2_pos < 0 && pnt3_pos < 0);
        }

    bool is_intersected_on_same_pln(const triangle_t &tr) const {
        for (int i = 0; i < 3; i++) {
            for (int j = i; j < 3; j++) {
                if (segs[i].is_ln_seg_intersected(tr.segs[j]))
                    return true;
            }
        }

        return is_pnt_inside_tr(tr.pnts[0]) || tr.is_pnt_inside_tr(pnts[0]);
    }

    bool is_pnt_inside_tr(const point_t &pnt) const {
        vector_t pnt_vecs[3] = {{pnt, pnts[0]}, {pnt, pnts[1]}, {pnt, pnts[2]}};

        bool is_in_11 = true, is_in_12 = true;

        vector_t norm_vec = pln.norm_vec;

        for (int i = 0; i < 3; i++) {
                bool is_dir = (pnt_vecs[i] * segs[i].line.dir_vec).is_co_dir(norm_vec);
                is_in_11 = is_in_11 &&  is_dir;
                is_in_12 = is_in_12 && !is_dir;
        }
        return is_in_11 || is_in_12;
    }

    bool is_intersected_on_inter_pln(const triangle_t &tr) const {
        line_t inter_line = pln.get_intersection(tr.pln);

        line_segment_t inter_tr1 = get_line_intersection(inter_line),
                       inter_tr2 = tr.get_line_intersection(inter_line);

        if (inter_tr1.is_zero_len() && inter_tr2.is_zero_len()) return false;

        if (inter_tr1.is_zero_len())
            return inter_tr2.is_line_pnt_on_ln_seg(inter_tr1.pnt1);

        if (inter_tr2.is_zero_len())
            return inter_tr1.is_line_pnt_on_ln_seg(inter_tr2.pnt1);

        return inter_tr1.is_line_pnt_on_ln_seg(inter_tr2.pnt1) ||
               inter_tr1.is_line_pnt_on_ln_seg(inter_tr2.pnt2) ||
               inter_tr2.is_line_pnt_on_ln_seg(inter_tr1.pnt1) ||
               inter_tr2.is_line_pnt_on_ln_seg(inter_tr1.pnt2);
    }

    line_segment_t get_line_intersection(const line_t& ln) const {
        line_segment_t inter_12 = segs[0].get_line_intersection(ln),
                       inter_23 = segs[1].get_line_intersection(ln),
                       inter_31 = segs[2].get_line_intersection(ln);

        if (inter_12.is_valid() && !inter_12.is_zero_len())
            return inter_12;
        if (inter_23.is_valid() && !inter_23.is_zero_len())
            return inter_23;
        if (inter_31.is_valid() && !inter_31.is_zero_len())
            return inter_31;

        if (inter_12.is_valid() && inter_23.is_valid())
            return {inter_12.pnt1, inter_23.pnt1};
        if (inter_23.is_valid() && inter_31.is_valid())
            return {inter_23.pnt1, inter_31.pnt1};
        if (inter_12.is_valid() && inter_31.is_valid())
            return {inter_12.pnt1, inter_31.pnt1};

        return {};
    }

    bool trs_intersect(const triangle_t &tr) const {
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

    bool segs_intersect(const triangle_t &tr) const {
        return segs[longest_seg_id].is_ln_seg_intersected(tr.segs[tr.longest_seg_id]);
    }

    int find_longest_seg() const {
        int longest_i = 0;
        if (segs[1].len() > segs[longest_i].len()) longest_i = 1;
        if (segs[2].len() > segs[longest_i].len()) longest_i = 2;

        return longest_i;
    }

    bool tr_pnt_intersect(const triangle_t &tr) const {
        double pnt_pos = pln.get_pnt_pos(tr.pnts[0]);

        if (!equal(pnt_pos, 0)) return false;

        return is_pnt_inside_tr(tr.pnts[0]);
    }

    bool tr_seg_intersect(const triangle_t &tr) const {
        line_segment_t seg = tr.segs[tr.longest_seg_id];

        double pnt1_pos = pln.get_pnt_pos(seg.pnt1),
               pnt2_pos = pln.get_pnt_pos(seg.pnt2);

        if ((pnt1_pos > 0 && pnt2_pos > 0) || (pnt1_pos < 0 && pnt2_pos < 0)) return false;

        if (equal(pnt1_pos, 0) && equal(pnt2_pos, 0))
            return segs[0].is_ln_seg_intersected(seg) ||
                   segs[1].is_ln_seg_intersected(seg) ||
                   segs[2].is_ln_seg_intersected(seg) || is_pnt_inside_tr(seg.pnt1);

        point_t inter_pnt = pln.get_intersection(seg.line);

        return is_pnt_inside_tr(inter_pnt);
    }

    bool seg_pnt_intersect(const triangle_t &tr) const {
        line_segment_t seg = segs[longest_seg_id];
        point_t pnt = tr.pnts[0];

        if (seg.line.is_pnt_on_line(pnt))
            return seg.is_line_pnt_on_ln_seg(pnt);
        return false;
    }

    public:
        triangle_t(const point_t &pnt1, const point_t &pnt2, const point_t &pnt3) :
            pnts({pnt1, pnt2, pnt3}),
            segs({{{pnt1, pnt2}, {pnt2, pnt3}, {pnt3, pnt1}}}),
            pln(pnt1, pnt2, pnt3),
            cntr((pnt1.x_ + pnt2.x_ + pnt3.x_) / 3,
                 (pnt1.y_ + pnt2.y_ + pnt3.y_) / 3,
                 (pnt1.z_ + pnt2.z_ + pnt3.z_) / 3),
            longest_seg_id(find_longest_seg()),
            dist_radius(segs[longest_seg_id].len()) {}

        bool is_intersected(const triangle_t &tr) const {
            double dist_max_intersect = (dist_radius + tr.dist_radius) / 2;

            if (vector_t{cntr, tr.cntr}.len() > dist_max_intersect) return false;

            if (is_triangle() && tr.is_triangle()) return trs_intersect(tr);

            if (is_pnt() && tr.is_pnt()) return pnts[0] == tr.pnts[0];

            if (is_line_seg() && tr.is_line_seg()) return segs_intersect(tr);

            if (is_triangle() && tr.is_pnt()) return tr_pnt_intersect(tr);

            if (is_pnt() && tr.is_triangle()) return tr.tr_pnt_intersect(*this);

            if (is_triangle() && tr.is_line_seg()) return tr_seg_intersect(tr);

            if (is_line_seg() && tr.is_triangle()) return tr.tr_seg_intersect(*this);

            if (is_line_seg() && tr.is_pnt()) return seg_pnt_intersect(tr);

            if (is_pnt() && tr.is_line_seg()) return tr.seg_pnt_intersect(*this);

            return false;
        }

        bool is_triangle() const {
            return !segs[0].line.dir_vec.is_collinear(segs[1].line.dir_vec);
        }

        bool is_line_seg() const {
            return !is_pnt() && (segs[0].line.dir_vec * segs[1].line.dir_vec).is_zero_len();
        }

        bool is_pnt() const {
            return pnts[0] == pnts[1] && pnts[1] == pnts[2];
        }

        bool is_in_cube(const point_t &cntr, double radius) const {
            return pnts[0].is_in_cube(cntr, radius) &&
                   pnts[1].is_in_cube(cntr, radius) &&
                   pnts[2].is_in_cube(cntr, radius);
        }

        bool is_part_in_cube(const point_t &cntr, double radius) const {
            return pnts[0].is_in_cube(cntr, radius) ||
                   pnts[1].is_in_cube(cntr, radius) ||
                   pnts[2].is_in_cube(cntr, radius);
        }

        void print() const {
            std::cout << "triangle:" << std::endl;

            for (size_t i = 0; i < 3; i++)
                pnts.at(i).print();
        }
    };
}
