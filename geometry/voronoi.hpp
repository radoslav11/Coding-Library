#ifndef VORONOI_HPP
#define VORONOI_HPP

#include <bits/stdc++.h>
#include <coding_library/geometry/point.hpp>
using namespace std;

// Variation of code by Monogon: https://codeforces.com/blog/entry/85638.
// We rotate the points with a small angle as the implementation doesn't
// directly handle the case of two equal X coordinates.

class VoronoiDiagram {
  private:
    static constexpr coord_t INF = 1e100;
    static inline coord_t sweep_x;

    struct Arc {
        mutable Point p, q;
        mutable int id = 0, i;

        Arc(const Point& p, const Point& q, int i) : p(p), q(q), i(i) {}

        coord_t get_y(coord_t x) const {
            if(q.y == INF) {
                return INF;
            }
            x += Point::eps;
            Point mid = (p + q) / 2.0;
            Point dir = (p - mid).perp();
            coord_t D = (x - p.x) * (x - q.x);
            if(D < 0) {
                return -INF;
            }

            if(abs(dir.y) < Point::eps) {
                return (x < mid.x) ? -INF : INF;
            }

            return mid.y +
                   ((mid.x - x) * dir.x + sqrtl(D) * dir.norm()) / dir.y;
        }

        bool operator<(const coord_t& y) const { return get_y(sweep_x) < y; }

        bool operator<(const Arc& o) const {
            return get_y(sweep_x) < o.get_y(sweep_x);
        }
    };

    using Beach = multiset<Arc, less<>>;

    struct Event {
        coord_t x;
        int id;
        Beach::iterator it;

        Event(coord_t x, int id, Beach::iterator it) : x(x), id(id), it(it) {}

        bool operator<(const Event& e) const { return x > e.x; }
    };

    Beach beach_line;
    vector<pair<Point, int>> vertices;
    priority_queue<Event> event_queue;
    vector<pair<int, int>> edges;
    vector<bool> valid;
    int n, next_vertex_id;

    void update_vertex_event(Beach::iterator it) {
        if(it->i == -1) {
            return;
        }

        valid[-it->id] = false;
        auto prev_it = prev(it);

        if(collinear(it->q - it->p, prev_it->p - it->p)) {
            return;
        }

        it->id = --next_vertex_id;
        valid.push_back(true);

        Point center = circumcenter(it->p, it->q, prev_it->p);
        coord_t event_x = center.x + (center - it->p).norm();

        bool valid_event =
            event_x > sweep_x - Point::eps &&
            prev_it->get_y(event_x) + Point::eps > it->get_y(event_x);
        if(valid_event) {
            event_queue.push(Event(event_x, it->id, it));
        }
    }

    void add_edge(int i, int j) {
        if(i == -1 || j == -1) {
            return;
        }
        edges.push_back({vertices[i].second, vertices[j].second});
    }

    void add_point(int i) {
        Point p = vertices[i].first;

        auto split_it = beach_line.lower_bound(p.y);
        auto new_it = beach_line.insert(split_it, Arc(p, split_it->p, i));
        auto prev_it =
            beach_line.insert(new_it, Arc(split_it->p, p, split_it->i));

        add_edge(i, split_it->i);

        update_vertex_event(prev_it);
        update_vertex_event(new_it);
        update_vertex_event(split_it);
    }

    void remove_arc(Beach::iterator it) {
        auto prev_it = prev(it);
        auto next_it = next(it);

        beach_line.erase(it);
        prev_it->q = next_it->p;

        add_edge(prev_it->i, next_it->i);

        update_vertex_event(prev_it);
        update_vertex_event(next_it);
    }

  public:
    VoronoiDiagram(const vector<Point>& points, bool fix_coordinates = true) {
        n = points.size();
        vertices.resize(n);

        for(int i = 0; i < n; i++) {
            vertices[i] = {points[i], i};
        }

        if(fix_coordinates && n > 0) {
            // Rotate around center by 1.0 radians.
            for(int i = 0; i < n; i++) {
                vertices[i].first = vertices[i].first.rotate(1.0);
            }
        }

        sort(vertices.begin(), vertices.end());
    }

    vector<pair<int, int>> compute(coord_t X = 1e9) {
        edges.clear();
        beach_line.clear();
        event_queue = priority_queue<Event>();

        X *= 3;
        beach_line.insert(Arc(Point(-X, -X), Point(-X, X), -1));
        beach_line.insert(Arc(Point(-X, X), Point(INF, INF), -1));

        for(int i = 0; i < n; i++) {
            event_queue.push(Event(vertices[i].first.x, i, beach_line.end()));
        }

        next_vertex_id = 0;
        valid.assign(1, false);

        while(!event_queue.empty()) {
            Event e = event_queue.top();
            event_queue.pop();
            sweep_x = e.x;

            if(e.id >= 0) {
                add_point(e.id);
            } else if(valid[-e.id]) {
                remove_arc(e.it);
            }
        }

        return edges;
    }

    const vector<pair<int, int>>& get_edges() const { return edges; }
};

#endif  // VORONOI_HPP
