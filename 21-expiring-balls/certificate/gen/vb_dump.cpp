// COPY of verifier/vb.cpp with one added statement: dump the final core (env DUMP=file), balls newest first, 'wo' hex, 0-based.
// Referee backward attractor. usage: vb n L [log2 table slots, default 22] [stop: 1 = stop when empty enters A]
// State: balls NEWEST first, byte i = w | o<<4, labels by first appearance scanning newest->oldest.
// With this labelling, "drop the oldest ball" = truncate the code, so
//   A = { p : some truncation of code(p) is in C }  (A is closed under adding an older ball, i.e. Expire leads into A).
// Rules generating A (non-terminal states from which Adversary forces load L):
//   seed: two bins of load L-1 in different components;
//   older: p in A  =>  p + (any older ball, still non-terminal forest) in A;
//   offer: q in A, newest ball b, and (q with b flipped) terminal or in A  =>  q - b in A.
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 U; typedef uint64_t u64;
int n, L; bool stopEarly = false;
const U EMPTYSLOT = ~(U)0;
U* tab; u64 mask, cnt = 0;
static inline u64 hsh(U x) { u64 a = (u64)x * 0x9e3779b97f4a7c15ULL; a ^= a >> 31; a += (u64)(x >> 64) * 0xd6e8feb86659fd93ULL; a ^= a >> 32; a *= 0xff51afd7ed558ccdULL; a ^= a >> 29; return a; }
static inline bool has(U x) { u64 i = hsh(x) & mask; while (tab[i] != EMPTYSLOT) { if (tab[i] == x) return true; i = (i + 1) & mask; } return false; }
static void ins(U x) { if (cnt * 10 > (mask + 1) * 7) { fprintf(stderr, "table full\n"); exit(3); } u64 i = hsh(x) & mask; while (tab[i] != EMPTYSLOT) i = (i + 1) & mask; tab[i] = x; cnt++; }
static inline U trunc_(U c, int j) { return j >= 16 ? c : (c & (((U)1 << (8 * j)) - 1)); }
static inline bool inA(U c, int k) { for (int j = k; j >= 0; j--) if (has(trunc_(c, j))) return true; return false; }
vector<U> work; u64 visited = 0, seeds = 0; bool found = false;
struct Fr { int k, m; unsigned char ld[12], cp[12]; U code; };
static inline U swap01(U c, int k) { U r = 0x10; for (int i = 1; i < k; i++) { int b = (int)(c >> (8 * i)) & 0xff; int w = b & 15, o = b >> 4; if (w < 2) w ^= 1; if (o < 2) o ^= 1; r |= (U)(w | (o << 4)) << (8 * i); } return r; }
static inline U dropNewest(U c, int k) { int mp[16]; for (int i = 0; i < 16; i++) mp[i] = -1; int nx = 0; U r = 0;
  for (int i = 1; i < k; i++) { int b = (int)(c >> (8 * i)) & 0xff; int w = b & 15, o = b >> 4; if (mp[w] < 0) mp[w] = nx++; if (mp[o] < 0) mp[o] = nx++; r |= (U)(mp[w] | (mp[o] << 4)) << (8 * (i - 1)); } return r; }
static Fr decode(U c) { Fr f; f.k = 0; f.m = 0; f.code = c; for (int i = 0; i < 12; i++) { f.ld[i] = 0; f.cp[i] = i; }
  while ((c >> (8 * f.k)) & 0xff) { int b = (int)(c >> (8 * f.k)) & 0xff; int w = b & 15, o = b >> 4; f.m = max(f.m, max(w, o) + 1); f.ld[w]++;
    int a = f.cp[w], d = f.cp[o]; for (int t = 0; t < 12; t++) if (f.cp[t] == d) f.cp[t] = a; f.k++; } return f; }
