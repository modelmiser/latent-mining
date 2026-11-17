// Referee's independent program for the game W(k,m).  Written from the problem statement.
// Pages are stored relative to the current request: page x <-> bit 32 + (x - pos).
// Subcommands:
//   game k m mode [strat]     exact F(k,m) (strat=0) or worst case of a fixed Online strategy
//   adva k m mode [variant]   solver A's adversary (from the text of Theorem L) vs every Online
//   advb k m mode [variant]   solver B's adversary (from the text of its lower bound) vs every Online
//   walk                      stdin lines: k m claimedFaults claimedOpt x1 x2 ...   (claimedOpt -1 = not given)
//   lemma k L                 all walks of length <= L: Lemma 2, Lemma 3, per-walk Theorem U
// mode 0: no pruning at all (all sub-caches offline, all legal Online moves)
// mode 1: only superset pruning (drop (C,f) if some (C',f') has C subset C', f'<=f; inclusion-maximal moves)
// mode 2: solvers' domination pruning for offline (f'+|C\C'|<=f); Online inclusion-maximal moves
#include <bits/stdc++.h>
using namespace std;
typedef uint64_t u64;
static int K, M, MODE;
static const u64 Bc = 1ULL << 32;
typedef vector<pair<u64, uint8_t>> WF;
static inline int pc(u64 x) { return __builtin_popcountll(x); }
static inline u64 sh(u64 x, int d) { return d == 1 ? x >> 1 : (d == -1 ? x << 1 : x); }

// successors of a fault at the current page from cache C (Bc not in C). np: no prefetch allowed
static void gen_moves(u64 C, int mode, vector<u64>& out, bool np = false) {
  for (int t = 0; t < (np ? 1 : 2); t++) {
    u64 q = np ? 0 : (t ? Bc << 1 : Bc >> 1);
    u64 U = C | Bc | q, rest = U & ~Bc;
    if (mode == 0) {
      for (u64 s = rest;; s = (s - 1) & rest) {
        if (pc(s) <= K - 1) out.push_back(s | Bc);
        if (!s) break;
      }
    } else {
      int ex = pc(U) - K;
      if (ex <= 0) out.push_back(U);
      else if (ex == 1) { for (u64 r = rest; r; r &= r - 1) out.push_back(U & ~(r & -r)); }
      else { for (u64 r = rest; r; r &= r - 1) { u64 b1 = r & -r; for (u64 r2 = r & (r - 1); r2; r2 &= r2 - 1) out.push_back(U & ~b1 & ~(r2 & -r2)); } }
    }
  }
}
static bool wf_step(const WF& w, int d, WF& out, int mode, bool np = false) {
  vector<pair<u64, uint8_t>> tmp; vector<u64> mv;
  for (auto& e : w) {
    u64 C = sh(e.first, d);
    if (C & Bc) tmp.push_back({C, e.second});
    else if (e.second < M) { mv.clear(); gen_moves(C, mode, mv, np); for (u64 c : mv) tmp.push_back({c, (uint8_t)(e.second + 1)}); }
  }
  sort(tmp.begin(), tmp.end());
  out.clear();
  for (auto& e : tmp) if (out.empty() || out.back().first != e.first) out.push_back(e);
  if (mode >= 1 && out.size() > 1) {
    WF keep; size_t n = out.size();
    for (size_t i = 0; i < n; i++) {
      bool dom = false;
      for (size_t j = 0; j < n && !dom; j++) {
        if (i == j) continue;
        if (mode == 1) { if ((out[i].first & ~out[j].first) == 0 && out[j].second <= out[i].second) dom = true; }
        else { if (out[j].second + pc(out[i].first & ~out[j].first) <= out[i].second) {
                 // tie-break so two mutually dominating entries do not both vanish
                 bool mutual = out[i].second + pc(out[j].first & ~out[i].first) <= out[j].second;
                 if (!mutual || j < i) dom = true; } }
      }
      if (!dom) keep.push_back(out[i]);
    }
    out.swap(keep);
  }
  return !out.empty();
}
static int wf_min(const WF& w) { int b = 99; for (auto& e : w) b = min(b, (int)e.second); return b; }

