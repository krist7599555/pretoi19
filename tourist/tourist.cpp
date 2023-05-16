#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <utility>
#include <numeric>
#include <numeric>
#include <variant>
using namespace std;
typedef long long lli;
typedef std::pair<int,int> pii;
#define all(x) (x).begin(), (x).end()
#define for_(i, k, n) for (auto i = (k); i != (n); ++i)
typedef std::vector<int> Vec;

const int MAX_GROUP_NUMBER = 100010;

struct Edge {
  int u;
  int v;
  int dirty_count;
};
struct CloseEdgeCommand {
  Edge* edge;
  int min_group_size;
  int max_group_size;
};
struct QueryCommand {
  int u;
  int v;
  int answer;
};
struct Command {
  enum { CLOSE_EDGE, QUERY } kind;
  std::variant<QueryCommand, CloseEdgeCommand> arg; //! require c++17 olderversion can use `union`
};


struct DisjointSetUnion {
  const int size;
  std::vector<int> vec;
  DisjointSetUnion(int sz): size(sz), vec(sz) {
    reset();
  }
  void reset() {
    std::iota(all(vec), 0);
  }
  bool is_same_group(int u, int v) {
    return parent(u) == parent(v);
  }
  void merge(int u, int v) {
    // fprintf(stderr, "  merge %d %d\n", u, v);
    vec[parent(u)] = parent(v);
  }
  friend std::ostream& operator<< (std::ostream& out, const DisjointSetUnion& dsu) {
    out << "DSU: [";
    for (auto val : dsu.vec) out << val << ' ';
    return out << "]";
  }
private:
  int parent(int i) {
    return vec[i] = vec[i] == i ? i : parent(vec[i]);
  }
};

std::vector<int> tourist(std::istream& in) {
  // std::cerr << "-------------\n";
  int n_node, n_edge, n_tourist, n_question;
  in >> n_node >> n_edge >> n_tourist >> n_question;
  Vec group_ids(MAX_GROUP_NUMBER, 0);
  int max_group_sz = 0;
  for_(_, 0, n_tourist) {
    int group; in >> group;
    group_ids[group] += 1;
    max_group_sz = std::max(max_group_sz, group_ids[group]);
  }
  Vec group_count(max_group_sz + 1); // [group_size] = number of group
  for (int cnt : group_ids) {
    if (cnt > 0) {
      group_count[cnt] += 1;
    }
  }
  std::vector<Edge> edges(n_edge);
  for (auto& e : edges) {
    int u, v;
    in >> u >> v;
    e = Edge { u - 1, v - 1, 0 };
  }
  std::vector<Command> commands(n_question);
  for (auto& cmd : commands) {
    int kind;
    in >> kind;
    if (kind == 1) {
      int edge_idx, li, ri;
      in >> edge_idx >> li >> ri;
      cmd = Command {
        Command::CLOSE_EDGE,
        CloseEdgeCommand { &edges[edge_idx - 1], li, ri } 
      };
    } else {
      assert(kind == 2);
      int s, t;
      in >> s >> t;
      cmd = Command {
        Command::QUERY,
        QueryCommand { s - 1, t - 1, 0 }
      };
    }
  }
  std::reverse(all(commands)); // offline execution

  int exec_count = 0;
  
  DisjointSetUnion dsu(n_node);
  
  for_(group_size, 0, group_count.size()) {
    if (group_count[group_size] > 0) {
      // fprintf(stderr, "\nat group of %d people. have %d group\n", group_size, group_count[group_size]);
      assert(++exec_count < 225); // ถ้ากลุ่มจะไม่ซ้ำเลยคือกลถ่มของ 1คน 2คน 3คน ... โดยผลรวมไม่เกิน 100_000; n(n+1)/2 = 100_000; n = 223.606
      
      dsu.reset();
      
      for (auto& cmd : commands) {
        if (cmd.kind == Command::CLOSE_EDGE) {
          auto arg = std::get<CloseEdgeCommand>(cmd.arg);
          if (arg.min_group_size <= group_size && group_size <= arg.max_group_size) {
            arg.edge->dirty_count += 1; // เส้นนคงอาจจะมีหลายกฏ ที่ทับจำนวนคนเท่ากันได้ เลยจะผ่านได้เมื่อผ่านทุกกฏ
          }
        }
      }
      // std::cerr << "  " << dsu << '\n';
      // std::cerr << "INIT\n";
      
      for (auto& e : edges) {
        if (e.dirty_count == 0) {
            dsu.merge(e.u, e.v); // สร้าง union พื้นฐานจากเส้นที่ไม่ถูกห้าม
            // std::cerr << "  " << dsu << '\n';
        }
      }
      // std::cerr << "START\n";
      for (auto& cmd : commands) {
        if (cmd.kind == Command::CLOSE_EDGE) {
          auto& arg = std::get<CloseEdgeCommand>(cmd.arg);
          if (arg.min_group_size <= group_size && group_size <= arg.max_group_size) {
            // fprintf(stderr, "  reduce edge [%d, %d] dirty_count=%d\n", arg.edge->u, arg.edge->v, arg.edge->dirty_count - 1);
            assert(arg.edge->dirty_count > 0);
            if (--arg.edge->dirty_count == 0) {
              dsu.merge(arg.edge->u, arg.edge->v);
              // std::cerr << "  " << dsu << '\n';
            }
          }
        }
        if (cmd.kind == Command::QUERY) {
          auto& arg = std::get<QueryCommand>(cmd.arg);
          bool is_reachable = dsu.is_same_group(arg.u, arg.v);
          // fprintf(stderr, "  query [%d, %d] %s\n", arg.u, arg.v, is_reachable ? "reachable" : "unreachable");
          arg.answer += is_reachable ? group_count[group_size] : 0; 
        }
      }
      assert(std::all_of(all(edges), [](const auto& it) { return it.dirty_count == 0; })); 
    }
  }

  Vec answers;
  std::reverse(all(commands));
  for (auto& cmd : commands) {
    if (cmd.kind == Command::QUERY) {
      auto& arg = std::get<QueryCommand>(cmd.arg);
      answers.push_back(arg.answer);
      // fprintf(stderr, "ans = %d\n", arg.answer);
    }
  }

  return answers;
}

int main() {
  std::stringstream testcase_1("6 7 10 8 10 3 3 7 3 7 3 3 10 7 3 1 5 4 6 3 1 2 4 6 2 5 3 4 2 3 5 1 4 1 3 1 7 2 5 2 3 5 1 2 2 6 2 3 5 1 1 3 7 2 6 5");
  assert(Vec({3,3,1,0}) == tourist(testcase_1));
  std::stringstream testcase_2("10 1 3 5 1 2 2 1 2 2 1 2 1 1 2 2 2 1 2 1 1 1 3 2 1 2");
  assert(Vec({2,1,0}) == tourist(testcase_2));
  std::stringstream testcase_3("5 4 6 5 49234 10392 49234 81390 81390 10392 5 1 2 3 4 2 1 4 2 5 2 1 4 2 2 1 3 2 2 2 3 5 1 4 2 2");
  assert(Vec({3,0}) == tourist(testcase_3));
  
  for (int i : tourist(std::cin)) {
    std::cout << i << '\n';
  }
}