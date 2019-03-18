#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
using namespace std;
/**
 * @brief      point in the 2D plane
 */
struct point {
  double x;
  double y;
  point() {}
  point(double p, double q) : x(p), y(q) {}
  bool operator==(point &other) const {
    if (x == other.x && y == other.y) {
      return true;
    } else {
      return false;
    }
  }
  bool operator<(point &other) const {
    if (x != other.x) {
      return x < other.x;
    } else {
      return y < other.y;
    }
  }
  bool operator>(point &other) const {
    if (x != other.x) {
      return x > other.x;
    } else {
      return y > other.y;
    }
  }
};
/**
 * @brief      cross product of three points
 *
 * @param      a     common point
 * @param      b     first point
 * @param      c     second point
 *
 * @return     0 if a,b,c are collinier, 1 if a->c is in clock-wise of a->b and
 *             -1 if counter clock-wise
 */
int ccw(point &a, point &b, point &c) {
  double k = ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
  if (k > 0) {
    return 1;
  } else if (k < 0) {
    return -1;
  } else {
    return 0;
  }
}
/**
 * @brief      triangulation of a set of points
 *
 * @param      P     vector of given points
 *
 * @return     set of edges in the resultant triangulation
 */
set<pair<int, int>> triangulate(vector<point> &P) {
  // n is the number of points
  int n = P.size();
  // n must be greater than 2 for triangulation
  assert(n > 2);
  // sorting the given points in ascending order
  sort(P.begin(), P.end());
  // uHull is the upper HUll
  vector<int> uHull;
  // lHull is the lower HUll
  vector<int> lHull;
  // uSz uHull current size
  int uSz = 0;
  // lHull current size
  int lSz = 0;
  // storage for final edges
  set<pair<int, int>> ans;
  for (int i = 0; i < n; i++) {
    // forming the upper for last encountered point
    while (uSz >= 2 && ccw(P[uHull[uSz - 2]], P[uHull[uSz - 1]], P[i]) == 1) {
      ans.insert({uHull[uSz - 1], i});
      uHull.pop_back();
      uSz--;
    }
    if (uSz > 0) {
      ans.insert({uHull[uSz - 1], i});
    }
    uHull.push_back(i);
    uSz++;
    // forming the lower for last encountered point
    while (lSz >= 2 && ccw(P[lHull[lSz - 2]], P[lHull[lSz - 1]], P[i]) == -1) {
      ans.insert({lHull[lSz - 1], i});
      lHull.pop_back();
      lSz--;
    }
    if (lSz > 0) ans.insert({lHull[lSz - 1], i});
    lHull.push_back(i);
    lSz++;
  }
  return ans;
}

int main() {
  cout << "Enter number of points(must be greater than 2): ";
  int n;
  cin >> n;
  cout << "Enter your " << n << " points in" << n
       << "new lines.\nEach line will be of the form x y viz abscissa "
          "ordinate.\n";
  vector<point> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i].x >> a[i].y;
  }
  // triangulation runs
  decltype(auto) ans = triangulate(a);
  ofstream file;
  file.open("temp.dat");
  for (auto &it : ans) {
    file << a[it.first].x << '\t' << a[it.first].y << '\n'
         << a[it.second].x << '\t' << a[it.second].y << "\n\n\n\n";
  }
  file.close();
  system("gnuplot -p -e \"plot 'temp.dat' with linespoint linecolor rgb '#dd181f' "
      "linetype 1 linewidth 2 pointtype 7 pointsize 1.5\"");
  cout << "done" << endl;
}