// ---------- interning ----------
static vector<WF> wfs; static unordered_map<string, int> wfidx; static vector<array<int, 2>> wfnext;
static int intern(const WF& w) {
  string key; key.resize(w.size() * 9);
  for (size_t i = 0; i < w.size(); i++) { memcpy(&key[i * 9], &w[i].first, 8); key[i * 9 + 8] = (char)w[i].second; }
  auto it = wfidx.find(key); if (it != wfidx.end()) return it->second;
  int id = wfs.size(); wfidx.emplace(move(key), id); wfs.push_back(w); wfnext.push_back({-2, -2}); return id;
}
static int wfgo(int id, int d) { // d=+1/-1; -1 if illegal
  int& r = wfnext[id][d == 1]; if (r != -2) return r;
  WF o; bool ok = wf_step(wfs[id], d, o, MODE); int v = ok ? intern(o) : -1; wfnext[id][d == 1] = v; return v;
}
static int wf_start() { WF w0 = {{0, 0}}, o; wf_step(w0, 0, o, MODE); return intern(o); }

// ---------- fixed Online strategies (cache as set; C lacks Bc) ----------
static u64 strat(u64 C, int id) {
  u64 l = Bc >> 1, r = Bc << 1, q = 0;
  bool pre = (id == 2) || (id != 3 && pc(C) <= K - 2);
  if (pre) { if (!(C & r)) q = r; else if (!(C & l)) q = l; }
  u64 N = C | Bc | q;
  while (pc(N) > K) {
    u64 best = 0; int bd = (id == 4) ? 1000 : -1;
    for (u64 x = N & ~Bc; x; x &= x - 1) { u64 b = x & -x; int dist = abs(__builtin_ctzll(b) - 32);
      if (id == 4 ? dist < bd : dist >= bd) { bd = dist; best = b; } }
    N &= ~best;
  }
  return N;
}

// ---------- full game ----------
struct PH { size_t operator()(const pair<int, u64>& p) const { return (p.second * 0x9E3779B97F4A7C15ULL) ^ ((u64)p.first * 0xC2B2AE3D27D4EB4FULL); } };
static int game(int st) {
  unordered_map<pair<int, u64>, int, PH> idx; vector<pair<int, u64>> states;
  vector<int> succ; vector<array<uint32_t, 2>> sbeg; vector<array<uint8_t, 2>> kind; vector<array<uint32_t, 2>> slen;
  auto get = [&](int w, u64 C) { auto k = make_pair(w, C); auto it = idx.find(k); if (it != idx.end()) return it->second;
    int id = states.size(); idx.emplace(k, id); states.push_back(k); return id; };
  int w0 = wf_start(); vector<int> starts; vector<u64> mv;
  if (st) starts.push_back(get(w0, strat(0, st))); else { gen_moves(0, MODE ? 1 : 0, mv); sort(mv.begin(), mv.end()); mv.erase(unique(mv.begin(), mv.end()), mv.end()); for (u64 c : mv) starts.push_back(get(w0, c)); }
  for (size_t i = 0; i < states.size(); i++) {
    auto s = states[i]; array<uint32_t, 2> b, l; array<uint8_t, 2> kd;
    for (int t = 0; t < 2; t++) {
      int d = t ? 1 : -1; int w2 = wfgo(s.first, d); b[t] = succ.size(); l[t] = 0; kd[t] = 0;
      if (w2 < 0) continue;
      u64 C = sh(s.second, d);
      if (C & Bc) { kd[t] = 1; succ.push_back(get(w2, C)); l[t] = 1; }
      else { kd[t] = 2;
        if (st) { succ.push_back(get(w2, strat(C, st))); l[t] = 1; }
        else { mv.clear(); gen_moves(C, MODE ? 1 : 0, mv); sort(mv.begin(), mv.end()); mv.erase(unique(mv.begin(), mv.end()), mv.end());
          for (u64 c : mv) succ.push_back(get(w2, c)); l[t] = mv.size(); } }
    }
    sbeg.push_back(b); slen.push_back(l); kind.push_back(kd);
    if (succ.size() > 1500000000u) { fprintf(stderr, "too big\n"); exit(3); }
  }
  size_t n = states.size(); vector<uint8_t> V(n, 0); int CAP = 4 * M + 6; bool ch = true; int it = 0;
  while (ch) { ch = false; it++;
    for (size_t ii = n; ii-- > 0;) { int v = 0;   // v=0: the Adversary may stop
      for (int t = 0; t < 2; t++) if (kind[ii][t]) { int mn = 255; for (uint32_t j = 0; j < slen[ii][t]; j++) mn = min(mn, (int)V[succ[sbeg[ii][t] + j]]);
        int x = mn + (kind[ii][t] == 2); if (x > v) v = x; }
      if (v > CAP) v = CAP;
      if (v > V[ii]) { V[ii] = v; ch = true; } } }
  int F = 255; for (int s : starts) F = min(F, (int)V[s]); F += 1;
  printf("game k=%d m=%d mode=%d strat=%d value=%d%s states=%zu wfs=%zu iters=%d formula=%d %s\n", K, M, MODE, st, F, F >= CAP ? "(UNBOUNDED)" : "", n, wfs.size(), it,
         max(M + 1, 2 * M + 1 - K / 2), F == max(M + 1, 2 * M + 1 - K / 2) ? "EQ" : "DIFF");
  return F;
}

