#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;
#define all(x) (x).begin(), (x).end()
const int LEFT = 0;
const int RIGHT = 1;
template<typename T>
using Table = std::vector<std::vector<T>>;

#ifdef LOG
const bool log = true;
#else
const bool log = false;
#endif

struct Train {
  int wall; // knapsack weight
  int human; // knapsack value
};

Table<int> knapsack(int max_wall, const std::vector<Train>& trains) {
  const int sz = max_wall + 1; 
  Table<int> dp(trains.size() + 1);
  dp[0].resize(sz);
  for (int i = 0; i != trains.size(); ++i) {
    dp[i+1].resize(sz);
    for (int w = 0; w <= max_wall; ++w) {
      dp[i+1][w] = std::max({
        dp[i][w],
        w > 0 ? dp[i+1][w-1] : 0,
        w >= trains[i].wall
          ? dp[i][w - trains[i].wall] + trains[i].human
          : 0
      });
    }
  }

  if (log) {
    std::cout << '\n';
    for (auto& vs : dp) {
      for (auto& v : vs) std::cout << v << ' ';
      std::cout << '\n';
    }
    std::cout << '\n';
  }

  return dp;
}

int busan(std::istream& in) {
  int n, max_wall, can_swap;
  in >> n >> max_wall >> can_swap;
  assert(can_swap == 0 || can_swap == 1);
  assert(n <= 2000);
  assert(max_wall <= 3000);

  std::vector<int> inps[2];
  for (auto& vs : inps) {
      vs.resize(n);
      for (auto& v : vs) {
        in >> v;
      }
  }
  
  // preprocess input change to pair
  int always_accept_human = 0;
  std::vector<Train> train[2];
  for (int i = 0; i < n; ++i) {
    int lhs = inps[0][i], rhs = inps[1][i];
    if (log) {
      printf("[%d %d]\n", lhs, rhs);
    }
    if (lhs >= 0 && rhs >= 0) {
      // human human
      always_accept_human += lhs + rhs;
    } else if (lhs > 0 && rhs < 0) {
      train[0].push_back({ -rhs, lhs });
      train[1].push_back({ 0, 0 });
    } else if (lhs < 0 && rhs > 0) {
      train[0].push_back({ 0, 0 });
      train[1].push_back({ -lhs, rhs });
    } else {
      // zombie zombie
    }
  }

  const int n2 = train[0].size(); 

  // calculate knapsack forward and backward
  Table<int> human_forward[2];
  Table<int> human_backward[2];
  for (int i : { LEFT, RIGHT }) {
    human_forward[i] = knapsack(max_wall, train[i]);
    std::reverse(all(train[i]));
    human_backward[i] = knapsack(max_wall, train[i]);
    std::reverse(all(train[i]));
  }

  if (can_swap == 0) {
    return always_accept_human + human_forward[LEFT][n2][max_wall] + human_forward[RIGHT][n2][max_wall];
  } else {
    int mx_human = 0;
    for (int cut = 0; cut <= n2; ++cut) { // cut position
      int best_left = 0;
      int best_right = 0;
      for (int w = 0; w <= max_wall; ++w) {
        best_left = std::max(best_left,
          human_forward[LEFT][cut][w] + human_backward[RIGHT][n2 - cut][max_wall - w]
        );
        best_right = std::max(best_right,
          human_forward[RIGHT][cut][w] + human_backward[LEFT][n2 - cut][max_wall - w]
        );
      }
      if (log) {
        printf("cut at %d; best [%d, %d]\n", cut, best_left, best_right);
      }
      mx_human = std::max(mx_human, best_left + best_right);
    }
    return mx_human + always_accept_human;
  }
}


int main() {
  std::stringstream testcase_1("5 10 0 6 -5 2 -6 -8 -11 1 -3 4 -1");
  assert(6 == busan(testcase_1));
  std::stringstream testcase_2("5 10 1 6 -5 2 -6 -8 -11 1 -3 4 -1");
  assert(7 == busan(testcase_2));
  std::cout << busan(std::cin);
}