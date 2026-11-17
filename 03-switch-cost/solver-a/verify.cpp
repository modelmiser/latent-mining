// Independent checker for an adversary strategy file written by game.cpp (PRINT=2).
// Shares NO code with game.cpp.  Jobs keep identities; promise feasibility is decided by a direct
// search over hindsight schedules from tick 0 on the full released job list (no frontiers, no dominance).
// It checks, for EVERY sequence of legal scheduler actions:
//   (1) each release is a legal job (r = current tick, 1<=p<=P, r+p<=d<=T);
//   (2) after each release the set of ALL jobs released so far is feasible for the hindsight machine
//       (switch cost 1 if header mode==1, switch cost 0 if mode==0);
//   (3) the play ends with some job that provably completes later than d+L (lateness >= L+1).
// usage: verify strategy.txt
#include <bits/stdc++.h>
using namespace std;
int P, T, L, MODE;
struct Node { int t; vector<pair<int,int>> rel; map<string,int> child; };
map<int, Node> nodes;
struct Job { int r, p, d; };
struct Mach { vector<int> rem; int cur = -1; int flag = 0; };
// ----- hindsight feasibility by brute force -----
vector<Job> HJ; int HCOST; unordered_set<string> hfail;
static bool hs(int t, vector<int> &rem, int cur, int flag) {
  bool all = true;
  for (size_t j = 0; j < HJ.size(); j++) if (rem[j]) { all = false; if (max(t, HJ[j].r) + rem[j] > HJ[j].d) return false; }
  if (all) return true;
  string k; k.push_back((char)t); k.push_back((char)(cur + 1)); k.push_back((char)flag);
  for (int x : rem) k.push_back((char)x);
  if (hfail.count(k)) return false;
  bool curUnf = (cur >= 0 && rem[cur] > 0);
  for (size_t j = 0; j < HJ.size(); j++) {
    if (!rem[j] || HJ[j].r > t) continue;
    if (HCOST == 1 && curUnf && (int)j != cur && !flag) continue;
    rem[j]--; bool ok = hs(t + 1, rem, (int)j, 0); rem[j]++;
    if (ok) return true;
  }
  if (hs(t + 1, rem, cur, 0)) return true;                 // idle
  if (HCOST == 1 && curUnf && !flag && hs(t + 1, rem, cur, 1)) return true; // switch tick
  if (HCOST == 1 && curUnf && flag && hs(t + 1, rem, cur, 1)) return true;  // switch tick again
  hfail.insert(k);
  return false;
}
map<vector<array<int,3>>, bool> feasCache;
static bool feasible(const vector<Job> &js) {
  vector<array<int,3>> key; for (auto &j : js) key.push_back({j.r, j.p, j.d});
  sort(key.begin(), key.end());
  auto it = feasCache.find(key); if (it != feasCache.end()) return it->second;
  HJ.clear(); for (auto &a : key) HJ.push_back({a[0], a[1], a[2]});
  hfail.clear(); HCOST = (MODE == 1) ? 1 : 0;
  vector<int> rem; for (auto &j : HJ) rem.push_back(j.p);
  bool r = hs(0, rem, -1, 0);
  feasCache[key] = r; return r;
}
// ----- online side -----
static bool doomed(const vector<Job> &js, const Mach &m, int t) {
  for (size_t j = 0; j < js.size(); j++) if (m.rem[j] && t + m.rem[j] > js[j].d + L) return true;
  return false;
}
static string canon(const vector<Job> &js, const Mach &m) {
  vector<int> codes; for (size_t j = 0; j < js.size(); j++) if (m.rem[j]) codes.push_back(js[j].d * 8 + m.rem[j]);
  sort(codes.begin(), codes.end());
  int A = 0, fl = 0;
  if (m.cur >= 0 && m.rem[m.cur] > 0) { A = js[m.cur].d * 8 + m.rem[m.cur]; fl = m.flag; }
  string s = to_string(A) + ":" + to_string(fl) + ":";
  for (size_t i = 0; i < codes.size(); i++) { if (i) s += "."; s += to_string(codes[i]); }
  return s;
}
static void moves(const vector<Job> &js, const Mach &m, vector<Mach> &out) {
  out.clear();
  bool curUnf = (m.cur >= 0 && m.rem[m.cur] > 0);
  for (size_t j = 0; j < js.size(); j++) {
    if (!m.rem[j]) continue;
    if (curUnf && (int)j != m.cur && !m.flag) continue;
    Mach n = m; n.rem[j]--; n.cur = (int)j; n.flag = 0; out.push_back(n);
  }
  { Mach n = m; n.flag = 0; out.push_back(n); }
  { Mach n = m; n.flag = 1; out.push_back(n); }
}
// after the last release: can the scheduler alone avoid lateness >= L+1 ?
map<string, bool> soloMemo;
static bool soloSurvive(const vector<Job> &js, const Mach &m, int t) {
  if (doomed(js, m, t)) return false;
  bool any = false; for (int x : m.rem) if (x) any = true;
  if (!any) return true;
  string k = to_string(t) + "|" + canon(js, m);
  auto it = soloMemo.find(k); if (it != soloMemo.end()) return it->second;
  vector<Mach> ms; moves(js, m, ms);
  bool r = false;
  for (auto &n : ms) if (soloSurvive(js, n, t + 1)) { r = true; break; }
  soloMemo[k] = r; return r;
}
long long visits = 0, plays = 0, feasChecks = 0; int maxJobs = 0;
set<string> seen;
static void fail(const string &msg) { printf("VERIFY FAILED: %s\n", msg.c_str()); exit(1); }
static void walk(int id, vector<Job> js, Mach m, int t) {
  // rigorous dedupe: same node, same full release history (as a sorted list), same exact online position
  { string k = to_string(id) + "#"; vector<array<int,3>> key; for (auto &j : js) key.push_back({j.r, j.p, j.d});
    // identities matter only through (r,p,d,rem) and which one is current
    vector<array<int,5>> full; for (size_t j = 0; j < js.size(); j++) full.push_back({js[j].r, js[j].p, js[j].d, m.rem[j], ((int)j == m.cur) ? 1 : 0});
    sort(full.begin(), full.end());
    for (auto &a : full) for (int x : a) { k += to_string(x); k += ","; }
    k += to_string(m.flag);
    if (seen.count(k)) return; seen.insert(k); }
  visits++;
  auto it = nodes.find(id); if (it == nodes.end()) fail("missing node");
  Node &nd = it->second;
  if (nd.t != t) fail("tick mismatch");
  for (auto &pr : nd.rel) {
    int p = pr.first, d = pr.second;
    if (p < 1 || p > P || t + p > d || d > T || t >= T) fail("illegal job");
    js.push_back({t, p, d}); m.rem.push_back(p);
  }
  maxJobs = max(maxJobs, (int)js.size());
  feasChecks++;
  if (!feasible(js)) fail("promise violated at node " + to_string(id));
  if (doomed(js, m, t)) { plays++; return; }
  vector<Mach> ms; moves(js, m, ms);
  for (auto &n : ms) {
    if (doomed(js, n, t + 1)) { plays++; continue; }
    if (t + 1 >= T) { if (soloSurvive(js, n, t + 1)) fail("scheduler survives after last release, node " + to_string(id)); plays++; continue; }
    string c = canon(js, n);
    auto ci = nd.child.find(c);
    if (ci == nd.child.end()) fail("strategy has no answer to a legal scheduler move at node " + to_string(id) + " state " + c);
    if (ci->second < 0) fail("strategy claims terminal but checker disagrees at node " + to_string(id) + " state " + c);
    walk(ci->second, js, n, t + 1);
  }
}
int main(int argc, char **argv) {
  ifstream f(argv[1]); string w;
  f >> w >> w >> P >> w >> T >> w >> L >> w >> MODE;
  if (argc > 2 && string(argv[2]) == "feas") { // cross-check of the solver's promise machinery against brute force
    long long n = 0, bad = 0, nfeas = 0;
    while (f >> w) {
      int ok, k; f >> ok >> k; vector<Job> js;
      for (int i = 0; i < k; i++) { Job j; f >> j.r >> j.p >> j.d; js.push_back(j); }
      bool b = feasible(js); n++; if (b) nfeas++;
      if (b != (ok == 1)) { bad++; if (bad < 5) { printf("MISMATCH solver=%d brute=%d :", ok, (int)b); for (auto &j : js) printf(" (%d,%d,%d)", j.r, j.p, j.d); printf("\n"); } }
    }
    printf("feasibility cross-check P=%d T=%d promise=%s: %lld job lists (%lld feasible), %lld mismatches\n", P, T, MODE == 1 ? "same-cost" : "free", n, nfeas, bad);
    return bad ? 1 : 0;
  }
  while (f >> w) {
    int id, t, k; f >> id >> t >> k; Node nd; nd.t = t;
    for (int i = 0; i < k; i++) { int p, d; f >> p >> d; nd.rel.push_back({p, d}); }
    int m; f >> m;
    for (int i = 0; i < m; i++) { string c; int cid; f >> c >> cid; nd.child[c] = cid; }
    nodes[id] = nd;
  }
  walk(0, {}, Mach(), 0);
  printf("VERIFIED: P=%d T=%d promise=%s : the strategy forces lateness >= %d against every scheduler.\n", P, T, MODE == 1 ? "same-cost" : "free", L + 1);
  printf("  strategy nodes %zu, positions checked %lld, terminal plays %lld, feasibility checks %lld (distinct job lists %zu), max jobs in a play %d\n",
         nodes.size(), visits, plays, feasChecks, feasCache.size(), maxJobs);
  return 0;
}