// ---------- adversaries ----------
struct AS { int8_t pos = 0, a = 0, b = 0, phase = 0, d = 0, nb = 0, fresh = 0, fre = 0, mode = 0, e = 0, tgt = 0, cyc = 0; };
static const int STOP = 100;
static u64 Ccur; static int WFcur; static long badinv = 0, illegal = 0;
static inline bool inC(const AS& s, int x) { int bit = 32 + x - s.pos; return (Ccur >> bit) & 1; }
static bool offline_has(const AS& s, int lo, int hi, int cost) { // some offline entry holds [lo,hi] with <= cost faults
  u64 mk = 0; for (int x = lo; x <= hi; x++) mk |= 1ULL << (32 + x - s.pos);
  for (auto& e : wfs[WFcur]) if ((mk & ~e.first) == 0 && e.second <= cost) return true;
  return false;
}
static int nearest_missing(const AS& s, int lo, int hi) { int best = STOP, bd = 1000; for (int x = lo; x <= hi; x++) if (!inC(s, x) && abs(x - s.pos) < bd) { bd = abs(x - s.pos); best = x; } return best; }
static int toward(const AS& s, int x) { return s.pos + (x > s.pos ? 1 : -1); }
// Solver A, Theorem L.  variant 1: prefer cached frontier; 2: never bounce in stage 2; 3: one block too many
static int advA(AS& s, int var) {
  int j = K / 2, v = s.b - s.a + 1, lo = s.a, hi = s.b;
  if (s.phase == 1) { if (s.d == -1) hi = min((int)s.b, s.a + K - 1); else lo = max((int)s.a, s.b - K + 1); }
  int x = nearest_missing(s, lo, hi);
  if (x != STOP && !(var == 2 && s.phase == 1)) { s.fresh = 0; return toward(s, x); }
  if (s.phase == 0) {
    if (v == 2 * M) { if (!offline_has(s, s.a, s.b, M)) badinv++; return STOP; }
    bool la = inC(s, s.a - 1), rb = inC(s, s.b + 1); int side = !la ? -1 : (!rb ? 1 : -1);
    if (var == 1) side = la ? -1 : (rb ? 1 : -1);
    if (v == K - 1) { s.phase = 1; s.d = side; s.nb = j; s.fre = (K % 2 == 0); s.fresh = 0; }
    else { int end = side == -1 ? s.a : s.b; if (s.pos != end) return toward(s, end); return s.pos + side; }
  }
  int end = s.d == -1 ? s.a : s.b;
  if (!s.fresh && !s.fre) { // between blocks: offline claims cache O with nb faults
    if (var != 2 && !offline_has(s, lo, hi, s.nb)) badinv++;
    if (s.nb == M + (var == 3)) return STOP;
  }
  if (s.pos != end) return toward(s, end);
  if (s.fre) { s.fre = 0; return s.pos + s.d; }
  if (s.fresh) { s.fresh = 0; return s.pos + s.d; }
  s.nb++; s.fresh = 1; return s.pos + s.d;
}
// Solver B.  modes: 0 before stage 0, 1 stage-0 walk, 2 idle, 3 walking to the edge and out, 4 mid-cycle, 5 walking back to x
// variant 1: one cycle too few; 2: one cycle too many; 3: leaves by end b without looking at Online's cache
static int advB(AS& s, int var) {
  int h = K / 2, g = min(M, h);
  if (s.mode == 0) { int sd = inC(s, -1) ? 1 : -1; s.tgt = sd * (2 * g - 1); s.mode = 1; }
  if (s.mode == 1) { if (s.pos != s.tgt) return toward(s, s.tgt); s.a = min(0, (int)s.tgt); s.b = max(0, (int)s.tgt); s.mode = 2; }
  if (s.mode == 5) { if (s.pos != s.tgt) return toward(s, s.tgt); s.mode = 2; }
  if (s.mode == 4) {
    int e = s.e, end = e == 1 ? s.b : s.a, o1 = end + e, o2 = end + 2 * e; bool full = (s.b - s.a + 1 == K);
    if (!inC(s, o2)) { if (e == 1) { s.b = o2; s.a += full ? 2 : 1; } else { s.a = o2; s.b -= full ? 2 : 1; } s.cyc++; s.mode = 2; return o2; }
    int x = STOP; // absent page of old O nearest to o1
    if (e == 1) { for (int y = s.b; y >= s.a; y--) if (!inC(s, y)) { x = y; break; } } else { for (int y = s.a; y <= s.b; y++) if (!inC(s, y)) { x = y; break; } }
    if (x == STOP) { badinv++; return STOP; }
    if (e == 1) { s.b = o1; if (full) s.a++; } else { s.a = o1; if (full) s.b--; }
    if (x < s.a || x > s.b) { badinv++; return STOP; }
    s.cyc++; s.tgt = x; s.mode = 5; return toward(s, x);
  }
  if (s.mode == 2) {
    if (!offline_has(s, s.a, s.b, g + s.cyc)) badinv++;
    int lim = (M < h) ? 0 : M - h; if (var == 1) lim--; if (var == 2) lim++;
    if (s.cyc >= lim) return STOP;
    int x = nearest_missing(s, s.a, s.b); if (x != STOP) return toward(s, x);
    s.e = !inC(s, s.b + 1) ? 1 : -1; if (var == 3) s.e = 1;
    if (var != 3 && inC(s, s.e == 1 ? s.b + 1 : s.a - 1)) { badinv++; return STOP; }
    s.mode = 3;
  }
  // mode 3
  int end = s.e == 1 ? s.b : s.a;
  if (s.pos != end) return toward(s, end);
  s.mode = 4; return end + s.e;
}
struct Node { AS s; int w; u64 C; };
static string nkey(const Node& n) { string k; k.resize(sizeof(AS) + 12); memcpy(&k[0], &n.s, sizeof(AS)); memcpy(&k[sizeof(AS)], &n.w, 4); memcpy(&k[sizeof(AS) + 4], &n.C, 8); return k; }
static int advrun(char which, int var) {
  unordered_map<string, int> idx; vector<Node> nodes; vector<int> dist; vector<int> hitsucc; vector<char> term;
  deque<int> dq;
  auto get = [&](const Node& n, int d, bool front) { string k = nkey(n); auto it = idx.find(k); int id;
    if (it == idx.end()) { id = nodes.size(); idx.emplace(move(k), id); nodes.push_back(n); dist.push_back(1 << 30); hitsucc.push_back(-1); term.push_back(0); } else id = it->second;
    if (d < dist[id]) { dist[id] = d; if (front) dq.push_front(id); else dq.push_back(id); }
    return id; };
  int w0 = wf_start(); vector<u64> mv; gen_moves(0, MODE ? 1 : 0, mv); sort(mv.begin(), mv.end()); mv.erase(unique(mv.begin(), mv.end()), mv.end());
  for (u64 c : mv) { Node n; n.w = w0; n.C = c; get(n, 1, false); }
  vector<char> done; long expanded = 0;
  while (!dq.empty()) {
    int id = dq.front(); dq.pop_front(); if ((int)done.size() < (int)nodes.size()) done.resize(nodes.size(), 0);
    if (done[id]) continue; done[id] = 1; expanded++;   // 0-1 BFS: first pop has the final distance
    Node n = nodes[id]; int dcur = dist[id]; Ccur = n.C; WFcur = n.w;
    AS s2 = n.s; int p = which == 'a' ? advA(s2, var) : advB(s2, var);
    if (p == STOP) { term[id] = 1; continue; }
    int d = p - n.s.pos; if (d != 1 && d != -1) { printf("BAD STEP\n"); exit(2); }
    int w2 = wfgo(n.w, d); if (w2 < 0) { illegal++; term[id] = 2; continue; }
    s2.pos = p; if (which == 'a') { s2.a = min((int)s2.a, p); s2.b = max((int)s2.b, p); } u64 C = sh(n.C, d); Node m; m.s = s2; m.w = w2;
    if (C & Bc) { m.C = C; hitsucc[id] = get(m, dcur, true); }
    else { mv.clear(); gen_moves(C, MODE ? 1 : 0, mv); sort(mv.begin(), mv.end()); mv.erase(unique(mv.begin(), mv.end()), mv.end());
      for (u64 c : mv) { m.C = c; get(m, dcur + 1, false); } }
    if (nodes.size() > 60000000) { printf("too big\n"); exit(3); }
  }
  int n = nodes.size(), best = 1 << 30; long cyc = 0;
  for (int i = 0; i < n; i++) if (term[i] == 1) best = min(best, dist[i]);
  // zero-fault infinite play = a cycle of hit edges (each node has at most one hit successor)
  vector<char> col(n, 0);
  for (int i = 0; i < n; i++) { if (col[i]) continue; vector<int> path; int x = i;
    while (x >= 0 && col[x] == 0) { col[x] = 1; path.push_back(x); x = hitsucc[x]; }
    if (x >= 0 && col[x] == 1) { cyc++; best = min(best, dist[x]); }
    for (int y : path) col[y] = 2; }
  int f = max(M + 1, 2 * M + 1 - K / 2);
  printf("adv%c k=%d m=%d mode=%d variant=%d min_online_faults=%d formula=%d nodes=%d illegal_requests=%ld offline_invariant_failures=%ld zero_fault_cycles=%ld %s\n",
         which, K, M, MODE, var, best == (1 << 30) ? -1 : best, f, n, illegal, badinv, cyc, (best >= f && !illegal && !badinv) ? "ACCEPT" : "REJECT");
  return 0;
}