// enumerate children (one older ball), non-terminal only
template <class F> static inline void children(const Fr& f, F fn) {
  for (int w = 0; w <= f.m && w < n; w++) { if (w < f.m && f.ld[w] + 1 >= L) continue;
    for (int o = 0; o <= f.m + 1 && o < n; o++) { if (o == w) continue;
      if (w < f.m) { if (o < f.m) { if (f.cp[w] == f.cp[o]) continue; } else if (o != f.m) continue; }
      else { /* w fresh = label m */ if (o > f.m + 1) continue; if (o == f.m) continue; /* o used or o = m+1 */ }
      Fr g = f; g.k = f.k + 1; g.m = max(f.m, max(w, o) + 1); g.ld[w]++; int a = g.cp[w], d = g.cp[o]; for (int t = 0; t < 12; t++) if (g.cp[t] == d) g.cp[t] = a;
      g.code = f.code | ((U)(w | (o << 4)) << (8 * f.k)); fn(g); } } }
static void addCore(U c) { ins(c); work.push_back(c); if (c == 0) found = true; }
static void visit(const Fr& f) { visited++;
  if (f.k > 0) { bool cond = (f.ld[1] + 1 >= L); if (!cond) cond = inA(swap01(f.code, f.k), f.k);
    if (cond) { U p = dropNewest(f.code, f.k); if (!inA(p, f.k - 1)) addCore(p); } }
  children(f, [&](const Fr& g) { if (!has(g.code)) visit(g); }); }
static void seedgen(const Fr& f) {
  // seed?
  for (int a = 0; a < f.m; a++) if (f.ld[a] == L - 1) for (int b = a + 1; b < f.m; b++) if (f.ld[b] == L - 1 && f.cp[a] != f.cp[b]) { addCore(f.code); seeds++; return; }
  // prune: future seed needs bins x,y in different components now (or fresh), and at most n-k-2 more balls can be added keeping them apart
  int R = n - 2 - f.k, need = 2 * (L - 1); int fresh = n - f.m;
  if (fresh >= 2) need = min(need, 2 * (L - 1));
  for (int a = 0; a < f.m; a++) { if (fresh >= 1) need = min(need, 2 * (L - 1) - f.ld[a]); for (int b = a + 1; b < f.m; b++) if (f.cp[a] != f.cp[b]) need = min(need, 2 * (L - 1) - f.ld[a] - f.ld[b]); }
  if (need > R) return;
  children(f, [&](const Fr& g) { seedgen(g); }); }
int main(int argc, char** argv) { n = atoi(argv[1]); L = atoi(argv[2]); int lg = argc > 3 ? atoi(argv[3]) : 22; stopEarly = argc > 4 && atoi(argv[4]);
  mask = ((u64)1 << lg) - 1; tab = (U*)malloc(sizeof(U) << lg); for (u64 i = 0; i <= mask; i++) tab[i] = EMPTYSLOT;
  if (L == 1) { printf("n=%d L=1 trivial\n", n); return 0; }
  seedgen(decode(0)); fprintf(stderr, "seeds=%llu t=%.1f\n", (unsigned long long)seeds, clock() / 1e6);
  while (!work.empty() && !(stopEarly && found)) { U c = work.back(); work.pop_back(); visit(decode(c)); }
  if (getenv("DUMP")) { FILE* fo = fopen(getenv("DUMP"), "w"); for (u64 i = 0; i <= mask; i++) if (tab[i] != EMPTYSLOT) { U c = tab[i]; if (!c) { fprintf(fo, "-\n"); continue; } int j = 0; while ((c >> (8 * j)) & 0xff) { int b = (int)(c >> (8 * j)) & 0xff; fprintf(fo, "%s%x%x", j ? " " : "", b & 15, b >> 4); j++; } fprintf(fo, "\n"); } fclose(fo); }
  printf("n=%d L=%d minimal_seeds=%llu core=%llu attractor_nonterminal=%llu adversary_%s%s t=%.1f\n", n, L, (unsigned long long)seeds, (unsigned long long)cnt, (unsigned long long)visited, found ? "CAN" : "CANNOT", (stopEarly && found) ? "(stopped early)" : "", clock() / 1e6);
}
