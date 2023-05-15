#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cmath>
using namespace std;
typedef long long lli;
#define all(x) (x).begin(), (x).end()

struct Point { lli x; lli y; };
bool compare_slope(const Point& a, const Point& b) {
  return a.x * b.y < b.x * a.y;
}

struct Schedule { Point begin; Point end; };

int oranges(std::istream& in) {
  int n; in >> n;
  std::vector<Schedule> schedules(n);
  for (auto& s : schedules) {
    int x1, y1, x2, y2;
    in >> x1 >> y1 >> x2 >> y2;
    s.begin = Point { x1, y1 };
    s.end = Point { x2, y2 };
    if (compare_slope(s.end, s.begin)) {
      std::swap(s.begin, s.end);
    }
  }
  std::sort(schedules.begin(), schedules.end(), [](const Schedule& lhs, const Schedule& rhs) {
    return compare_slope(lhs.end, rhs.end);
  });

  int count_maximum_schedule = 1;
  Point schedule_end = schedules[0].end;
  for (auto s : schedules) {
    if (compare_slope(schedule_end, s.begin)) {
      schedule_end = s.end;
      count_maximum_schedule += 1;
    }
  }
  return n - count_maximum_schedule;
}


int main() {
  std::stringstream testcase_1("5 4 1 1 5 8 5 6 4 4 5 6 6 5 7 3 6 6 3 9 0");
  assert(1 == oranges(testcase_1));
  std::stringstream testcase_2("6 6 0 8 1 7 4 7 2 0 3 7 6 1 7 2 5 1 6 5 1 4 3 4 5");
  assert(2 == oranges(testcase_2));
  std::stringstream testcase_3("4 6 0 8 1 7 4 7 2 1 7 3 6 6 6 4 5");
  assert(0 == oranges(testcase_3));
  std::stringstream testcase_4("4 1 1 1 1 2 2 2 2 3 3 3 3 4 4 4 4");
  assert(3 == oranges(testcase_4));
  std::cout << oranges(std::cin);
}