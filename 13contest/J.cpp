#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

const int kBinSearch = 1;

const int kPols = 3;

const long double kEqualPrecision = 0.00000001;

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

  long long CrossProduct(Point another) const {
    return xx * another.yy - another.xx * yy;
  }

  long long DotProduct(Point another) const {
    return xx * another.xx + yy * another.yy;
  }

  static long double Dist(Point p1, Point p2) {
    return std::sqrt(static_cast<long long>(p2.xx - p1.xx) * (p2.xx - p1.xx) +
                     static_cast<long long>(p2.yy - p1.yy) * (p2.yy - p1.yy));
  }
};

Point operator-(Point ll, Point rr) {
  return Point{ll.xx - rr.xx, ll.yy - rr.yy};
}

long long GetSign(Point cur1, Point next1, Point cur2, Point next2) {
  long long x1 = next1.xx - cur1.xx;
  long long y1 = next1.yy - cur1.yy;
  long long x2 = next2.xx - cur2.xx;
  long long y2 = next2.yy - cur2.yy;
  return static_cast<long long>(x1) * y2 - static_cast<long long>(x2) * y1;
}

void DeleteExcessPoints(std::vector<Point>& polygon) {
  std::vector<Point> ans;
  ans.reserve(polygon.size());
  long long prev = 1;
  long long prevprev = 0;
  ans.push_back(polygon[0]);
  ans.push_back(polygon[1]);
  for (size_t i = 2; i < polygon.size(); ++i) {
    if (GetSign(polygon[prev], polygon[prevprev], polygon[prev], polygon[i]) ==
        0) {
      ans.pop_back();
    }
    ans.push_back(polygon[i]);
    prevprev = prev;
    prev = i;
  }
  polygon = ans;
}

std::vector<Point> Renum(const std::vector<Point>& pol1) {
  auto iter_min_p = std::min_element(pol1.begin(), pol1.end());
  std::vector<Point> right_vec(iter_min_p, pol1.end());
  right_vec.reserve(pol1.size());
  for (auto it = pol1.begin(); it != iter_min_p; ++it) {
    right_vec.push_back(*it);
  }
  return right_vec;
}

std::vector<Point> MinkSum(std::vector<Point>& pol1, std::vector<Point>& pol2) {
  pol1 = Renum(pol1);
  pol2 = Renum(pol2);
  std::vector<Point> sum;
  sum.reserve(pol1.size() + pol2.size());
  sum.push_back({pol1[0].xx + pol2[0].xx, pol1[0].yy + pol2[0].yy});
  long long n1 = pol1.size();
  long long n2 = pol2.size();
  long long cur_ind1 = 0;
  long long cur_ind2 = 0;
  while (cur_ind1 != n1 && cur_ind2 != n2) {
    long long next1 = (cur_ind1 + 1) % n1;
    long long next2 = (cur_ind2 + 1) % n2;
    Point new_point = sum.back();
    if (GetSign(pol1[cur_ind1], pol1[next1], pol2[cur_ind2], pol2[next2]) >=
        0) {
      new_point.xx += pol1[next1].xx - pol1[cur_ind1].xx;
      new_point.yy += pol1[next1].yy - pol1[cur_ind1].yy;
      ++cur_ind1;
    } else {
      new_point.xx += pol2[next2].xx - pol2[cur_ind2].xx;
      new_point.yy += pol2[next2].yy - pol2[cur_ind2].yy;
      ++cur_ind2;
    }
    sum.push_back(new_point);
  }
  while (cur_ind1 != n1) {
    Point new_point = sum.back();
    long long next1 = (cur_ind1 + 1) % n1;
    new_point.xx += pol1[next1].xx - pol1[cur_ind1].xx;
    new_point.yy += pol1[next1].yy - pol1[cur_ind1].yy;
    ++cur_ind1;
    sum.push_back(new_point);
  }
  while (cur_ind2 != n2) {
    Point new_point = sum.back();
    long long next2 = (cur_ind2 + 1) % n2;
    new_point.xx += pol2[next2].xx - pol2[cur_ind2].xx;
    new_point.yy += pol2[next2].yy - pol2[cur_ind2].yy;
    ++cur_ind2;
    sum.push_back(new_point);
  }
  DeleteExcessPoints(sum);
  sum.pop_back();
  return sum;
}

long double GetAngle(Point pstart, Point pvert, Point pend) {
  Point v1 = {pstart.xx - pvert.xx, pstart.yy - pvert.yy};
  Point v2 = {pend.xx - pvert.xx, pend.yy - pvert.yy};
  long double ans = std::atan2(v1.CrossProduct(v2), v1.DotProduct(v2));
  if (ans < 0) {
    ans += 2 * M_PI;
  }
  return ans;
}

std::vector<long double> GetAngles(const std::vector<Point>& polygon) {
  long long nn = polygon.size();
  std::vector<long double> angle(nn);
  angle[1] = 0;
  for (long long i = 2; i < nn; ++i) {
    angle[i] = GetAngle(polygon[1], polygon[0], polygon[i]);
  }
  return angle;
}

std::pair<int, int> GetEdges(const std::vector<long double>& angle,
                             long double p_angle) {
  int nn = angle.size();
  int rr = nn - 1;
  int ll = 1;
  while (rr - ll > kBinSearch) {
    int mm = (ll + rr) / 2;
    if (angle[mm] - p_angle > -kEqualPrecision) {
      rr = mm;
    } else {
      ll = mm;
    }
  }
  for (int i = std::max(1, ll - kBinSearch);
       i < std::min(nn - 1, rr + kBinSearch); ++i) {
    if (p_angle - angle[i] > -kEqualPrecision &&
        p_angle - angle[i + 1] < kEqualPrecision) {
      return {i, i + 1};
    }
  }
  return {0, 1};
}

bool IsInsidePolygonBinary(const std::vector<Point>& polygon, Point pp,
                           const std::vector<long double>& angle) {
  long long nn = polygon.size();
  long double p_angle = GetAngle(polygon[1], polygon[0], pp);
  if (p_angle - angle[nn - 1] > kEqualPrecision) {
    return false;
  }
  std::pair<long long, long long> edges = GetEdges(angle, p_angle);
  Point sp = {pp.xx - polygon[edges.second].xx,
              pp.yy - polygon[edges.second].yy};
  Point sf = {polygon[edges.first].xx - polygon[edges.second].xx,
              polygon[edges.first].yy - polygon[edges.second].yy};
  long double sign = sp.CrossProduct(sf);
  return sign > -kEqualPrecision;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::vector<std::vector<Point>> pol(3);
  for (int j = 0; j < kPols; ++j) {
    int nn;
    std::cin >> nn;
    pol[j].resize(nn);
    for (int i = 0; i < nn; ++i) {
      std::cin >> pol[j][i].xx >> pol[j][i].yy;
    }
  }
  int req_am;
  std::cin >> req_am;
  std::vector<Point> request(req_am);
  for (int i = 0; i < req_am; ++i) {
    std::cin >> request[i].xx >> request[i].yy;
    request[i].xx *= 3;
    request[i].yy *= 3;
  }
  std::vector<Point> sum01 = MinkSum(pol[0], pol[1]);
  std::vector<Point> sum012 = MinkSum(sum01, pol[2]);
  std::vector<long double> angle = GetAngles(sum012);
  for (int i = 0; i < req_am; ++i) {
    if (IsInsidePolygonBinary(sum012, request[i], angle)) {
      std::cout << "YES\n";
    } else {
      std::cout << "NO\n";
    }
  }
}
