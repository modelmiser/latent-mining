// synth.cpp -- exhaustive lazy synthesis of mutual-exclusion automata.
// usage: synth n b s [maxnodes] [flags]
//   flags: "neg"  = break the Z_b negation symmetry is NOT applied (nothing pruned by default
//                   except state renaming; see below).
// State numbering per process: 0 = rem, 1 = crit, 2.. = other states.
// Search: transition entries are defined lazily, only when the reachable-set
// exploration needs them.  A partial table is rejected as soon as the part of
// the configuration graph it already determines contains
//   (a) two processes in crit, or
//   (b) a progress violation (a reachable crit-free strongly connected set, with
//       active set exactly S != {}, containing a step of every process of S).
// Both are monotone: defining more entries only adds configurations/edges, so a
// rejected partial table has no correct completion.  Pruning used:
//   * state renaming: a successor may be an already-used state or the single
//     next fresh one (canonical numbering by first use);
//   * "add 0" is not offered for non-crit states (it equals a read whose
//     successors all coincide, which the lazy read branch covers);
//   * crit is "add c -> rem" (a read with all successors rem equals add 0).
// A table is accepted when exploration needs no undefined entry; entries never
// needed are unreachable under every schedule and may be filled arbitrarily.
#include <bits/stdc++.h>
using namespace std;
int n, b, s;
const int MN = 4, MS = 10, MB = 8;
int typ[MN][MS];            // 0 undef, 1 read, 2 add
int addc[MN][MS];
int succ_[MN][MS][MB];      // -1 undef; add uses index 0
int used_[MN];
long long nodes = 0, maxnodes = -1, nsol = 0;
bool countall = false;
int NC;                     // number of configs
vector<int> pw;             // pw[i] = multiplier for process i

inline int stOf(int cfg, int i) { return (cfg / pw[i]) % s; }
inline int rOf(int cfg) { return cfg % b; }

// exploration buffers
vector<int> seen, order_;
vector<array<int, MN>> edge; // edge[cfg][i] = target or -1
int seenStamp = 0;
vector<int> stamp;

struct Need { int i, q, v; };

// tarjan
vector<int> idx, low, comp, stk; vector<char> onstk, inG;
int tcount, ccount;

bool progressViolation() {
  int full = (1 << n);
  for (int S = 1; S < full; S++) {
    // nodes: reachable configs, no crit, active set == S
    vector<int> nodesS;
    for (int c : order_) {
      bool ok = true;
      for (int i = 0; i < n && ok; i++) {
        int q = stOf(c, i);
        if (q == 1) ok = false;
        if (((S >> i) & 1) != (q != 0)) ok = false;
      }
      inG[c] = ok;
      if (ok) nodesS.push_back(c);
    }
    if (!nodesS.empty()) {
      for (int c : nodesS) { idx[c] = -1; onstk[c] = 0; }
      tcount = 0; ccount = 0; stk.clear();
      // iterative tarjan
      for (int root : nodesS) if (idx[root] < 0) {
        vector<pair<int,int>> cs; cs.push_back({root, 0});
        idx[root] = low[root] = tcount++; stk.push_back(root); onstk[root] = 1;
        while (!cs.empty()) {
          int c = cs.back().first; int &k = cs.back().second;
          if (k < n) {
            int i = k++;
            if (!((S >> i) & 1)) continue;
            int t = edge[c][i];
            if (t < 0 || !inG[t]) continue;
            if (idx[t] < 0) { idx[t] = low[t] = tcount++; stk.push_back(t); onstk[t] = 1; cs.push_back({t, 0}); }
            else if (onstk[t]) low[c] = min(low[c], idx[t]);
          } else {
            if (low[c] == idx[c]) {
              while (true) { int w = stk.back(); stk.pop_back(); onstk[w] = 0; comp[w] = ccount; if (w == c) break; }
              ccount++;
            }
            cs.pop_back();
            if (!cs.empty()) { int p = cs.back().first; low[p] = min(low[p], low[c]); }
          }
        }
      }
      vector<int> mask(ccount, 0);
      for (int c : nodesS) for (int i = 0; i < n; i++) if ((S >> i) & 1) {
        int t = edge[c][i];
        if (t >= 0 && inG[t] && comp[t] == comp[c]) mask[comp[c]] |= 1 << i;
      }
      for (int k = 0; k < ccount; k++) if (mask[k] == S) { for (int c : nodesS) inG[c] = 0; return true; }
    }
    for (int c : nodesS) inG[c] = 0;
  }
  return false;
}

