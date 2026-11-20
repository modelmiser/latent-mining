// Referee's exhaustive search, written from the problem statement.
// usage: vsynth n b s [split k m]   -> prints NONE or a solution (solver-a text format, for vcheck.py)
// Tables are filled lazily: an entry is chosen only when some reachable configuration needs it.
// A partial table is dropped when the configurations it already determines contain two crits or a
// progress violation (both only get worse when entries are added).  A table is accepted when
// nothing reachable is undefined.  State 0 = rem, 1 = crit.  Successors are chosen among states
// already in use plus one new one (renaming of states only).  crit = "add c, go to rem", c in Z_b
// (a read at crit is the same as add 0).  A non-crit state is a read (branches filled lazily) or
// "add c", c != 0 (add 0 is a read with equal branches).
// split k m: only the top-level branches with index % m == k (to use two cores).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
enum { UNDEF = 0, READ = 1, ADD = 2 };
int n, b, s;
int kind[4][8], cst[4][8], nx[4][8][8], inuse[4];
long long nodes = 0;
int splitk = 0, splitm = 1, depth = 0;
int NCFG;
vector<int> mulv;
vector<int> reachList; vector<char> reached;
vector<int> tgt; // tgt[c*n+p] = successor config or -1
int stOf(int c, int p) { return (c / mulv[p]) % s; }

// progress violation among determined part?
vector<int> num, low, compId, stackv; vector<char> onst, inSet; int cnt, ncomp;
int curM;
void dfs(int c) {
  num[c] = low[c] = cnt++; stackv.push_back(c); onst[c] = 1;
  for (int p = 0; p < n; p++) if (curM >> p & 1) {
    int d = tgt[c * n + p]; if (d < 0 || !inSet[d]) continue;
    if (num[d] < 0) { dfs(d); low[c] = min(low[c], low[d]); }
    else if (onst[d]) low[c] = min(low[c], num[d]);
  }
  if (low[c] == num[c]) { while (true) { int w = stackv.back(); stackv.pop_back(); onst[w] = 0; compId[w] = ncomp; if (w == c) break; } ncomp++; }
}
bool badProgress() {
  for (int M = 1; M < (1 << n); M++) {
    curM = M; vector<int> V;
    for (int c : reachList) {
      bool ok = true;
      for (int p = 0; p < n; p++) { int q = stOf(c, p); if (q == 1 || ((q != 0) != ((M >> p & 1) != 0))) { ok = false; break; } }
      if (ok) { V.push_back(c); inSet[c] = 1; num[c] = -1; }
    }
    cnt = 0; ncomp = 0; stackv.clear();
    for (int c : V) if (num[c] < 0) dfs(c);
    vector<int> mv(ncomp, 0);
    for (int c : V) for (int p = 0; p < n; p++) if (M >> p & 1) { int d = tgt[c * n + p]; if (d >= 0 && inSet[d] && compId[d] == compId[c]) mv[compId[c]] |= 1 << p; }
    bool bad = false; for (int k = 0; k < ncomp; k++) if (mv[k] == M) bad = true;
    for (int c : V) inSet[c] = 0;
    if (bad) return true;
  }
  return false;
}
// 0 violation, 1 complete, 2 need (np,nq,nv); nv=-1: the state itself is undefined
int explore(int &np, int &nq, int &nv) {
  for (int c : reachList) reached[c] = 0;
  reachList.clear(); reachList.push_back(0); reached[0] = 1; bool need = false;
  for (size_t h = 0; h < reachList.size(); h++) {
    int c = reachList[h], R = c % b, nc = 0;
    for (int p = 0; p < n; p++) nc += stOf(c, p) == 1;
    if (nc >= 2) return 0;
    for (int p = 0; p < n; p++) {
      int q = stOf(c, p), q2, R2 = R; tgt[c * n + p] = -1;
      if (kind[p][q] == UNDEF) { if (!need) { need = true; np = p; nq = q; nv = -1; } continue; }
      if (kind[p][q] == READ) { q2 = nx[p][q][R]; if (q2 < 0) { if (!need) { need = true; np = p; nq = q; nv = R; } continue; } }
      else { q2 = nx[p][q][0]; R2 = (R + cst[p][q]) % b; }
      int d = c + (q2 - q) * mulv[p] + (R2 - R); tgt[c * n + p] = d;
      if (!reached[d]) { reached[d] = 1; reachList.push_back(d); }
    }
  }
  if (badProgress()) return 0;
  return need ? 2 : 1;
}
void printsol() {
  printf("n %d b %d s %d\n", n, b, s);
  for (int p = 0; p < n; p++) for (int q = 0; q < s; q++) {
    printf("p %d q %d ", p + 1, q);
    if (kind[p][q] == UNDEF) printf("unreached\n");
    else if (kind[p][q] == ADD) printf("add %d %d\n", cst[p][q], nx[p][q][0]);
    else { printf("read"); for (int v = 0; v < b; v++) printf(" %d", nx[p][q][v]); printf("\n"); }
  }
}
bool rec() {
  nodes++;
  int p, q, v; int r = explore(p, q, v);
  if (r == 0) return false;
  if (r == 1) { printsol(); return true; }
  int branch = 0; bool top = (depth == 0); depth++;
  #define TRY { if (!top || branch % splitm == splitk) { if (rec()) return true; } branch++; }
  int u0 = inuse[p];
  if (v >= 0) {
    for (int q2 = 1; q2 <= min(u0, s - 1); q2++) { nx[p][q][v] = q2; inuse[p] = max(u0, q2 + 1); TRY; inuse[p] = u0; }
    nx[p][q][v] = -1;
  } else if (q == 1) {
    kind[p][q] = ADD; nx[p][q][0] = 0;
    for (int c = 0; c < b; c++) { cst[p][q] = c; TRY; }
    kind[p][q] = UNDEF;
  } else {
    kind[p][q] = ADD;
    for (int c = 1; c < b; c++) for (int q2 = 1; q2 <= min(u0, s - 1); q2++) { cst[p][q] = c; nx[p][q][0] = q2; inuse[p] = max(u0, q2 + 1); TRY; inuse[p] = u0; }
    kind[p][q] = READ; for (int w = 0; w < b; w++) nx[p][q][w] = -1;
    TRY;
    kind[p][q] = UNDEF;
  }
  depth--;
  return false;
}
int main(int argc, char **argv) {
  n = atoi(argv[1]); b = atoi(argv[2]); s = atoi(argv[3]);
  if (argc > 6) { splitk = atoi(argv[5]); splitm = atoi(argv[6]); }
  int m = b; mulv.resize(n); for (int p = 0; p < n; p++) { mulv[p] = m; m *= s; } NCFG = m;
  reached.assign(NCFG, 0); tgt.assign((size_t)NCFG * n, -1); num.assign(NCFG, -1); low.assign(NCFG, 0); compId.assign(NCFG, 0); onst.assign(NCFG, 0); inSet.assign(NCFG, 0);
  memset(kind, 0, sizeof kind); memset(nx, -1, sizeof nx); for (int p = 0; p < n; p++) inuse[p] = 2;
  bool ok = rec();
  printf("RESULT n=%d b=%d s=%d split=%d/%d %s nodes=%lld\n", n, b, s, splitk, splitm, ok ? "SOLUTION" : "NONE", nodes);
  return 0;
}
