#include <bits/stdc++.h>
#include <coding_library/geometry/point.hpp>
using namespace std;

// Tested on problem 277 from SGU:
// https://codeforces.com/problemsets/acmsguru/problem/99999/277

class DynamicConvexHull {
  public:
    set<Point> lower;
    set<Point> upper;

  private:
    coord_t lower_sum = 0;
    coord_t upper_sum = 0;

    using Iter = set<Point>::iterator;

    void add_to_hull(set<Point>& hull, coord_t& sum, const Point& p, int sign) {
        if(hull.empty()) {
            hull.insert(p);
            return;
        }

        auto right = hull.lower_bound(p);

        if(right != hull.end() && right->x == p.x) {
            if(sign * (p.y - right->y) >= 0) {
                return;
            }
            Iter left = (right != hull.begin()) ? prev(right) : hull.end();
            Iter right_next = next(right);
            if(left != hull.end()) {
                sum -= (*left) ^ (*right);
            }
            if(right_next != hull.end()) {
                sum -= (*right) ^ (*right_next);
            }
            if(left != hull.end() && right_next != hull.end()) {
                sum += (*left) ^ (*right_next);
            }
            hull.erase(right);
            right = right_next;
        }

        Iter left = (right != hull.begin()) ? prev(right) : hull.end();

        if(left != hull.end() && right != hull.end()) {
            if(sign * ccw(*left, *right, p) >= 0) {
                return;
            }
            sum -= (*left) ^ (*right);
        }

        while(left != hull.end() && left != hull.begin()) {
            Iter left_left = prev(left);
            if(sign * ccw(*left_left, *left, p) > 0) {
                break;
            }
            sum -= (*left_left) ^ (*left);
            hull.erase(left);
            left = left_left;
        }

        while(right != hull.end()) {
            Iter right_next = next(right);
            if(right_next == hull.end()) {
                break;
            }
            if(sign * ccw(p, *right, *right_next) > 0) {
                break;
            }
            sum -= (*right) ^ (*right_next);
            hull.erase(right);
            right = right_next;
        }

        if(left != hull.end()) {
            sum += (*left) ^ p;
        }
        if(right != hull.end()) {
            sum += p ^ (*right);
        }

        hull.insert(p);
    }

  public:
    DynamicConvexHull() = default;

    void add(const Point& p) {
        add_to_hull(lower, lower_sum, p, 1);
        add_to_hull(upper, upper_sum, p, -1);
    }

    coord_t doubled_area() const {
        if(lower.empty() || upper.empty()) {
            return 0;
        }

        coord_t result = lower_sum - upper_sum;

        const Point& right_lower = *lower.rbegin();
        const Point& right_upper = *upper.rbegin();
        if(!(right_lower == right_upper)) {
            result += right_lower ^ right_upper;
        }

        const Point& left_lower = *lower.begin();
        const Point& left_upper = *upper.begin();
        if(!(left_lower == left_upper)) {
            result += left_upper ^ left_lower;
        }

        return result;
    }
};
