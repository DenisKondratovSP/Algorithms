#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

struct Point {
  long long xx;
  long long yy;

  bool operator<(const Point& other) const {
    if (xx == other.xx) {
      return yy < other.yy;
    }
    return xx < other.xx;
  }

  bool operator==(const Point& other) const {
    return xx == other.xx && yy == other.yy;
  }
};

long long GetSign(Point prevprev, Point prev, Point p_new) {
  long long x1 = prev.xx - prevprev.xx;
  long long y1 = prev.yy - prevprev.yy;
  long long x2 = p_new.xx - prev.xx;
  long long y2 = p_new.yy - prev.yy;
  return x1 * y2 - x2 * y1;
}

std::vector<Point> Graham(const std::vector<Point>& points) {
  int nn = points.size();
  std::vector<Point> up;
  std::vector<Point> down;
  up.push_back(points[0]);
  down.push_back(points[0]);
  for (int i = 1; i < nn; ++i) {
    while (down.size() >= 2 &&
           GetSign(down[down.size() - 2], down.back(), points[i]) <= 0) {
      down.pop_back();
    }
    down.push_back(points[i]);
    while (up.size() >= 2 &&
           GetSign(up[up.size() - 2], up.back(), points[i]) >= 0) {
      up.pop_back();
    }
    up.push_back(points[i]);
  }
  for (int i = down.size() - 2; i >= 1; --i) {
    up.push_back(down[i]);
  }
  return up;
}

long long GetDoubleArea(const std::vector<Point>& vert) {
  long long ans = 0;
  int nn = vert.size();
  for (int i = 0; i < nn - 1; ++i) {
    ans += static_cast<long long>(vert[i].yy + vert[i + 1].yy) *
           (vert[i + 1].xx - vert[i].xx);
  }
  ans += static_cast<long long>(vert[nn - 1].yy + vert[0].yy) *
         (vert[0].xx - vert[nn - 1].xx);
  return ans;
}

int main() {
  int nn;
  std::cin >> nn;
  std::vector<Point> points(nn);
  for (int i = 0; i < nn; ++i) {
    std::cin >> points[i].xx >> points[i].yy;
  }
  std::sort(points.begin(), points.end());
  points.erase(std::unique(points.begin(), points.end()), points.end());
  std::vector<Point> convex_hull = Graham(points);
  std::cout << convex_hull.size() << "\n";
  for (auto vertex : convex_hull) {
    std::cout << vertex.xx << " " << vertex.yy << "\n";
  }
  long long ans = GetDoubleArea(convex_hull);
  std::cout << ans / 2;
  if (ans % 2 == 1) {
    std::cout << ".5\n";
  } else {
    std::cout << ".0\n";
  }
}
