// open-f-upper/opt4.cpp : how much can the 1-moves be worth?  Restricted position, PRESCIENT manager.
// Position P(M,cls): 1s on cells of [0,M) with cell%3 != cls ("11.11."), n2 = (M-#1s)/2 2s packed on [M, M+2*n2).
//   (This is where first fit stands after: M x alloc 1, free one residue class, alloc 2 while legal.)
// Adversary script from P: free every second 2 (parity par) -> k frees = k manager moves of a 1;
//   then free 1s (greedy minimum set kept so that no new free 3-window appears); then alloc 3 while legal.
// Manager: ANY net relocation of at most k 1s into cells that are free after the k frees (zone holes, the freed
//   2-holes, k cells above the top).  This is a superset of what k sequential moves can do (it even knows which
//   2s will be freed), so   min over relocations of the final extent   is a valid LOWER bound on the cells
//   needed by any manager that reaches P(M,cls).
// usage: opt4 exact M cls par      (enumerates all relocations of <= k ones)
//        opt4 sa M cls par iters seed   (simulated annealing over relocations: an upper bound on that minimum)
#include <bits/stdc++.h>
using namespace std;
static int M, cls, par, n1, n2, k, E0; static vector<int> base; // 0 free,1 one,2 cell of a kept 2
static vector<int> srcs, dsts;
static inline int evalLay(const vector<int>& o, int* wasteOut = nullptr) {
    int n = o.size(); int e = n; while (e > 0 && !o[e - 1]) e--;
    static thread_local vector<char> keep; keep.assign(n, 0); int h = 0;
    for (int r = 2; r < e; r++) { if (o[r] == 2 || o[r - 1] == 2 || o[r - 2] == 2) continue; if (!(o[r] | o[r - 1] | o[r - 2])) continue; if (keep[r] | keep[r - 1] | keep[r - 2]) continue;
        int j = r; while (o[j] != 1) j--; keep[j] = 1; h++; }
    int live = h, e2 = 0, slots = 0, run = 0, waste = 0;
    for (int i = 0; i < e; i++) { if (o[i] == 2) live++; if (o[i] == 2 || keep[i]) e2 = i + 1; }
    for (int i = 0; i < e2; i++) { if (o[i] == 2 || keep[i]) { slots += run / 3; waste += run % 3; run = 0; } else run++; }
    int n3 = (M - live) / 3; int fin = e2 + 3 * max(0, n3 - slots); if (wasteOut) *wasteOut = waste;
    return max(fin, E0); }
static long cnt; static int best; static vector<int> bestLay;
static void recD(vector<int>& o, int need, int from) { if (need == 0) { cnt++; int v = evalLay(o); if (v < best) { best = v; bestLay = o; } return; }
    for (int i = from; i + need <= (int)dsts.size(); i++) { o[dsts[i]] = 1; recD(o, need - 1, i + 1); o[dsts[i]] = 0; } }
static void recS(vector<int>& o, int j, int need, int from) { if (need == 0) { recD(o, j, 0); return; }
    for (int i = from; i + need <= (int)srcs.size(); i++) { o[srcs[i]] = 0; recS(o, j, need - 1, i + 1); o[srcs[i]] = 1; } }
static string show(const vector<int>& o) { string s; int e = o.size(); while (e > 0 && !o[e - 1]) e--; for (int i = 0; i < e; i++) s += ".12"[o[i]]; return s; }
int main(int argc, char** argv) { string mode = argv[1]; M = atoi(argv[2]); cls = atoi(argv[3]); par = atoi(argv[4]);
    base.assign(M, 0); for (int i = 0; i < M; i++) if (i % 3 != cls) { base[i] = 1; n1++; }
    n2 = (M - n1) / 2; k = 0; for (int i = 0; i < n2; i++) { bool freed = (i % 2 == par); base.push_back(freed ? 0 : 2); base.push_back(freed ? 0 : 2); if (freed) k++; }
    E0 = M + 2 * n2; if (!getenv("NOTOP")) for (int i = 0; i < k; i++) base.push_back(0); // NOTOP=1: no destinations above the top (a 1 put there is simply freed by a sensible adversary)
    for (int i = 0; i < (int)base.size(); i++) { if (base[i] == 1) srcs.push_back(i); if (base[i] == 0) dsts.push_back(i); }
    printf("M=%d cls=%d par=%d: #1s=%d #2s=%d freed(k)=%d  sources=%zu dests=%zu  start=%s\n", M, cls, par, n1, n2, k, srcs.size(), dsts.size(), show(base).c_str());
    vector<int> o = base; printf(" no moves: final extent %d\n", evalLay(o));
    { vector<int> c = base; for (int t = 0; t < k; t++) { int hi = -1; for (int i = 0; i < M; i++) if (c[i] == 1) hi = i; for (int i = 0; i < hi; i++) if (!c[i]) { c[hi] = 0; c[i] = 1; break; } } printf(" compaction: final extent %d  %s\n", evalLay(c), show(c).c_str()); }
    if (mode == "exact") { best = 1 << 30; for (int j = 0; j <= k; j++) recS(o, j, j, 0);
        printf(" EXACT min over %ld relocations: final extent %d   (5M/3=%.2f, 7M/4=%.2f)  argmin=%s\n", cnt, best, 5.0 * M / 3, 7.0 * M / 4, show(bestLay).c_str()); }
    else { long iters = atol(argv[5]); mt19937_64 rng(atoi(argv[6])); int gbest = 1 << 30; vector<int> gl;
        for (int restart = 0; restart < 8; restart++) {
            vector<int> S, D; vector<int> c = base; // start: restart 0 = compaction, others random
            if (restart == 0) { for (int t = 0; t < k; t++) { int hi = -1; for (int i = 0; i < M; i++) if (c[i] == 1) hi = i; for (int i = 0; i < hi; i++) if (!c[i]) { c[hi] = 0; c[i] = 1; break; } } }
            else for (int t = 0; t < k; t++) { int a, b; do a = srcs[rng() % srcs.size()]; while (c[a] != 1); do b = dsts[rng() % dsts.size()]; while (c[b] != 0); c[a] = 0; c[b] = 1; }
            auto cost = [&](const vector<int>& x) { int w; int v = evalLay(x, &w); return (double)v + 0.01 * w; };
            double cc = cost(c); double T = 2.0;
            for (long it = 0; it < iters; it++, T = max(0.02, T * (1 - 5.0 / iters))) {
                // neighbour: pick a 1 that is "moved or original" and a free cell, swap, but keep #relocated <= k
                int a = rng() % c.size(), b = rng() % c.size(); if (c[a] != 1 || c[b] != 0) continue; if (base[b] == 2) continue;
                c[a] = 0; c[b] = 1; int rel = 0; for (size_t i = 0; i < c.size(); i++) if (base[i] == 1 && c[i] != 1) rel++;
                double nc = rel <= k ? cost(c) : 1e9;
                if (nc <= cc || exp((cc - nc) / T) > (rng() % 1000000) / 1e6) { cc = nc; int v = evalLay(c); if (v < gbest) { gbest = v; gl = c; } } else { c[a] = 1; c[b] = 0; } } }
        printf(" SA best found: final extent %d   (5M/3=%.2f, 7M/4=%.2f)  lay=%s\n", gbest, 5.0 * M / 3, 7.0 * M / 4, show(gl).c_str()); }
    return 0; }
