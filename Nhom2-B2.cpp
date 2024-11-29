#include <bits/stdc++.h>
#define sz(x) (int) (x).size()
using namespace std;

using ld = long double;
const ld EPS = 1e-9;

template<class T>
inline T Abs (T x)  {
    return x < 0 ? -x : x;
}

struct Point {
    ld x, y;
    Point () {}
    Point(ld x, ld y) : x(x), y(y) {}
};

struct Polygon {
    vector<Point> vertices;

    Polygon () { vertices.clear(); }

    void addVertex(Point p) {
        vertices.push_back(p);
    }

    ld area() const {
        ld a = 0;
        int n = vertices.size();
        for (int i = 0; i < n; ++i) {
            Point p1 = vertices[i];
            Point p2 = vertices[(i + 1) % n];
            a += (p1.x * p2.y - p1.y * p2.x);
        }
        return Abs(a) / 2.0;
    }
};

int orientation(Point a, Point b, Point c) {
    ld val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    if (fabs(val) < EPS) return 0;
    return (val > 0) ? 1 : -1;
}


bool segmentsIntersect(Point a, Point b, Point c, Point d, Point &intersection) {
    ld A1 = b.y - a.y, B1 = a.x - b.x, C1 = A1 * a.x + B1 * a.y;
    ld A2 = d.y - c.y, B2 = c.x - d.x, C2 = A2 * c.x + B2 * c.y;
    ld det = A1 * B2 - A2 * B1;

    if (fabs(det) < EPS) return false;

    intersection.x = (B2 * C1 - B1 * C2) / det;
    intersection.y = (A1 * C2 - A2 * C1) / det;

    auto onSegment = [](Point p, Point a, Point b) {
        return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
               min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
    };
    return onSegment(intersection, a, b) && onSegment(intersection, c, d);
}

Polygon polygonIntersection(const Polygon &poly1, const Polygon &poly2) {
    Polygon result;
    for (auto &p : poly1.vertices) {
        bool inside = true;
        for (int i = 0; i < sz(poly2.vertices); ++i) {
            Point a = poly2.vertices[i];
            Point b = poly2.vertices[(i + 1) % poly2.vertices.size()];
            if (orientation(a, b, p) == 1) {
                inside = false;
                break;
            }
        }
        if (inside) result.addVertex(p);
    }

    for (auto &p : poly2.vertices) {
        bool inside = true;
        for (int i = 0; i < sz(poly1.vertices); ++i) {
            Point a = poly1.vertices[i];
            Point b = poly1.vertices[(i + 1) % poly1.vertices.size()];
            if (orientation(a, b, p) == -1) {
                inside = false;
                break;
            }
        }
        if (inside) result.addVertex(p);
    }

    for (int i = 0; i < sz(poly1.vertices); ++i) {
        Point a = poly1.vertices[i];
        Point b = poly1.vertices[(i + 1) % poly1.vertices.size()];
        for (int j = 0; j < sz(poly2.vertices); ++j) {
            Point c = poly2.vertices[j];
            Point d = poly2.vertices[(j + 1) % poly2.vertices.size()];
            Point inter;
            if (segmentsIntersect(a, b, c, d, inter)) {
                result.addVertex(inter);
            }
        }
    }

    Point center(0, 0);
    for (auto &p : result.vertices) {
        center.x += p.x;
        center.y += p.y;
    }
    center.x /= result.vertices.size();
    center.y /= result.vertices.size();

    sort(result.vertices.begin(), result.vertices.end(), [&](Point a, Point b) {
        return atan2(a.y - center.y, a.x - center.x) <
               atan2(b.y - center.y, b.x - center.x);
    });

    return result;
}

int main() {
    Polygon poly1, poly2;

    int m, n;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        ld x, y;
        cin >> x >> y;
        poly1.addVertex(Point(x, y));
    }

    cin >> n;
    for (int i = 0; i < n; ++i) {
        ld x, y;
        cin >> x >> y;
        poly2.addVertex(Point(x, y));
    }

    Polygon intersection = polygonIntersection(poly1, poly2);
    cout << fixed << setprecision(6) << intersection.area();

    return 0;
}
