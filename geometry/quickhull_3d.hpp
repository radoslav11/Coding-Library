#include <bits/stdc++.h>
using namespace std;

// Tested on problem 290 from SGU:
// https://codeforces.com/problemsets/acmsguru/problem/99999/290
//
// Implements the Quickhull algorithm from Barber, Dobkin, Huhdanpaa, "The
// Quickhull Algorithm for Convex Hulls", ACM TOMS 22(4), 1996:
// https://doi.org/10.1145/235815.235821

using coord_t = int64_t;

struct Point3D {
    coord_t x, y, z;

    Point3D(coord_t x = 0, coord_t y = 0, coord_t z = 0) : x(x), y(y), z(z) {}

    Point3D operator+(const Point3D& p) const {
        return {x + p.x, y + p.y, z + p.z};
    }

    Point3D operator-(const Point3D& p) const {
        return {x - p.x, y - p.y, z - p.z};
    }

    Point3D cross(const Point3D& p) const {
        return {y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x};
    }

    coord_t dot(const Point3D& p) const { return x * p.x + y * p.y + z * p.z; }
    
    bool operator==(const Point3D& p) const {
        return x == p.x && y == p.y && z == p.z;
    }

    friend coord_t signed_volume(
        const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d
    ) {
        return (b - a).cross(c - a).dot(d - a);
    }
};

class QuickHull3D {
  public:
    struct Face {
        array<int, 3> v;
        array<int, 3> nbr;
        vector<int> outside;
        bool alive;
    };

    vector<Point3D> points;
    vector<Face> faces;
    bool degenerate;

    QuickHull3D(const vector<Point3D>& pts) : points(pts), degenerate(false) {
        build();
    }

    bool on_boundary(const Point3D& p) const {
        if(degenerate) {
            return true;
        }

        for(const auto& f: faces) {
            if(!f.alive) {
                continue;
            }

            if(signed_volume(
                   points[f.v[0]], points[f.v[1]], points[f.v[2]], p
               ) == 0) {
                return true;
            }
        }

        return false;
    }

  private:
    coord_t above(int fi, int pi) const {
        const auto& f = faces[fi];
        return signed_volume(
            points[f.v[0]], points[f.v[1]], points[f.v[2]], points[pi]
        );
    }

    int add_face(int a, int b, int c) {
        faces.push_back({{a, b, c}, {-1, -1, -1}, {}, true});
        return (int)faces.size() - 1;
    }

    int find_edge(int fi, int u, int v) const {
        for(int e = 0; e < 3; e++) {
            if(faces[fi].v[e] == u && faces[fi].v[(e + 1) % 3] == v) {
                return e;
            }
        }

        return -1;
    }

    void build() {
        int n = (int)points.size();
        if(n < 4) {
            degenerate = true;
            return;
        }

        int i0 = 0, i1 = -1, i2 = -1, i3 = -1;
        for(int i = 1; i < n; i++) {
            if(!(points[i] == points[i0])) {
                i1 = i;
                break;
            }
        }

        if(i1 < 0) {
            degenerate = true;
            return;
        }

        for(int i = 1; i < n; i++) {
            if(i == i1) {
                continue;
            }

            Point3D c = (points[i1] - points[i0]).cross(points[i] - points[i0]);
            if(c.dot(c) > 0) {
                i2 = i;
                break;
            }
        }

        if(i2 < 0) {
            degenerate = true;
            return;
        }

        for(int i = 0; i < n; i++) {
            if(i == i0 || i == i1 || i == i2) {
                continue;
            }

            if(signed_volume(points[i0], points[i1], points[i2], points[i]) !=
               0) {
                i3 = i;
                break;
            }
        }

        if(i3 < 0) {
            degenerate = true;
            return;
        }

        if(signed_volume(points[i0], points[i1], points[i2], points[i3]) < 0) {
            swap(i1, i2);
        }

        int f0 = add_face(i0, i2, i1);
        int f1 = add_face(i1, i2, i3);
        int f2 = add_face(i0, i3, i2);
        int f3 = add_face(i0, i1, i3);

        faces[f0].nbr = {f2, f1, f3};
        faces[f1].nbr = {f0, f2, f3};
        faces[f2].nbr = {f3, f1, f0};
        faces[f3].nbr = {f0, f1, f2};

        for(int p = 0; p < n; p++) {
            if(p == i0 || p == i1 || p == i2 || p == i3) {
                continue;
            }

            for(int fi = 0; fi < 4; fi++) {
                if(above(fi, p) > 0) {
                    faces[fi].outside.push_back(p);
                    break;
                }
            }
        }

        queue<int> q;
        for(int fi = 0; fi < 4; fi++) {
            if(!faces[fi].outside.empty()) {
                q.push(fi);
            }
        }

        while(!q.empty()) {
            int fi = q.front();
            q.pop();
            if(!faces[fi].alive || faces[fi].outside.empty()) {
                continue;
            }

            int apex = faces[fi].outside[0];
            coord_t best = above(fi, apex);
            for(int p: faces[fi].outside) {
                coord_t d = above(fi, p);
                if(d > best) {
                    best = d;
                    apex = p;
                }
            }

            vector<int> visible = {fi};
            vector<char> seen(faces.size(), 0);
            seen[fi] = 1;

            vector<array<int, 4>> horizon;
            for(int idx = 0; idx < (int)visible.size(); idx++) {
                int cur = visible[idx];
                for(int e = 0; e < 3; e++) {
                    int nb = faces[cur].nbr[e];
                    if(nb < 0 || seen[nb]) {
                        continue;
                    }
                    if(above(nb, apex) > 0) {
                        seen[nb] = 1;
                        visible.push_back(nb);
                    } else {
                        int u = faces[cur].v[e];
                        int v = faces[cur].v[(e + 1) % 3];
                        int ne = find_edge(nb, v, u);
                        horizon.push_back({u, v, nb, ne});
                    }
                }
            }

            vector<int> orphans;
            for(int vf: visible) {
                for(int p: faces[vf].outside) {
                    if(p != apex) {
                        orphans.push_back(p);
                    }
                }

                faces[vf].outside.clear();
                faces[vf].alive = false;
            }

            vector<int> new_faces;
            map<pair<int, int>, int> apex_edge;
            for(auto& h: horizon) {
                int u = h[0], v = h[1], nb = h[2], ne = h[3];
                int nf = add_face(apex, u, v);
                faces[nf].nbr[1] = nb;
                faces[nb].nbr[ne] = nf;
                apex_edge[{apex, u}] = nf;
                apex_edge[{v, apex}] = nf;
                new_faces.push_back(nf);
            }

            for(int nf: new_faces) {
                int u = faces[nf].v[1];
                int v = faces[nf].v[2];
                auto it1 = apex_edge.find({u, apex});
                if(it1 != apex_edge.end()) {
                    faces[nf].nbr[0] = it1->second;
                }

                auto it2 = apex_edge.find({apex, v});
                if(it2 != apex_edge.end()) {
                    faces[nf].nbr[2] = it2->second;
                }
            }

            for(int p: orphans) {
                for(int nf: new_faces) {
                    if(above(nf, p) > 0) {
                        faces[nf].outside.push_back(p);
                        break;
                    }
                }
            }

            for(int nf: new_faces) {
                if(!faces[nf].outside.empty()) {
                    q.push(nf);
                }
            }
        }
    }
};
