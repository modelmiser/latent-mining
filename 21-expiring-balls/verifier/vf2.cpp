// Referee forward solver. usage: vf n L
// States: balls NEWEST first; ball = (w,o); bins labelled by first appearance scanning newest->oldest.
// Enumerates every non-terminal state reachable from empty, then least fixed point of Adversary wins.
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 U;
int n, L, MODE = 0; // 0 normal, 1 no Expire, 2 Rule fixed to greedy(min load, tie -> earlier in age order)
set<unsigned __int128> terms; bool unsafe = false;
struct St { int k; int w[12], o[12]; };
static U canon(const St& s) { int mp[16]; for (int i = 0; i < 16; i++) mp[i] = -1; int nx = 0; U c = 0;
  for (int i = 0; i < s.k; i++) { if (mp[s.w[i]] < 0) mp[s.w[i]] = nx++; if (mp[s.o[i]] < 0) mp[s.o[i]] = nx++;
    c |= (U)(mp[s.w[i]] | (mp[s.o[i]] << 4)) << (8 * i); } return c; }
static St dec(U c) { St s; s.k = 0; while (c & 0xff) { s.w[s.k] = c & 15; s.o[s.k] = (c >> 4) & 15; s.k++; c >>= 8; } return s; }
struct H { size_t operator()(U x) const { uint64_t a = (uint64_t)x * 0x9e3779b97f4a7c15ULL ^ (uint64_t)(x >> 64) * 0xc2b2ae3d27d4eb4fULL; a ^= a >> 29; a *= 0xbf58476d1ce4e5b9ULL; a ^= a >> 32; return a; } };
unordered_map<U, int, H> id; vector<U> codes;
// successors: exp[i] (-1 none), offers as pairs of (id or -2 terminal)
vector<int> ex; vector<vector<pair<int,int>>> off;
static int getid(U c) { auto it = id.find(c); if (it != id.end()) return it->second; int i = codes.size(); id[c] = i; codes.push_back(c); return i; }
int main(int argc, char** argv) { n = atoi(argv[1]); L = atoi(argv[2]); if (argc > 3) MODE = atoi(argv[3]);
  getid(0);
  for (size_t i = 0; i < codes.size(); i++) { St s = dec(codes[i]);
    int m = 0, ld[16] = {0}, cp[16]; for (int j = 0; j < 16; j++) cp[j] = j;
    for (int j = 0; j < s.k; j++) { m = max(m, max(s.w[j], s.o[j]) + 1); ld[s.w[j]]++;
      int a = cp[s.w[j]], b = cp[s.o[j]]; for (int t = 0; t < 16; t++) if (cp[t] == b) cp[t] = a; }
    int e = -1; if (s.k && MODE != 1) { St t = s; t.k--; e = getid(canon(t)); }   // oldest = last entry
    vector<pair<int,int>> ofs;
    int top = min(n, m + 2);
    for (int u = 0; u < top; u++) for (int v = u + 1; v < top; v++) {
      if (u >= m && !(u == m && v == m + 1)) continue;      // both fresh: only (m,m+1)
      if (u < m && v >= m && v != m) continue;              // one fresh: only label m
      if (u < m && v < m && cp[u] == cp[v]) continue;
      int r[2];
      for (int z = 0; z < 2; z++) { int w = z ? v : u, o = z ? u : v;
        if (MODE == 2) { int fu = -1, fv = -1; for (int j = 0; j < s.k; j++) { if (s.w[j] == u || s.o[j] == u) fu = j; if (s.w[j] == v || s.o[j] == v) fv = j; }
          int g; if (ld[u] != ld[v]) g = ld[u] < ld[v] ? u : v; else g = (fu >= fv) ? u : v;   /* larger index = older; untouched (-1) last; both untouched symmetric */
          if (w != g) { r[z] = -3; continue; } if (ld[w] + 1 >= L) unsafe = true; }
        if (ld[w] + 1 >= L) { St t; t.k = s.k + 1; t.w[0] = w; t.o[0] = o; for (int j = 0; j < s.k; j++) { t.w[j + 1] = s.w[j]; t.o[j + 1] = s.o[j]; } terms.insert(canon(t)); r[z] = -2; continue; }
        St t; t.k = s.k + 1; t.w[0] = w; t.o[0] = o; for (int j = 0; j < s.k; j++) { t.w[j + 1] = s.w[j]; t.o[j + 1] = s.o[j]; }
        r[z] = getid(canon(t)); }
      ofs.push_back({r[0], r[1]}); }
    if (ex.size() <= i) { ex.resize(i + 1); off.resize(i + 1); }
    ex[i] = e; off[i] = ofs; }
  size_t N = codes.size(); if (MODE == 2) { printf("n=%d L=%d greedy rule: reachable=%zu %s\n", n, L, N, unsafe ? "UNSAFE" : "SAFE"); return 0; } vector<char> win(N, 0); bool ch = true; int sweeps = 0; size_t nw = 0;
  while (ch) { ch = false; sweeps++;
    for (size_t i = 0; i < N; i++) { if (win[i]) continue; bool ok = ex[i] >= 0 && win[ex[i]];
      if (!ok) for (auto& p : off[i]) { if ((p.first == -2 || win[p.first]) && (p.second == -2 || win[p.second])) { ok = true; break; } }
      if (ok) { win[i] = 1; nw++; ch = true; } } }
  printf("mode=%d terminal=%zu ", MODE, terms.size()); printf("n=%d L=%d states=%zu winning=%zu sweeps=%d adversary_%s\n", n, L, N, nw, sweeps, win[0] ? "CAN" : "CANNOT");
}