// ---------- walk witness checker ----------
static int walkcheck() {
  string line; int bad = 0, nline = 0;
  while (getline(cin, line)) { if (line.empty()) continue; istringstream is(line); int k, m, cf, co; is >> k >> m >> cf >> co; vector<int> w; int x; while (is >> x) w.push_back(x);
    nline++; K = k; M = w.size() + 1; string why;
    if (w.empty() || w[0] != 0) why += " start!=0";
    for (size_t i = 1; i < w.size(); i++) if (abs(w[i] - w[i - 1]) != 1) why += " not-a-walk";
    int opt[2] = {-1, -1}; int faults = 0;
    if (why.empty()) {
      for (int mode = 0; mode < 2; mode++) { WF cur = {{0, 0}}, nx; wf_step(cur, 0, nx, mode); cur = nx; for (size_t i = 1; i < w.size(); i++) { wf_step(cur, w[i] - w[i - 1], nx, mode); cur = nx; } opt[mode] = wf_min(cur); }
      u64 C = strat(0, 1); faults = 1; for (size_t i = 1; i < w.size(); i++) { C = sh(C, w[i] - w[i - 1]); if (!(C & Bc)) { C = strat(C, 1); faults++; } }
      if (opt[0] != opt[1]) why += " opt-modes-differ";
      if (opt[0] > m) why += " opt>m(illegal walk)";
      if (co >= 0 && co != opt[0]) why += " claimed-opt-wrong";
      if (cf != faults) why += " claimed-faults-wrong";
      if (faults < max(m + 1, 2 * m + 1 - k / 2)) why += " faults-below-formula";
    }
    printf("k=%d m=%d len=%zu opt=%d faults(FILL-FAR)=%d : %s\n", k, m, w.size(), opt[0], faults, why.empty() ? "ACCEPT" : ("REJECT" + why).c_str());
    if (!why.empty()) bad++; }
  printf("lines=%d rejected=%d\n", nline, bad); return 0;
}

