#include <cstring>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <utility>
#include <vector>

using ll = long long;
const int INF = 0x7fffffff;
const int MAX_N = 60;

int edgeCount, transCount;
int unionCount;
int startCount, endCount, finalCount;
int res[10000][10];
int startNode[100];
int endNode[100];
int finalNode[100];

struct edge {
  int to;
  char v;
  edge(int a, char b) {
    to = a;
    v = b;
  }
};

std::vector<edge> G[MAX_N];
std::map<int, int> s;
std::map<int, int>::iterator a;
bool vis[MAX_N];
char trans[10];

void init();
void input(std::ifstream &fin);
void bfs();

int dfs(int from, char key);
// from代表从编号为from的点开始dfs，key代表要经过的权值

void output(std::ofstream &fout);

void change(std::ofstream &fout, int ans);

int main() {
  std::ifstream fin("./build/nfa.txt", std::ios::in);
  std::ofstream fout("./build/dfa.txt", std::ios::out);
  init();
  input(fin);
  bfs();
  output(fout);
  return 0;
}

void init() {
  unionCount = 0;
  finalCount = 0;
  memset(vis, false, sizeof vis);
  memset(res, 0, sizeof res);
  s.clear();
  s.insert(std::make_pair(0, 0));
  for (int i = 0; i < MAX_N; i++)
    G[i].clear();
}

void input(std::ifstream &fin) {
  // 输入弧转换种类个数
  fin >> transCount;

  // 请输入 transCount 个字母代表不同的弧转换类型(空不计算)
  for (int i = 0; i < transCount; i++) {
    fin >> trans[i];
  }

  // 输入起始点的个数
  fin >> startCount;
  // 输入这 startCount 个起始点的编号(用空格隔开)
  for (int i = 0; i < startCount; i++) {
    fin >> startNode[i];
  }

  // 请输入终止点的个数
  fin >> endCount;
  // 输入这 endCount 个终止点的编号(用空格隔开)
  for (int i = 0; i < endCount; i++) {
    fin >> endNode[i];
  }

  // 请输入边的个数
  fin >> edgeCount;

  // 下列 edgeCount 行每行输入 3 个值
  // 分别代表结点的起点、终点、弧转换方式(方式为空输入0)，用空格隔开
  for (int i = 1; i <= edgeCount; i++) {
    int from, to;
    char v;
    fin >> from >> to >> v;
    G[from].push_back({to, v});
  }
}

void bfs() {
  std::queue<int> q;
  int a = 0;
  for (int i = 0; i < startCount; i++) {
    a |= dfs(startNode[i], '0');
  }

  q.push(a);
  int n = 1;
  s.insert(std::make_pair(a, n++));

  while (q.size()) {
    int t = q.front();
    q.pop();

    unionCount++;
    res[unionCount][0] = t;

    for (int i = 0; i < transCount; i++) {
      int cur = t;
      int p = 0;
      int ans = 0;
      while (cur > 0) {
        if (cur & 1) {
          ans |= dfs(p, trans[i]);
        }
        p++;
        cur >>= 1;
      }

      cur = ans;
      p = 0;
      ans = 0;
      while (cur > 0) {
        std::memset(vis, false, sizeof vis);
        if (cur & 1) {
          ans |= dfs(p, '0');
        }
        p++;
        cur >>= 1;
      }

      res[unionCount][i + 1] = ans;

      if (s.find(ans) == s.end() && ans != 0) {
        q.push(ans);
        s.insert(std::make_pair(ans, n++));
      }
    }
  }
}

int dfs(int from, char key) {
  int ans = 0; // 初始化集合为空

  if (key == '0') {
    for (int i = 0; i < G[from].size(); i++) {
      if (G[from][i].v == key && vis[G[from][i].to] == 0) {
        ans |= (1 << G[from][i].to);
        vis[G[from][i].to] = true;
        ans |= dfs(G[from][i].to, key);
      }
    }
    ans |= (1 << from);
  } else {
    for (int i = 0; i < G[from].size(); i++) {
      if (G[from][i].v == key) {
        ans |= (1 << G[from][i].to);
      }
    }
  }
  return ans;
}

void output(std::ofstream &fout) {
  fout << std::left << std::setw(14) << 0;
  for (int i = 0; i < transCount; i++) {
    fout << std::left << std::setw(14) << trans[i];
  }
  fout << '\n';

  for (int i = 1; i <= unionCount; i++) {
    for (int j = 0; j <= transCount; j++) {
      a = s.find(res[i][j]);
      fout << std::left << std::setw(14) << a->second;
    }
    fout << '\n';
  }

  fout << '\n';
  fout << 0;
  change(fout, 0);
  fout << '\n';
  for (int i = 1; i <= unionCount; i++) {
    fout << i;
    change(fout, res[i][0]);
    if (i == 1) {
      fout << "<-起始点";
    }
    for (int j = 0; j < endCount; j++) {
      if ((res[i][0] >> endNode[j]) & 1) {
        fout << "<-终止点";
        finalNode[finalCount++] = i;
        break;
      }
    }

    fout << '\n';
  }
}

void change(std::ofstream &fout, int ans) {
  if (ans == 0) {
    fout << "(null)";
    return;
  }

  int cur = ans;
  int p = 0;
  fout << '(';
  while (cur > 0) {
    if (cur & 1) {
      fout << p << (cur == 1 ? ')' : ' ');
    }
    cur >>= 1;
    p++;
  }
}