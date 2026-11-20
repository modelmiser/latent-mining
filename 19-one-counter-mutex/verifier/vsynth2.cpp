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
// guide: only follow choices that agree with a given witness (positive control: a known solution
// must survive every pruning step).  gk/gc/gn = witness tables, gmap = search state -> witness state.
int guided = 0, gk[4][8], gc[4][8], gn[4][8][8], gmap[4][8];
bool gok_target(int p, int q2, int wt, int u0) { // may search-successor q2 stand for witness state wt?
  if (!guided) return true;
  if (q2 < u0) return gmap[p][q2] == wt;
  for (int x = 0; x < u0; x++) if (gmap[p][x] == wt) return false;
  gmap[p][q2] = wt; return true;
}
int nact, staged = 0; // staged: settle processes 1..k (as a k-process system) before process k+1 moves
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
    for (int p = 0; p < nact; p++) {
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
  if (r == 1) { if (nact < n) { nact++; bool ok = rec(); nact--; return ok; } printsol(); return true; }
  int branch = 0; bool top = (depth == 0); depth++;
  #define TRY { if (!top || branch % splitm == splitk) { if (rec()) return true; } branch++; }
  int u0 = inuse[p];
  if (v >= 0) {
    for (int q2 = 1; q2 <= min(u0, s - 1); q2++) { if (guided && !gok_target(p, q2, gn[p][gmap[p][q]][v], u0)) continue; nx[p][q][v] = q2; inuse[p] = max(u0, q2 + 1); TRY; inuse[p] = u0; }
    nx[p][q][v] = -1;
  } else if (q == 1) {
    kind[p][q] = ADD; nx[p][q][0] = 0;
    for (int c = 0; c < b; c++) { if (guided && c != (gk[p][1] == ADD ? gc[p][1] : 0)) continue; cst[p][q] = c; TRY; }
    kind[p][q] = UNDEF;
  } else {
    kind[p][q] = ADD;
    for (int c = 1; c < b; c++) for (int q2 = 1; q2 <= min(u0, s - 1); q2++) { if (guided) { int w = gmap[p][q]; if (gk[p][w] != ADD || gc[p][w] != c || !gok_target(p, q2, gn[p][w][0], u0)) continue; } cst[p][q] = c; nx[p][q][0] = q2; inuse[p] = max(u0, q2 + 1); TRY; inuse[p] = u0; }
    kind[p][q] = READ; for (int w = 0; w < b; w++) nx[p][q][w] = -1;
    if (!guided || gk[p][gmap[p][q]] == READ) TRY;
    kind[p][q] = UNDEF;
  }
  depth--;
  return false;
}
int main(int argc, char **argv) {
  n = atoi(argv[1]); b = atoi(argv[2]); s = atoi(argv[3]);
  if (argc > 6) { splitk = atoi(argv[5]); splitm = atoi(argv[6]); }
  if (argc > 4 && !strcmp(argv[4], "staged")) staged = 1;
  nact = staged ? 1 : n;
  if (getenv("GUIDE")) { // solver-a text format; add 0 at a non-crit state is treated as a read
    guided = 1; FILE *f = fopen(getenv("GUIDE"), "r"); char line[512]; memset(gk, 0, sizeof gk);
    while (fgets(line, sizeof line, f)) { int P, Q, off; char w[32];
      if (sscanf(line, "p %d q %d %31s%n", &P, &Q, w, &off) != 3) continue; P--;
      if (!strcmp(w, "add")) { int c, t; sscanf(line + off, "%d %d", &c, &t); if (c == 0 && Q != 1) { gk[P][Q] = READ; for (int v = 0; v < b; v++) gn[P][Q][v] = t; } else { gk[P][Q] = ADD; gc[P][Q] = c; gn[P][Q][0] = t; } }
      else if (!strcmp(w, "read")) { gk[P][Q] = READ; char *z = line + off; for (int v = 0; v < b; v++) { int t, o2; sscanf(z, "%d%n", &t, &o2); z += o2; gn[P][Q][v] = t; } } }
    for (int p = 0; p < n; p++) { gmap[p][0] = 0; gmap[p][1] = 1; }
  }
  int m = b; mulv.resize(n); for (int p = 0; p < n; p++) { mulv[p] = m; m *= s; } NCFG = m;
  reached.assign(NCFG, 0); tgt.assign((size_t)NCFG * n, -1); num.assign(NCFG, -1); low.assign(NCFG, 0); compId.assign(NCFG, 0); onst.assign(NCFG, 0); inSet.assign(NCFG, 0);
  memset(kind, 0, sizeof kind); memset(nx, -1, sizeof nx); for (int p = 0; p < n; p++) inuse[p] = 2;
  bool ok = rec();
  printf("RESULT n=%d b=%d s=%d staged=%d split=%d/%d %s nodes=%lld\n", n, b, s, staged, splitk, splitm, ok ? "SOLUTION" : "NONE", nodes);
  return 0;
}
