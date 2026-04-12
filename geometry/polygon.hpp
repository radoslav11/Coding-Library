#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <bits/stdc++.h>
#include <coding_library/geometry/point.hpp>
using namespace std;

class Polygon {
  public:
    vector<Point> points;

    Polygon() {}
    Polygon(const vector<Point>& points) : points(points) {}

    int size() const { return points.size(); }
    const Point& operator[](int i) const { return points[i]; }
    Point& operator[](int i) { return points[i]; }

    coord_t area() const {
        coord_t a = 0;
        for(int i = 0; i < size(); i++) {
            a += points[i] ^ points[(i + 1) % size()];
        }
        return a / 2.0;
    }
};

class PointInConvexPolygon {
  private:
    Point min_point;
    vector<Point> points_by_angle;

    void prepare() {
        points_by_angle = polygon.points;
        vector<Point>::iterator min_point_it =
            min_element(points_by_angle.begin(), points_by_angle.end());
        min_point = *min_point_it;

        points_by_angle.erase(min_point_it);
        sort(
            points_by_angle.begin(), points_by_angle.end(),
            [&](const Point& a, const Point& b) {
                int d = ccw(min_point, a, b);
                if(d != 0) {
                    return d > 0;
                }
                return (a - min_point).norm2() < (b - min_point).norm2();
            }
        );
    }

  public:
    Polygon polygon;
    PointInConvexPolygon(const Polygon& polygon) : polygon(polygon) {
        prepare();
    }

    bool contains(const Point& p) const {
        int l = 0, r = (int)points_by_angle.size() - 1;
        while(r - l > 1) {
            int m = (l + r) / 2;
            if(ccw(min_point, points_by_angle[m], p) >= 0) {
                l = m;
            } else {
                r = m;
            }
        }

        return point_in_triangle(
            min_point, points_by_angle[l], points_by_angle[r], p
        );
    }
};

class ConvexHull : public Polygon {
  public:
    int lower_end;
    vector<Point> lower, upper;

    ConvexHull(const vector<Point>& points) {
        this->points = points;
        sort(this->points.begin(), this->points.end());
        this->points.erase(
            unique(this->points.begin(), this->points.end()), this->points.end()
        );

        if(this->points.size() <= 2) {
            lower_end = (int)this->points.size() - 1;
            lower = this->points;
            upper = {this->points.back()};
            if(this->points.size() > 1) {
                upper.push_back(this->points.front());
            }
            return;
        }

        vector<int> hull = {0};
        vector<bool> used(this->points.size());

        function<void(int, int)> expand_hull = [&](int i, int min_hull_size) {
            while((int)hull.size() >= min_hull_size &&
                  ccw(this->points[hull[hull.size() - 2]],
                      this->points[hull.back()], this->points[i]) >= 0) {
                used[hull.back()] = false;
                hull.pop_back();
            }
            hull.push_back(i);
            used[i] = true;
        };

        for(int i = 1; i < (int)this->points.size(); i++) {
            expand_hull(i, 2);
        }

        int uhs = hull.size();
        for(int i = (int)this->points.size() - 2; i >= 0; i--) {
            if(!used[i]) {
                expand_hull(i, uhs + 1);
            }
        }

        hull.pop_back();

        vector<Point> pts;
        for(int i: hull) {
            pts.push_back(this->points[i]);
        }
        reverse(pts.begin(), pts.end());
        this->points = std::move(pts);

        lower_end = size() - uhs;
        lower.assign(
            this->points.begin(), this->points.begin() + lower_end + 1
        );
        upper.assign(this->points.begin() + lower_end, this->points.end());
        upper.push_back(this->points[0]);
    }

    pair<int, int> tangents_from(const Point& p) const {
        int n = size();
        if(n <= 1) {
            return {0, 0};
        }

        int a = 0, b = 0;
        auto update = [&](int id) {
            id %= n;
            if(ccw(p, points[a], points[id]) > 0) {
                a = id;
            }
            if(ccw(p, points[b], points[id]) < 0) {
                b = id;
            }
        };

        auto bin_search = [&](int low, int high) {
            if(low >= high) {
                return;
            }
            update(low);
            int sl = ccw(p, points[low % n], points[(low + 1) % n]);
            while(low + 1 < high) {
                int mid = (low + high) / 2;
                if(ccw(p, points[mid % n], points[(mid + 1) % n]) == sl) {
                    low = mid;
                } else {
                    high = mid;
                }
            }
            update(high);
        };

        int lid =
            (int)(lower_bound(lower.begin(), lower.end(), p) - lower.begin());
        bin_search(0, lid);
        bin_search(lid, (int)lower.size() - 1);

        int uid =
            (int)(lower_bound(upper.begin(), upper.end(), p, greater<Point>()) -
                  upper.begin());
        int base = lower_end;
        bin_search(base, base + uid);
        bin_search(base + uid, base + (int)upper.size() - 1);
        return {a, b};
    }
};

#endif  // POLYGON_HPP
