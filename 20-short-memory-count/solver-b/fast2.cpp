// Generalised version of fast.cpp for W up to 12 (window sets as arrays of 64-bit words), pruned search only.
//   fast2 W N find S [all]   : machines (canonical BFS numbering, <=N reachable states) with g <= S
//   fast2 W N efind S [all]  : machines with max spread <= S
// Optional env PREFIX="t t t ..." fixes the first transitions (for splitting runs); NODELIMIT unused.
// Pruning (safe for existence): in a partial machine, C(q) computed from defined transitions is a subset of the final C(q),
// so spreads only grow and edges only get added; a cycle among states of spread > S persists in every completion.
// machine line format: w n t00 t01 t10 t11 ... g
#include <bits/stdc++.h>
#include <immintrin.h>
using namespace std;
const int MAXN = 14, MAXWD = 64;
int W, N, NW, HW; // NW words total (2^W bits), HW = words in half (if W>=7) 
struct BS { uint64_t w[MAXWD]; };
BS PC[14];
int tr[MAXN][2];
static inline bool anyb(const BS& a) { for (int i = 0; i < NW; i++) if (a.w[i]) return true; return false; }
static inline bool inter(const BS& a, const BS& b) { for (int i = 0; i < NW; i++) if (a.w[i] & b.w[i]) return true; return false; }
static inline void stepset(const BS& S, int b, BS& R) {
    if (W <= 6) {
        int half = 1 << (W - 1);
        uint64_t s = S.w[0]; uint64_t F = (s | (s >> half)) & ((1ULL << half) - 1);
        R.w[0] = _pdep_u64(F, 0x5555555555555555ULL) << b; // half<=32 bits
        return;
    }
    for (int i = 0; i < HW; i++) {
        uint64_t F = S.w[i] | S.w[i + HW];
        R.w[2 * i] = _pdep_u64(F & 0xffffffffULL, 0x5555555555555555ULL) << b;
        R.w[2 * i + 1] = _pdep_u64(F >> 32, 0x5555555555555555ULL) << b;
    }
}
int spr[MAXN];
BS L[MAXN], T[MAXN];
void compute(int m) {
    for (int q = 0; q < m; q++) memset(L[q].w, 0, 8 * NW);
    L[0].w[0] = 1;
    BS R;
    for (int k = 0; k < W; k++) {
        for (int q = 0; q < m; q++) memset(T[q].w, 0, 8 * NW);
        for (int q = 0; q < m; q++) if (anyb(L[q])) for (int b = 0; b < 2; b++) if (tr[q][b] >= 0) { stepset(L[q], b, R); uint64_t* t = T[tr[q][b]].w; for (int i = 0; i < NW; i++) t[i] |= R.w[i]; }
        for (int q = 0; q < m; q++) memcpy(L[q].w, T[q].w, 8 * NW);
    }
    bool ch = true;
    while (ch) {
        ch = false;
        for (int q = 0; q < m; q++) if (anyb(L[q])) for (int b = 0; b < 2; b++) if (tr[q][b] >= 0) {
            stepset(L[q], b, R); uint64_t* t = L[tr[q][b]].w;
            for (int i = 0; i < NW; i++) { uint64_t n = t[i] | R.w[i]; if (n != t[i]) { t[i] = n; ch = true; } }
        }
    }
    for (int q = 0; q < m; q++) {
        spr[q] = -1;
        if (!anyb(L[q])) continue;
        int lo = 0, hi = W;
        while (!inter(L[q], PC[lo])) lo++;
        while (!inter(L[q], PC[hi])) hi--;
        spr[q] = hi - lo;
    }
}
bool cyclic(int m, int s) {
    int indeg[MAXN] = {0}; bool in[MAXN];
    for (int q = 0; q < m; q++) in[q] = spr[q] > s;
    for (int q = 0; q < m; q++) if (in[q]) for (int b = 0; b < 2; b++) { int t = tr[q][b]; if (t >= 0 && in[t]) indeg[t]++; }
    int st[MAXN], sp = 0, removed = 0, tot = 0;
    for (int q = 0; q < m; q++) if (in[q]) { tot++; if (!indeg[q]) st[sp++] = q; }
    while (sp) { int q = st[--sp]; removed++; for (int b = 0; b < 2; b++) { int t = tr[q][b]; if (t >= 0 && in[t] && --indeg[t] == 0) st[sp++] = t; } }
    return removed != tot;
}
int mode, S; bool countall = false; int printmax = 20;
unsigned long long nodes = 0, found = 0; bool stop = false;
vector<int> prefix;
void printm(int m) {
    compute(m); int g = 0; while (cyclic(m, g)) g++;
    printf("%d %d", W, m);
    for (int q = 0; q < m; q++) printf(" %d %d", tr[q][0], tr[q][1]);
    printf(" %d\n", g); fflush(stdout);
}
void rec(int pos, int m) {
    if (stop) return;
    int q = pos >> 1, b = pos & 1;
    if (q == m) { found++; if (found == 1 || (countall && (long long)found <= printmax)) printm(m); if (!countall) stop = true; return; }
    int lim = min(m, N - 1);
    for (int t = 0; t <= lim; t++) {
        if (pos < (int)prefix.size() && prefix[pos] != t) continue;
        tr[q][b] = t;
        int m2 = max(m, t + 1);
        nodes++;
        compute(m2);
        bool ok = true;
        if (mode == 1) ok = !cyclic(m2, S);
        else for (int x = 0; x < m2; x++) if (spr[x] > S) ok = false;
        if (ok) rec(pos + 1, m2);
        tr[q][b] = -1;
        if (stop) return;
    }
}
int main(int argc, char** argv) {
    W = atoi(argv[1]); N = atoi(argv[2]); string md = argv[3]; S = atoi(argv[4]);
    if (argc > 5) countall = true;
    if (argc > 6) printmax = atoi(argv[6]);
    mode = md == "find" ? 1 : 2;
    NW = W <= 6 ? 1 : (1 << (W - 6)); HW = NW / 2;
    memset(PC, 0, sizeof PC);
    for (int u = 0; u < (1 << W); u++) PC[__builtin_popcount(u)].w[u >> 6] |= 1ULL << (u & 63);
    memset(tr, -1, sizeof tr);
    if (getenv("PREFIX")) { stringstream ss(getenv("PREFIX")); int x; while (ss >> x) prefix.push_back(x); }
    rec(0, 1);
    fprintf(stderr, "w=%d N=%d %s S=%d: %s (found=%llu, nodes=%llu, exhaustive=%s)\n", W, N, md.c_str(), S, found ? "EXISTS" : "NONE", found, nodes, (stop ? "no(stopped at first)" : "yes"));
}