// returns: 0 = violation, 1 = complete & correct, 2 = need (filled)
int explore(Need &need) {
  seenStamp++;
  order_.clear();
  bool haveNeed = false;
  int init = 0; // all rem, R=0
  stamp[init] = seenStamp; order_.push_back(init);
  for (size_t h = 0; h < order_.size(); h++) {
    int c = order_[h];
    int R = rOf(c);
    int ncrit = 0;
    for (int i = 0; i < n; i++) if (stOf(c, i) == 1) ncrit++;
    if (ncrit >= 2) return 0;
    for (int i = 0; i < n; i++) {
      int q = stOf(c, i);
      edge[c][i] = -1;
      int t;
      if (typ[i][q] == 0) { if (!haveNeed) { need = {i, q, -1}; haveNeed = true; } continue; }
      if (typ[i][q] == 1) {
        int q2 = succ_[i][q][R];
        if (q2 < 0) { if (!haveNeed) { need = {i, q, R}; haveNeed = true; } continue; }
        t = c + (q2 - q) * pw[i];
      } else {
        int q2 = succ_[i][q][0];
        int R2 = (R + addc[i][q]) % b;
        t = c + (q2 - q) * pw[i] + (R2 - R);
      }
      edge[c][i] = t;
      if (stamp[t] != seenStamp) { stamp[t] = seenStamp; order_.push_back(t); }
    }
  }
  if (progressViolation()) return 0;
  return haveNeed ? 2 : 1;
}

void printSolution(FILE *f) {
  fprintf(f, "n %d b %d s %d\n", n, b, s);
  for (int i = 0; i < n; i++) {
    for (int q = 0; q < s; q++) {
      fprintf(f, "p %d q %d ", i + 1, q);
      if (typ[i][q] == 0) fprintf(f, "unreached\n");
      else if (typ[i][q] == 2) fprintf(f, "add %d %d\n", addc[i][q], succ_[i][q][0]);
      else { fprintf(f, "read"); for (int v = 0; v < b; v++) fprintf(f, " %d", succ_[i][q][v]); fprintf(f, "\n"); }
    }
  }
}

bool search_() {
  nodes++;
  if (maxnodes >= 0 && nodes > maxnodes) { printf("NODE LIMIT\n"); exit(3); }
  Need nd;
  int r = explore(nd);
  if (r == 0) return false;
  if (r == 1) {
    nsol++;
    if (nsol == 1) { printSolution(stdout); fflush(stdout); }
    return !countall;
  }
  int i = nd.i, q = nd.q;
  if (nd.v < 0) {
    if (q == 1) {
      typ[i][q] = 2; succ_[i][q][0] = 0;
      for (int c = 0; c < b; c++) { addc[i][q] = c; if (search_()) return true; }
      typ[i][q] = 0;
      return false;
    }
    // read (successors lazily)
    typ[i][q] = 1;
    for (int v = 0; v < b; v++) succ_[i][q][v] = -1;
    if (search_()) return true;
    // add c != 0
    typ[i][q] = 2;
    int u0 = used_[i];
    for (int c = 1; c < b; c++) {
      addc[i][q] = c;
      int top = min(u0, s - 1);
      for (int q2 = 1; q2 <= top; q2++) {
        succ_[i][q][0] = q2;
        used_[i] = max(u0, q2 + 1);
        if (search_()) return true;
        used_[i] = u0;
      }
    }
    typ[i][q] = 0;
    return false;
  } else {
    int u0 = used_[i];
    int top = min(u0, s - 1);
    for (int q2 = 1; q2 <= top; q2++) {
      succ_[i][q][nd.v] = q2;
      used_[i] = max(u0, q2 + 1);
      if (search_()) return true;
      used_[i] = u0;
    }
    succ_[i][q][nd.v] = -1;
    return false;
  }
}

int main(int argc, char **argv) {
  n = atoi(argv[1]); b = atoi(argv[2]); s = atoi(argv[3]);
  if (argc > 4) maxnodes = atoll(argv[4]);
  if (argc > 5 && string(argv[5]) == "count") countall = true;
  pw.assign(n, 0);
  int m = b; for (int i = 0; i < n; i++) { pw[i] = m; m *= s; }
  NC = m;
  stamp.assign(NC, 0); edge.assign(NC, {}); idx.assign(NC, -1); low.assign(NC, 0); comp.assign(NC, 0);
  onstk.assign(NC, 0); inG.assign(NC, 0);
  for (int i = 0; i < n; i++) { used_[i] = 2; for (int q = 0; q < s; q++) { typ[i][q] = 0; for (int v = 0; v < MB; v++) succ_[i][q][v] = -1; } }
  bool ok = search_();
  if (countall) printf("RESULT n=%d b=%d s=%d solutions(partial-table leaves)=%lld nodes=%lld\n", n, b, s, nsol, nodes);
  else printf("RESULT n=%d b=%d s=%d %s nodes=%lld\n", n, b, s, ok ? "SOLUTION" : "NONE", nodes);
  return 0;
}
