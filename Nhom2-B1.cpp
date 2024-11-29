#include <bits/stdc++.h>
using namespace std;

using ld = long double;

struct Point {
    ld x, y;
    Point () {}
    Point(ld x, ld y) : x(x), y(y) {}

    bool operator < (const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};

ld cross(const Point &O, const Point &A, const Point &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}
// MonotoneChain
vector<Point> convexHull(vector<Point> &points) {
    int n = points.size(), k = 0;
    if (n <= 3) return points;

    vector<Point> hull(2 * n);

    sort(points.begin(), points.end());

    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(hull[k - 2], hull[k - 1], points[i]) <= 0) k--;
        hull[k++] = points[i];
    }

    for (int i = n - 2, t = k + 1; i >= 0; --i) {
        while (k >= t && cross(hull[k - 2], hull[k - 1], points[i]) <= 0) k--;
        hull[k++] = points[i];
    }

    hull.resize(k - 1);
    return hull;
}

ld perimeter(const vector<Point> &hull) {
    ld result = 0;
    int n = hull.size();
    for (int i = 0; i < n; ++i) {
        Point p1 = hull[i];
        Point p2 = hull[(i + 1) % n];
        result += sqrtl((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    }
    return result;
}

int main() {
    int n;
    cin >> n;
    vector<Point> points(n);
    for (int i = 0; i < n; ++i)
        cin >> points[i].x >> points[i].y;

    vector<Point> hull = convexHull(points);

    ld length = perimeter(hull);

    cout << fixed << setprecision(6) << length << endl;
    for (const auto &p : hull)
        cout << p.x << " " << p.y << endl;

    return 0;
}