// ---------- lemma checks over all walks of length <= L ----------
static long nwalk = 0, f2 = 0, f3 = 0, fU = 0, f1 = 0; static int L;
static void rec(int len, const WF& off, const WF& np, u64 C, int t, int vmin, int vmax, int pos) {
  nwalk++; int f = wf_min(off), g = wf_min(np), j = K / 2, v = vmax - vmin + 1;
  if (g > 2 * f) f2++;                       // Lemma 2
  if (f < (v + 1) / 2) f1++;                 // Lemma 1
  if (t > max(f + 1, 2 * f - j + 1)) fU++;   // Theorem U, per-walk form
  if (len == L) return;
  for (int d = -1; d <= 1; d += 2) { WF o2, n2; wf_step(off, d, o2, 1); wf_step(np, d, n2, 1, true); u64 C2 = sh(C, d); int t2 = t; if (!(C2 & Bc)) { C2 = strat(C2, 1); t2++; }
    rec(len + 1, o2, n2, C2, t2, min(vmin, pos + d), max(vmax, pos + d), pos + d); }
}
// Lemma 3: from interval cache I (containing the current page), far-end eviction without prefetch is NP-optimal
static void rec3(int len, const WF& np, u64 C, int t) {
  nwalk++; if (t != wf_min(np)) f3++;
  if (len == L) return;
  for (int d = -1; d <= 1; d += 2) { WF n2; wf_step(np, d, n2, 1, true); u64 C2 = sh(C, d); int t2 = t; if (!(C2 & Bc)) { C2 = strat(C2, 3); t2++; } rec3(len + 1, n2, C2, t2); }
}
int main(int argc, char** argv) {
  string cmd = argv[1];
  if (cmd == "walk") return walkcheck();
  K = atoi(argv[2]);
  if (cmd == "lemma") { L = atoi(argv[3]); M = 90;
    WF w0 = {{0, 0}}, o, n; wf_step(w0, 0, o, 1); wf_step(w0, 0, n, 1, true); rec(1, o, n, strat(0, 1), 1, 0, 0, 0);
    printf("lemma k=%d L=%d walks=%ld  Lemma1_fail=%ld Lemma2_fail=%ld TheoremU_perwalk_fail=%ld\n", K, L, nwalk, f1, f2, fU);
    nwalk = 0; int ni = 0;
    for (int sz = 1; sz <= K; sz++) for (int left = 0; left < sz; left++) { u64 I = 0; for (int x = -left; x < sz - left; x++) I |= 1ULL << (32 + x); WF s = {{I, 0}}; rec3(1, s, I, 0); ni++; }
    printf("lemma3 k=%d L=%d intervals=%d (interval,walk) pairs=%ld  fail=%ld\n", K, L, ni, nwalk, f3);
    return 0; }
  M = atoi(argv[3]); MODE = atoi(argv[4]); int extra = argc > 5 ? atoi(argv[5]) : 0;
  if (cmd == "game") game(extra);
  else if (cmd == "adva") advrun('a', extra);
  else if (cmd == "advb") advrun('b', extra);
  return 0;
}
