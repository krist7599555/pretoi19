// GOD CODE!!!
#include <bits/stdc++.h>
#define for_(i,k,n) for(auto i = k; i != n; ++i) 
#define fori(i,k,n) for(auto i = k; i <= n; ++i) 
using namespace std;
using pii = pair<int, int>;

const int N = 2005, M = 3005;
int n, m, s;
int a[N], b[N], dp[N][M][4];
int ans, ans1, ans2;

int main() {
  scanf("%d %d %d", &n, &m, &s);
  for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
  for (int i = 1; i <= n; i++) scanf("%d", &b[i]);

  // knapsack forward left
  for (int i = 1; i <= n; i++)
    for (int j = 0; j <= m; j++)
      dp[i][j][0] = max(dp[i - 1][j][0], (j + min(0, b[i]) < 0 ? 0 : dp[i - 1][j + min(0, b[i])][0] + a[i]));

  // knapsack forward right
  for (int i = 1; i <= n; i++)
    for (int j = 0; j <= m; j++)
      dp[i][j][1] = max(dp[i - 1][j][1], (j + min(0, a[i]) < 0 ? 0 : dp[i - 1][j + min(0, a[i])][1] + b[i]));

  // knapsack backward left
  for (int i = n; i >= 1; i--)
    for (int j = 0; j <= m; j++)
      dp[i][j][2] = max(dp[i + 1][j][2], (j + min(0, b[i]) < 0 ? 0 : dp[i + 1][j + min(0, b[i])][2] + a[i]));

  // knapsack backward right
  for (int i = n; i >= 1; i--)
    for (int j = 0; j <= m; j++)
      dp[i][j][3] = max(dp[i + 1][j][3], (j + min(0, a[i]) < 0 ? 0 : dp[i + 1][j + min(0, a[i])][3] + b[i]));
  
  // best answer no swap
  ans = max(ans, dp[n][m][0] + dp[n][m][1]);
  if (s) {
    // cut at i
    for (int i = 1; i <= n; i++, ans = max(ans, ans1 + ans2), ans1 = 0, ans2 = 0) {
      for (int j = 0; j <= m; j++) {
        ans1 = max(ans1, dp[i][j][0] + dp[i + 1][m - j][3]);
        ans2 = max(ans2, dp[i][j][1] + dp[i + 1][m - j][2]);
      }
    }
  }
  printf("%d", ans);
}