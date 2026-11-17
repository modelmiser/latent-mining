// Exhaustive solver for the online switch-cost lateness game.
// usage: game P T MODE [OC=1] [MAXJ=-1] [POL=0] [LFIX=-1] [PRINT=0]
//  MODE 0: free promise (preemptive EDF demand vector)
//  MODE 1: same-cost promise (exact frontier of hindsight machine states, switch cost 1)
//  MODE 2: free promise via the frontier machinery with switch cost 0 (cross-check of MODE 0)
//  OC   : online machine switch cost (1 normal, 0 sanity check)
//  MAXJ : max number of jobs the adversary may release per tick (-1 = unlimited = exhaustive)
//  POL  : 0 any scheduler; 1 scheduler never idles with work pending; 2 non-preemptive EDF;
//         3 never switch (but may idle / choose freely)
//  LFIX : if >=0 only decide "can adversary force lateness >= LFIX+1"
//  PRINT: 1 = print an adversary strategy tree for L = value-1
#include <bits/stdc++.h>
using namespace std;
int P, T, L, MODE, OC = 1, MAXJ = -1, POL = 0;
int PMASK = -1; // env PMASK: bitmask of allowed job lengths (bit p). Restricting it gives LOWER bounds only.

struct M { vector<uint8_t> jobs; uint8_t A = 0, flag = 0; };
static inline int REM(uint8_t c) { return c & 7; }
static inline int DL(uint8_t c) { return c >> 3; }
static inline uint8_t CODE(int d, int rem) { return (uint8_t)(d * 8 + rem); }
static string mkey(const M &m) {
  string s; s.reserve(2 + m.jobs.size());
  s.push_back((char)m.A); s.push_back((char)m.flag);
  s.append((const char *)m.jobs.data(), m.jobs.size());
  return s;
}
static M unkey(const string &s) {
  M m; m.A = (uint8_t)s[0]; m.flag = (uint8_t)s[1];
  m.jobs.assign(s.begin() + 2, s.end());
  return m;
}
static void addjob(M &m, uint8_t c) {
  m.jobs.insert(upper_bound(m.jobs.begin(), m.jobs.end(), c), c);
}
// all machine moves from m with switch cost `cost`
static void succ(const M &m, int cost, vector<M> &out) {
  out.clear();
  uint8_t prev = 0;
  for (size_t i = 0; i < m.jobs.size(); i++) {
    uint8_t c = m.jobs[i];
    if (c == prev) continue;
    prev = c;
    if (!(cost == 0 || m.A == 0 || c == m.A || m.flag)) continue;
    M n; n.jobs = m.jobs; n.jobs.erase(n.jobs.begin() + i);
    if (REM(c) > 1) { addjob(n, c - 1); n.A = (cost == 0) ? 0 : (uint8_t)(c - 1); }
    else n.A = 0;
    n.flag = 0;
    out.push_back(n);
  }
  { M n = m; n.flag = 0; out.push_back(n); }                 // idle
  if (cost == 1 && m.A != 0) { M n = m; n.flag = 1; out.push_back(n); } // switch tick
}
// ---------- hindsight viability (no further releases) ----------
unordered_map<string, char> viaMemo;
int HC; // hindsight cost
static bool viable(int t, const M &m) {
  if (m.jobs.empty()) return true;
  for (uint8_t c : m.jobs) if (t + REM(c) > DL(c)) return false;
  string k; k.push_back((char)t); k += mkey(m);
  auto it = viaMemo.find(k);
  if (it != viaMemo.end()) return it->second;
  vector<M> ss; succ(m, HC, ss);
  bool r = false;
  for (auto &s : ss) if (viable(t + 1, s)) { r = true; break; }
  viaMemo[k] = r;
  return r;
}
// ---------- promise states ----------
vector<vector<string>> fronts;       // MODE 1/2: sorted set of machine-state keys
vector<vector<uint8_t>> edfs;        // MODE 0: W[d]
unordered_map<string, int> internMap;
unordered_map<uint64_t, int> addMemo, stepMemo;
int UW = 99, LD = 99; // env UW: p<P jobs only with d-r <= UW ; env LD: p=P jobs only with d >= T-LD (restrictions => LOWER bounds only)
int MAXLONG = 99, NL = 0; // env MAXLONG: at most this many jobs with p>=2 in the whole game (restriction => LOWER bound only)
int DOM = 0; // env DOM=1: reduce each frontier to its non-dominated states (exact; see proofs.txt, Lemma D)
// does hindsight state a dominate b?  (a can imitate every continuation of b)
static bool dominates(const string &a, const string &b) {
  uint8_t A1 = a[0], f1 = a[1], A2 = b[0], f2 = b[1];
  vector<uint8_t> x(a.begin() + 2, a.end()), y(b.begin() + 2, b.end());
  if (x.size() > y.size()) return false;
  if (!(A1 == 0 || f1 == 1)) {
    if (A2 == 0 || DL(A1) != DL(A2) || REM(A1) > REM(A2) || f1 < f2) return false;
    x.erase(find(x.begin(), x.end(), A1)); y.erase(find(y.begin(), y.end(), A2));
  }
  // per deadline: i-th largest rem of x <= i-th largest rem of y ; lists are sorted ascending by (d,rem)
  int i = (int)x.size() - 1, j = (int)y.size() - 1;
  while (i >= 0) {
    while (j >= 0 && DL(y[j]) > DL(x[i])) j--;
    if (j < 0 || DL(y[j]) != DL(x[i]) || REM(y[j]) < REM(x[i])) return false;
    i--; j--;
  }
  return true;
}
static int internFront(vector<string> &v) {
  sort(v.begin(), v.end()); v.erase(unique(v.begin(), v.end()), v.end());
  if (v.empty()) return -1;
  if (DOM && v.size() > 1) {
    vector<string> w;
    for (size_t i = 0; i < v.size(); i++) {
      bool dom = false;
      for (size_t j = 0; j < v.size() && !dom; j++) {
        if (i == j) continue;
        if (dominates(v[j], v[i]) && !(dominates(v[i], v[j]) && i < j)) dom = true;
      }
      if (!dom) w.push_back(v[i]);
    }
    v.swap(w);
  }
  string k;
  for (auto &s : v) { k.push_back((char)s.size()); k += s; }
  auto it = internMap.find(k);
  if (it != internMap.end()) return it->second;
  int id = fronts.size(); fronts.push_back(v); internMap[k] = id; return id;
}
static int internEdf(const vector<uint8_t> &w) {
  string k((const char *)w.data(), w.size());
  auto it = internMap.find(k);
  if (it != internMap.end()) return it->second;
  int id = edfs.size(); edfs.push_back(w); internMap[k] = id; return id;
}
static int promAdd(int pid, int t, int p, int d) {
  uint64_t k = ((uint64_t)pid << 20) | (t << 12) | (p << 6) | d;
  auto it = addMemo.find(k);
  if (it != addMemo.end()) return it->second;
  int r;
  if (MODE == 0) {
    vector<uint8_t> w = edfs[pid]; w[d] += p;
    int sum = 0; bool ok = true;
    for (int e = t + 1; e <= T; e++) { sum += w[e]; if (sum > e - t) { ok = false; break; } }
    r = ok ? internEdf(w) : -1;
  } else {
    vector<string> v;
    for (auto &s : fronts[pid]) { M m = unkey(s); addjob(m, CODE(d, p)); if (viable(t, m)) v.push_back(mkey(m)); }
    r = internFront(v);
  }
  addMemo[k] = r; return r;
}
static int promStep(int pid, int t) {
  uint64_t k = ((uint64_t)pid << 8) | t;
  auto it = stepMemo.find(k);
  if (it != stepMemo.end()) return it->second;
  int r;
  if (MODE == 0) {
    vector<uint8_t> w = edfs[pid];
    for (int e = t + 1; e <= T; e++) if (w[e]) { w[e]--; break; }
    r = internEdf(w);
  } else {
    vector<string> v; vector<M> ss;
    for (auto &s : fronts[pid]) { M m = unkey(s); succ(m, HC, ss); for (auto &n : ss) if (viable(t + 1, n)) v.push_back(mkey(n)); }
    r = internFront(v);
    if (r < 0) { fprintf(stderr, "BUG: empty frontier after step\n"); exit(2); }
  }
  stepMemo[k] = r; return r;
}
// ---------- the game ----------
unordered_map<string, char> gmemo;
long long nodes = 0;
static bool dead(const M &m, int t) {
  for (uint8_t c : m.jobs) if (t + REM(c) > DL(c) + L) return true;
  return false;
}
// minimal-lateness test for the online machine alone (no further releases): can all jobs finish by d+ell ?
unordered_map<string, char> soloMemo;
static bool soloOK(int t, const M &m, int ell) {
  if (m.jobs.empty()) return true;
  for (uint8_t c : m.jobs) if (t + REM(c) > DL(c) + ell) return false;
  string k; k.push_back((char)t); k.push_back((char)ell); k += mkey(m);
  auto it = soloMemo.find(k);
  if (it != soloMemo.end()) return it->second;
  vector<M> ss; succ(m, OC, ss);
  bool r = false;
  for (auto &s : ss) if (soloOK(t + 1, s, ell)) { r = true; break; }
  soloMemo[k] = r; return r;
}
static int soloVal(int t, const M &m) { int e = 0; while (!soloOK(t, m, e)) e++; return e; }
static void onlineMoves(const M &on, vector<M> &out, int t) {
  if (on.jobs.empty()) { out.clear(); M n = on; n.flag = 0; n.A = 0; out.push_back(n); return; }
  if (POL >= 4) {
    // candidate order: run A, run others by (deadline, remaining), switch, idle
    // (POL 6: jobs by (deadline, LONGEST remaining first); POL 7: idle before starting a fresh job when A is none)
    vector<M> all; succ(on, OC, all);
    vector<M> runs, cand; M sw, idle; bool hasSw = false;
    for (auto &n : all) { if (n.jobs != on.jobs) runs.push_back(n); else if (n.flag) { sw = n; hasSw = true; } else idle = n; }
    // identify run-A move: the one whose job list equals on.jobs with A decremented
    int ai = -1;
    if (on.A) for (size_t i = 0; i < runs.size(); i++) {
      M x; x.jobs = on.jobs; x.jobs.erase(find(x.jobs.begin(), x.jobs.end(), on.A)); if (REM(on.A) > 1) addjob(x, on.A - 1);
      if (x.jobs == runs[i].jobs) ai = i;
    }
    if (ai >= 0) cand.push_back(runs[ai]);
    for (size_t i = 0; i < runs.size(); i++) if ((int)i != ai) cand.push_back(runs[i]); // succ order = code order = (d, rem)
    if (hasSw) cand.push_back(sw);
    cand.push_back(idle);
    out.clear();
    if (POL == 4) {
      for (auto &n : cand) if (soloOK(t + 1, n, L)) { out.push_back(n); return; }
      out.push_back(cand[0]); return;
    }
    int best = 1000, bi = 0;
    for (size_t i = 0; i < cand.size(); i++) { int v = soloVal(t + 1, cand[i]); if (v < best) { best = v; bi = i; } }
    out.push_back(cand[bi]); return;
  }
  if (POL == 2) {
    out.clear();
    uint8_t c = on.A ? on.A : on.jobs[0];
    M n; n.jobs = on.jobs; n.jobs.erase(find(n.jobs.begin(), n.jobs.end(), c));
    if (REM(c) > 1) { addjob(n, c - 1); n.A = c - 1; } else n.A = 0;
    out.push_back(n); return;
  }
  succ(on, OC, out);
  if (POL == 1) { // drop idle (the move with same jobs, flag 0)
    vector<M> o2;
    for (auto &n : out) if (!(n.jobs == on.jobs && n.flag == 0)) o2.push_back(n);
    out.swap(o2);
  }
  if (POL == 3) {
    vector<M> o2;
    for (auto &n : out) if (!(n.jobs == on.jobs && n.flag == 1)) o2.push_back(n);
    out.swap(o2);
  }
}
static bool advWins(int t, int pid, const M &on);
static bool schedLoses(int t, int pid, const M &on) {
  if (dead(on, t)) return true;
  if (t >= T && on.jobs.empty()) return false;
  string k; k.push_back('S'); k.push_back((char)NL); k.push_back((char)t); k.append((const char *)&pid, 4); k += mkey(on);
  auto it = gmemo.find(k);
  if (it != gmemo.end()) return it->second;
  nodes++;
  vector<M> ss; onlineMoves(on, ss, t);
  int npid = (t + 1 < T) ? promStep(pid, t) : -1;
  bool r = true;
  for (auto &s : ss) {
    if (dead(s, t + 1)) continue;
    if (!advWins(t + 1, npid, s)) { r = false; break; }
  }
  gmemo[k] = r; return r;
}
static bool enumR(int t, int pid, const M &on, int startType, int cnt) {
  if (schedLoses(t, pid, on)) return true;
  if (MAXJ >= 0 && cnt >= MAXJ) return false;
  int idx = 0;
  for (int d = t + 1; d <= T; d++)
    for (int p = 1; p <= P && p <= d - t; p++, idx++) {
      if (idx < startType) continue;
      if (!((PMASK >> p) & 1)) continue;
      if (p < P && d - t > UW) continue;
      if (p == P && d < T - LD) continue;
      int pid2 = promAdd(pid, t, p, d);
      if (pid2 < 0) continue;
      if (p >= 2 && NL >= MAXLONG) continue;
      M on2 = on; addjob(on2, CODE(d, p));
      if (p >= 2) NL++;
      bool rr = enumR(t, pid2, on2, idx, cnt + 1);
      if (p >= 2) NL--;
      if (rr) return true;
    }
  return false;
}
static bool advWins(int t, int pid, const M &on) {
  if (dead(on, t)) return true;
  if (t >= T) return schedLoses(t, -1, on);
  string k; k.push_back('A'); k.push_back((char)NL); k.push_back((char)t); k.append((const char *)&pid, 4); k += mkey(on);
  auto it = gmemo.find(k);
  if (it != gmemo.end()) return it->second;
  bool r = enumR(t, pid, on, 0, 0);
  gmemo[k] = r; return r;
}
// ---------- strategy printing ----------
static string show(const M &m) {
  string s = "{";
  for (uint8_t c : m.jobs) { char b[32]; snprintf(b, 32, "(rem%d,d%d)", REM(c), DL(c)); s += b; }
  char b[48]; if (m.A) snprintf(b, 48, "} A=(rem%d,d%d)%s", REM(m.A), DL(m.A), m.flag ? " sw" : ""); else snprintf(b, 48, "} A=-");
  return s + b;
}
static bool findR(int t, int pid, const M &on, int startType, int cnt, vector<pair<int,int>> &R, int &opid, M &oon, int lim) {
  if (cnt == lim) { if (schedLoses(t, pid, on)) { opid = pid; oon = on; return true; } return false; }
  int idx = 0;
  for (int d = t + 1; d <= T; d++)
    for (int p = 1; p <= P && p <= d - t; p++, idx++) {
      if (idx < startType) continue;
      if (!((PMASK >> p) & 1)) continue;
      if (p < P && d - t > UW) continue;
      if (p == P && d < T - LD) continue;
      int pid2 = promAdd(pid, t, p, d);
      if (pid2 < 0) continue;
      M on2 = on; addjob(on2, CODE(d, p));
      if (p >= 2 && NL >= MAXLONG) continue;
      R.push_back({p, d});
      if (p >= 2) NL++;
      bool rr = findR(t, pid2, on2, idx, cnt + 1, R, opid, oon, lim);
      if (p >= 2) NL--;
      if (rr) return true;
      R.pop_back();
    }
  return false;
}
set<string> printed;
static void printTree(int t, int pid, const M &on, int depth) {
  string ind(depth * 2, ' ');
  if (dead(on, t)) { printf("%s[t=%d] %s  => some job cannot finish by d+%d: ADVERSARY WINS\n", ind.c_str(), t, show(on).c_str(), L); return; }
  if (t >= T) { printf("%s[t=%d] %s  => no more releases; scheduler alone cannot avoid lateness %d (exhaustive)\n", ind.c_str(), t, show(on).c_str(), L + 1); return; }
  string k; k.push_back((char)t); k.append((const char *)&pid, 4); k += mkey(on);
  if (printed.count(k)) { printf("%s[t=%d] %s  => (position already expanded above)\n", ind.c_str(), t, show(on).c_str()); return; }
  printed.insert(k);
  vector<pair<int,int>> R; int opid = pid; M oon = on; bool ok = false;
  for (int lim = 0; lim <= 12 && !ok; lim++) { R.clear(); ok = findR(t, pid, on, 0, 0, R, opid, oon, lim); }
  if (!ok) { printf("%s?? no winning release found\n", ind.c_str()); return; }
  string rs;
  for (auto &pr : R) { char b[40]; snprintf(b, 40, "(r%d,p%d,d%d) ", t, pr.first, pr.second); rs += b; }
  if (R.empty()) rs = "nothing";
  printf("%s[t=%d] online %s : adversary releases %s\n", ind.c_str(), t, show(on).c_str(), rs.c_str());
  if (dead(oon, t)) { printf("%s  => a job cannot finish by d+%d: ADVERSARY WINS\n", ind.c_str(), L); return; }
  vector<M> ss; onlineMoves(oon, ss, t);
  int npid = (t + 1 < T) ? promStep(opid, t) : -1;
  for (auto &s : ss) {
    const char *what = (s.jobs != oon.jobs) ? "run" : (s.flag ? "switch" : "idle");
    printf("%s  scheduler %s -> %s\n", ind.c_str(), what, show(s).c_str());
    printTree(t + 1, npid, s, depth + 2);
  }
}
// ---------- machine-readable strategy dump (checked by verify.cpp, an independent implementation) ----------
static string canon(const M &m) {
  string s = to_string((int)m.A) + ":" + to_string((int)m.flag) + ":";
  for (size_t i = 0; i < m.jobs.size(); i++) { if (i) s += "."; s += to_string((int)m.jobs[i]); }
  return s;
}
unordered_map<string, int> dumpId; int dumpN = 0; FILE *dumpF = nullptr;
static int dumpTree(int t, int pid, const M &on) {
  string k; k.push_back((char)NL); k.push_back((char)t); k.append((const char *)&pid, 4); k += mkey(on);
  auto it = dumpId.find(k);
  if (it != dumpId.end()) return it->second;
  int id = dumpN++; dumpId[k] = id;
  vector<pair<int,int>> R; int opid = pid; M oon = on; bool ok = false;
  for (int lim = 0; lim <= 12 && !ok; lim++) { R.clear(); ok = findR(t, pid, on, 0, 0, R, opid, oon, lim); }
  if (!ok) { fprintf(stderr, "dump: no winning release\n"); exit(3); }
  int nlong = 0; for (auto &pr : R) if (pr.first >= 2) nlong++;
  string line = "node " + to_string(id) + " " + to_string(t) + " " + to_string(R.size());
  for (auto &pr : R) line += " " + to_string(pr.first) + " " + to_string(pr.second);
  vector<pair<string,int>> ch;
  if (!dead(oon, t)) {
    vector<M> ss; onlineMoves(oon, ss, t);
    int npid = (t + 1 < T) ? promStep(opid, t) : -1;
    NL += nlong;
    for (auto &s : ss) {
      int cid;
      if (dead(s, t + 1)) cid = -1; else if (t + 1 >= T) cid = -2; else cid = dumpTree(t + 1, npid, s);
      ch.push_back({canon(s), cid});
    }
    NL -= nlong;
  }
  line += " " + to_string(ch.size());
  for (auto &c : ch) line += " " + c.first + " " + to_string(c.second);
  fprintf(dumpF, "%s\n", line.c_str());
  return id;
}
// ---------- feasibility cross-check dump: every job multiset with <= K jobs, as judged by the promise machinery ----------
static void feasDump(int t, int pid, vector<array<int,3>> &lst, int startType, int K, FILE *f) {
  if (t >= T) return;
  if ((int)lst.size() < K) {
    int idx = 0;
    for (int d = t + 1; d <= T; d++)
      for (int p = 1; p <= P && p <= d - t; p++, idx++) {
        if (idx < startType) continue;
        int pid2 = promAdd(pid, t, p, d);
        lst.push_back({t, p, d});
        fprintf(f, "feas %d %zu", pid2 >= 0 ? 1 : 0, lst.size());
        for (auto &a : lst) fprintf(f, " %d %d %d", a[0], a[1], a[2]);
        fprintf(f, "\n");
        if (pid2 >= 0) feasDump(t, pid2, lst, idx, K, f);
        lst.pop_back();
      }
  }
  feasDump(t + 1, promStep(pid, t), lst, 0, K, f);
}
int main(int argc, char **argv) {
  if (argc < 4) { fprintf(stderr, "usage\n"); return 1; }
  P = atoi(argv[1]); T = atoi(argv[2]); MODE = atoi(argv[3]);
  if (argc > 4) OC = atoi(argv[4]);
  if (argc > 5) MAXJ = atoi(argv[5]);
  if (argc > 6) POL = atoi(argv[6]);
  int LFIX = argc > 7 ? atoi(argv[7]) : -1;
  int PRINT = argc > 8 ? atoi(argv[8]) : 0;
  HC = (MODE == 1) ? 1 : 0;
  if (getenv("DOM")) DOM = atoi(getenv("DOM"));
  if (getenv("MAXLONG")) MAXLONG = atoi(getenv("MAXLONG"));
  if (getenv("UW")) UW = atoi(getenv("UW"));
  if (getenv("LD")) LD = atoi(getenv("LD"));
  if (getenv("PMASK")) PMASK = atoi(getenv("PMASK"));
  int pid0;
  if (MODE == 0) pid0 = internEdf(vector<uint8_t>(T + 1, 0));
  else { vector<string> v{mkey(M())}; pid0 = internFront(v); }
  M empty;
  if (getenv("FEASDUMP")) {
    FILE *f = fopen(getenv("FEASDUMP"), "w");
    fprintf(f, "header P %d T %d L 0 mode %d\n", P, T, MODE == 1 ? 1 : 0);
    vector<array<int,3>> lst; feasDump(0, pid0, lst, 0, getenv("FEASK") ? atoi(getenv("FEASK")) : 4, f);
    fclose(f); return 0;
  }
  int value = -1;
  int lo = LFIX >= 0 ? LFIX : 0, hi = LFIX >= 0 ? LFIX : 3 * P + 3;
  for (L = lo; L <= hi; L++) {
    gmemo.clear(); nodes = 0;
    bool w = advWins(0, pid0, empty);
    printf("PMASK=%d P=%d T=%d MODE=%d OC=%d MAXJ=%d POL=%d : adversary forces lateness >= %d ? %s   (memo %zu, promise states %zu, viaMemo %zu)\n",
           PMASK, P, T, MODE, OC, MAXJ, POL, L + 1, w ? "YES" : "NO", gmemo.size(), MODE == 0 ? edfs.size() : fronts.size(), viaMemo.size());
    fflush(stdout);
    if (!w) { value = L; break; }
  }
  if (LFIX < 0) printf("VALUE P=%d T=%d MODE=%d OC=%d MAXJ=%d POL=%d : %d\n", P, T, MODE, OC, MAXJ, POL, value);
  if (PRINT == 2) { // dump strategy for threshold L (LFIX given, or value-1)
    L = (LFIX >= 0) ? LFIX : value - 1; gmemo.clear(); NL = 0;
    if (!advWins(0, pid0, empty)) { fprintf(stderr, "nothing to dump\n"); return 4; }
    dumpF = fopen(getenv("DUMP") ? getenv("DUMP") : "strategy.txt", "w");
    fprintf(dumpF, "header P %d T %d L %d mode %d\n", P, T, L, MODE);
    dumpTree(0, pid0, empty);
    fclose(dumpF);
    printf("dumped %d nodes\n", dumpN);
    return 0;
  }
  if (PRINT && value >= 1) {
    L = value - 1; gmemo.clear();
    advWins(0, pid0, empty);
    printTree(0, pid0, empty, 0);
  }
  return 0;
}
