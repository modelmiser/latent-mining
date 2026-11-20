// Canonical (BFS-numbered) enumeration of initially-connected binary DFAs with <= N states.
// modes:
//   fast W N hist [dump]   : no pruning; histogram of g over all canonical machines with <=N reachable states (all filled)
//                            also histogram of e-value (max spread). 'dump' prints every machine with g.
//   fast W N find S [all]  : pruned search for machines with g <= S. prints first witness (or count if 'all').
//   fast W N efind S       : pruned search for machines with max spread <= S.
// machine line format: w n t00 t01 t10 t11 ... g
#include <bits/stdc++.h>
#include <immintrin.h>
using namespace std;
typedef unsigned __int128 u128;
int W, N; int half; // half = 2^(W-1)
u128 PC[9]; // PC[c] = set of windows with popcount c
int tr[9][2]; // -1 undefined
static inline u128 stepset(u128 S, int b) {
    // windows u -> (2u+b) mod 2^W
    uint64_t F;
    if (W == 7) F = (uint64_t)S | (uint64_t)(S >> 64);
    else { uint64_t s = (uint64_t)S; F = (s | (s >> half)) & ((half==64)?~0ULL:((1ULL << half) - 1)); }
    uint64_t lo = _pdep_u64(F & 0xffffffffULL, 0x5555555555555555ULL);
    uint64_t hi = _pdep_u64(F >> 32, 0x5555555555555555ULL);
    u128 R = ((u128)hi << 64) | lo;
    return R << b;
}
int spr[9]; bool hasC[9];
// compute spreads for the (partial) machine on states 0..m-1
void compute(int m) {
    u128 L[9], T[9];
    for (int q = 0; q < m; q++) L[q] = 0;
    L[0] = 1; // empty string at level 0
    for (int k = 0; k < W; k++) {
        for (int q = 0; q < m; q++) T[q] = 0;
        for (int q = 0; q < m; q++) if (L[q]) for (int b = 0; b < 2; b++) if (tr[q][b] >= 0) T[tr[q][b]] |= stepset(L[q], b);
        for (int q = 0; q < m; q++) L[q] = T[q];
    }
    bool ch = true;
    while (ch) {
        ch = false;
        for (int q = 0; q < m; q++) if (L[q]) for (int b = 0; b < 2; b++) if (tr[q][b] >= 0) {
            int t = tr[q][b]; u128 n = L[t] | stepset(L[q], b);
            if (n != L[t]) { L[t] = n; ch = true; }
        }
    }
    for (int q = 0; q < m; q++) {
        hasC[q] = L[q] != 0; spr[q] = -1;
        if (!hasC[q]) continue;
        int lo = 0, hi = W;
        while (!(L[q] & PC[lo])) lo++;
        while (!(L[q] & PC[hi])) hi--;
        spr[q] = hi - lo;
    }
}
// is the graph induced on states with spr > s (defined edges only) cyclic?
bool cyclic(int m, int s) {
    int indeg[9] = {0}; bool in[9];
    for (int q = 0; q < m; q++) in[q] = spr[q] > s;
    for (int q = 0; q < m; q++) if (in[q]) for (int b = 0; b < 2; b++) { int t = tr[q][b]; if (t >= 0 && in[t]) indeg[t]++; }
    int st[9], sp = 0, removed = 0, tot = 0;
    for (int q = 0; q < m; q++) if (in[q]) { tot++; if (!indeg[q]) st[sp++] = q; }
    while (sp) { int q = st[--sp]; removed++; for (int b = 0; b < 2; b++) { int t = tr[q][b]; if (t >= 0 && in[t] && --indeg[t] == 0) st[sp++] = t; } }
    return removed != tot;
}
int mode; int S; bool dumpall = false, countall = false;
unsigned long long leaves = 0, nodes = 0, found = 0;
unsigned long long histg[10], histe[10];
void printm(int m, int g) {
    printf("%d %d", W, m);
    for (int q = 0; q < m; q++) printf(" %d %d", tr[q][0], tr[q][1]);
    printf(" %d\n", g);
}
void leaf(int m) {
    leaves++;
    if (mode == 0) {
        compute(m);
        int e = 0; for (int q = 0; q < m; q++) e = max(e, spr[q]);
        int g = 0; while (cyclic(m, g)) g++;
        histg[g]++; histe[e]++;
        if (dumpall) printm(m, g);
    } else {
        // pruning already established the property at this node
        found++;
        if (found == 1 || (countall && found <= 20)) { compute(m); int g = 0; while (cyclic(m, g)) g++; printm(m, g); fflush(stdout); }
    }
}
bool stop = false;
void rec(int pos, int m) { // pos = 2*q+b next transition to fill; m states discovered
    if (stop) return;
    int q = pos >> 1, b = pos & 1;
    if (q == m) { leaf(m); if (mode != 0 && !countall) stop = true; return; }
    int lim = min(m, N - 1);
    for (int t = 0; t <= lim; t++) {
        tr[q][b] = t;
        int m2 = max(m, t + 1);
        nodes++;
        bool ok = true;
        if (mode == 1) { compute(m2); ok = !cyclic(m2, S); }
        else if (mode == 2) { compute(m2); for (int x = 0; x < m2; x++) if (spr[x] > S) ok = false; }
        if (ok) rec(pos + 1, m2);
        tr[q][b] = -1;
        if (stop) return;
    }
}
int main(int argc, char** argv) {
    W = atoi(argv[1]); N = atoi(argv[2]); half = 1 << (W - 1);
    string md = argv[3];
    if (md == "hist") { mode = 0; if (argc > 4) dumpall = true; }
    else if (md == "find") { mode = 1; S = atoi(argv[4]); if (argc > 5) countall = true; }
    else if (md == "efind") { mode = 2; S = atoi(argv[4]); if (argc > 5) countall = true; }
    for (int c = 0; c <= W; c++) PC[c] = 0;
    for (int u = 0; u < (1 << W); u++) PC[__builtin_popcount(u)] |= (u128)1 << u;
    memset(tr, -1, sizeof tr);
    rec(0, 1);
    if (mode == 0) {
        fprintf(stderr, "w=%d N=%d canonical machines=%llu\n g-hist:", W, N, leaves);
        for (int i = 0; i <= W; i++) fprintf(stderr, " %d:%llu", i, histg[i]);
        fprintf(stderr, "\n e-hist:");
        for (int i = 0; i <= W; i++) fprintf(stderr, " %d:%llu", i, histe[i]);
        fprintf(stderr, "\n");
    } else {
        fprintf(stderr, "w=%d N=%d %s S=%d: %s (found=%llu, nodes=%llu, exhaustive=%s)\n", W, N, md.c_str(), S, found ? "EXISTS" : "NONE", found, nodes, (stop ? "no(stopped at first)" : "yes"));
    }
}
