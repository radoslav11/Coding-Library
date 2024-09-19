#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <bits/stdc++.h>
using namespace std;

using coord_t = double;

struct Point {
    coord_t x, y;
    Point(coord_t x = 0, coord_t y = 0) : x(x), y(y) {}

    Point operator+(const Point &p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point &p) const { return Point(x - p.x, y - p.y); }
    Point operator*(coord_t c) const { return Point(x * c, y * c); }
    Point operator/(coord_t c) const { return Point(x / c, y / c); }

    coord_t operator*(const Point &p) const { return x * p.x + y * p.y; }
    coord_t operator^(const Point &p) const { return x * p.y - y * p.x; }

    bool operator==(const Point &p) const { return x == p.x && y == p.y; }
    bool operator!=(const Point &p) const { return x != p.x || y != p.y; }
    bool operator<(const Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }
    bool operator>(const Point &p) const {
        return x != p.x ? x > p.x : y > p.y;
    }
    bool operator<=(const Point &p) const {
        return x != p.x ? x < p.x : y <= p.y;
    }
    bool operator>=(const Point &p) const {
        return x != p.x ? x > p.x : y >= p.y;
    }

    coord_t norm2() const { return x * x + y * y; }
    double norm() const { return sqrt(norm2()); }
    double angle() const { return atan2(y, x); }

    Point rotate(double a) const {
        return Point(x * cos(a) - y * sin(a), x * sin(a) + y * cos(a));
    }

    Point perp() const { return Point(-y, x); }
    Point unit() const { return *this / norm(); }
    Point normal() const { return perp().unit(); }
    Point project(const Point &p) const {
        return *this * (*this * p) / norm2();
    }
    Point reflect(const Point &p) const {
        return *this * 2 * (*this * p) / norm2() - p;
    }

    friend ostream &operator<<(ostream &os, const Point &p) {
        return os << p.x << ' ' << p.y;
    }
    friend istream &operator>>(istream &is, Point &p) {
        return is >> p.x >> p.y;
    }

    friend int ccw(const Point &a, const Point &b, const Point &c) {
        coord_t v = (b - a) ^ (c - a);
        if(v > 0) {
            return 1;
        } else if(v < 0) {
            return -1;
        } else {
            return 0;
        }
    }

    friend bool point_in_triangle(
        const Point &a, const Point &b, const Point &c, const Point &p
    ) {
        int d1 = ccw(a, b, p);
        int d2 = ccw(b, c, p);
        int d3 = ccw(c, a, p);
        return (d1 >= 0 && d2 >= 0 && d3 >= 0) ||
               (d1 <= 0 && d2 <= 0 && d3 <= 0);
    }
};

class Polygon {
  public:
    vector<Point> points;

    Polygon() {}
    Polygon(const vector<Point> &points) : points(points) {}

    int size() const { return points.size(); }

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
            [&](const Point &a, const Point &b) {
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
    PointInConvexPolygon(const Polygon &polygon) : polygon(polygon) {
        prepare();
    }

    bool contains(const Point &p) const {
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
#endif  // GEOMETRY_HPP
