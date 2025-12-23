#include <bits/stdc++.h>
#include <coding_library/geometry/point.hpp>

using namespace std;

using Line = pair<Point, Point>;

class HalfPlaneIntersection {
  private:
    vector<Line> lines;
    deque<int> dq;
    bool empty_intersection = false;

    Point dir(int i) const { return lines[i].second - lines[i].first; }

    bool outside(int i, const Point& pt) const {
        return ccw(lines[i].first, lines[i].second, pt) < 0;
    }

    Point inter(int i, int j) const {
        return line_line_intersection(
            lines[i].first, lines[i].second, lines[j].first, lines[j].second
        );
    }

    bool is_parallel(int i, int j) const {
        return abs(dir(i) ^ dir(j)) < Point::eps;
    }

    bool same_direction(int i, int j) const { return (dir(i) * dir(j)) > 0; }

  public:
    static vector<Line> sort_by_angle(const vector<Line>& lines) {
        vector<Line> sorted = lines;
        sort(sorted.begin(), sorted.end(), [](const Line& a, const Line& b) {
            return (a.second - a.first).angle() < (b.second - b.first).angle();
        });
        return sorted;
    }

    HalfPlaneIntersection(const vector<Line>& lines, bool is_sorted = false)
        : lines(is_sorted ? lines : sort_by_angle(lines)) {
        int n = this->lines.size();

        for(int i = 0; i < n; i++) {
            while(dq.size() > 1 &&
                  outside(i, inter(dq.back(), dq[dq.size() - 2]))) {
                dq.pop_back();
            }
            while(dq.size() > 1 && outside(i, inter(dq.front(), dq[1]))) {
                dq.pop_front();
            }

            if(!dq.empty() && is_parallel(i, dq.back())) {
                if(!same_direction(i, dq.back())) {
                    empty_intersection = true;
                    return;
                }
                if(outside(i, this->lines[dq.back()].first)) {
                    dq.pop_back();
                } else {
                    continue;
                }
            }

            dq.push_back(i);
        }

        while(dq.size() > 2 &&
              outside(dq.front(), inter(dq.back(), dq[dq.size() - 2]))) {
            dq.pop_back();
        }
        while(dq.size() > 2 && outside(dq.back(), inter(dq.front(), dq[1]))) {
            dq.pop_front();
        }

        if(dq.size() < 3) {
            empty_intersection = true;
        }
    }

    bool is_non_empty() const { return !empty_intersection; }

    vector<Point> get_polygon() const {
        if(empty_intersection) {
            return {};
        }
        vector<Point> result(dq.size());
        for(size_t i = 0; i + 1 < dq.size(); i++) {
            result[i] = inter(dq[i], dq[i + 1]);
        }
        result.back() = inter(dq.back(), dq.front());
        return result;
    }
};
