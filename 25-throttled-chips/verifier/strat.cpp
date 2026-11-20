// Referee's test of solver A's Theorem 2 strategy (written from proofs.txt's description, not from strategy_sim.py).
// Lags are kept as integers scaled by 5:  L5_x = 3k_x + k_{x-1} + k_{x+1} - 5 F_x.
// Strategy: after the add, among READY vertices (eta>=2) with lag >= 1 (L5 >= 5) fire one with largest lag
// (tie-break TB: 0 leftmost, 1 rightmost, 2 fewest chips, 3 most chips); else pass.
// Checks at every step: (*) eta_x = k_x - 2F_x + F_{x-1} + F_{x+1} (by construction: we move chips and compare),
//   (**) 5*eta_x = (k_{x-2}+..+k_{x+2}) + 2 L5_x - L5_{x-1} - L5_{x+1},  lag >= 0,
//   records: max lag, max (5*eta - t), smallest max-lag M at which NO vertex of maximal lag is ready (claim (d): never for M >= 6),
//   Theta = sum max(lag-6,0)^2 <= t.
// modes:  strat ex n W TB      exhaustive over all lists of length n on W vertices
//         strat beam n W TB B obj   beam search (width B) for lists maximising obj: 0 = max lag, 1 = max(5*eta - t), 2 = lag at fixed centre
//         strat list TB a1 a2 ...   run one list (vertices 0..), verbose summary
#include <bits/stdc++.h>
using namespace std;
static const int PAD = 40, NV = 120;
struct St { short k[NV]; short F[NV]; short eta[NV]; int t; int maxlag5; int maxexc; int minNotReady5; bool bad; int lo, hi; };
static int TB = 0, CK = 3, LAZY5 = 5, THR5 = 30;   // CK!=3 or THR5<30: deliberately broken variants; LAZY5: least lag*5 needed to fire
static inline int L5(const St& s, int x) { return CK * s.k[x] + s.k[x - 1] + s.k[x + 1] - 5 * s.F[x]; }
static void initSt(St& s) { memset(&s, 0, sizeof s); s.maxexc = -1000000; s.minNotReady5 = 0; s.lo = NV; s.hi = -1; }
static void fail(const char* m, const St& s) { fprintf(stderr, "INVARIANT VIOLATED: %s at t=%d\n", m, s.t); exit(7); }
static void roundStep(St& s, int a) {   // a = cell index
    s.t++; s.k[a]++; s.eta[a]++;
    if (a - 1 < s.lo) s.lo = a - 1; if (a + 1 > s.hi) s.hi = a + 1;
    int lo = s.lo - 1, hi = s.hi + 1;
    if (lo < 3 || hi > NV - 4) fail("board too small", s);
    // after step (1)
    int M5 = 0; for (int x = lo; x <= hi; x++) { int l = L5(s, x); if (l < 0) fail("negative lag", s); M5 = max(M5, l); }
    for (int x = lo; x <= hi; x++) {
        int star5 = s.k[x - 2] + s.k[x - 1] + s.k[x] + s.k[x + 1] + s.k[x + 2];
        if (5 * s.eta[x] != star5 + 2 * L5(s, x) - L5(s, x - 1) - L5(s, x + 1)) fail("height identity (**)", s);
        if (s.eta[x] != s.k[x] - 2 * s.F[x] + s.F[x - 1] + s.F[x + 1]) fail("(*)", s);
        s.maxexc = max(s.maxexc, 5 * s.eta[x] - s.t);
    }
    s.maxlag5 = max(s.maxlag5, M5);
    bool readyMax = false; for (int x = lo; x <= hi; x++) if (L5(s, x) == M5 && s.eta[x] >= 2) readyMax = true;
    if (!readyMax && M5 > 0) s.minNotReady5 = max(s.minNotReady5, M5);   /* field holds the LARGEST such M */
    if (!readyMax && M5 >= THR5) fail("claim (d): max lag >= 6 but no max-lag vertex ready", s);
    // step (2)
    int best = -1, bl = LAZY5 - 1;
    for (int x = lo; x <= hi; x++) if (s.eta[x] >= 2) { int l = L5(s, x); if (l < LAZY5) continue;
        bool take = false;
        if (l > bl) take = true; else if (l == bl && best >= 0) { if (TB == 1) take = true; else if (TB == 2) take = s.eta[x] < s.eta[best]; else if (TB == 3) take = s.eta[x] > s.eta[best]; }
        if (take) { best = x; bl = l; } }
    if (best >= 0) {
        if (M5 >= 30 && bl != M5) fail("fired vertex is not of maximal lag although M>=6", s);
        s.eta[best] -= 2; s.eta[best - 1]++; s.eta[best + 1]++; s.F[best]++;
        if (best - 1 < s.lo) s.lo = best - 1; if (best + 1 > s.hi) s.hi = best + 1;
    }
    // after step (2)
    long long theta25 = 0;
    for (int x = lo - 1; x <= hi + 1; x++) {
        if (s.eta[x] < 0) fail("negative chips", s);
        int l = L5(s, x); if (l < 0) fail("negative lag after firing", s);
        if (l > 30) theta25 += (long long)(l - 30) * (l - 30);
        s.maxexc = max(s.maxexc, 5 * s.eta[x] - s.t);
        if (s.eta[x] > s.t / 5.0 + 13.2 + 2 * sqrt((double)s.t) + 1e-9) fail("height bound (f)", s);
    }
    if (theta25 > 25LL * s.t) fail("Theta <= t", s);
}
static int gMaxLag5 = 0, gMaxExc = -1000000, gMinNR = 0; static long long gCount = 0;
static vector<int> cur, bestLagList, bestExcList, nrList;
static void ex(const St& s, int n, int W) {
    if ((int)cur.size() == n) { gCount++;
        if (s.maxlag5 > gMaxLag5) { gMaxLag5 = s.maxlag5; bestLagList = cur; }
        if (s.maxexc > gMaxExc) { gMaxExc = s.maxexc; bestExcList = cur; }
        if (s.minNotReady5 > gMinNR) { gMinNR = s.minNotReady5; nrList = cur; }
        return; }
    for (int a = 0; a < W; a++) { if (cur.empty() && a > (W - 1) / 2 && TB >= 2) break; St t = s; roundStep(t, PAD + a); cur.push_back(a); ex(t, n, W); cur.pop_back(); }
}
static void pr(const char* nm, const vector<int>& v) { printf("%s:", nm); for (int a : v) printf(" %d", a); printf("\n"); }
int main(int argc, char** argv) {
    string mode = argv[1];
    if (getenv("CK")) CK = atoi(getenv("CK")); if (getenv("LAZY5")) LAZY5 = atoi(getenv("LAZY5")); if (getenv("THR5")) THR5 = atoi(getenv("THR5"));
    if (mode == "ex") {
        int n = atoi(argv[2]), W = atoi(argv[3]); TB = atoi(argv[4]);
        St s; initSt(s); ex(s, n, W);
        printf("ex n=%d W=%d TB=%d lists=%lld : max lag=%.1f  max(height - t/5)=%.1f  largest max-lag value with NO max-lag vertex ready=%.1f\n", n, W, TB, gCount, gMaxLag5 / 5.0, gMaxExc / 5.0, gMinNR / 5.0);
        pr("  lag list", bestLagList); pr("  height list", bestExcList); pr("  not-ready list", nrList);
    } else if (mode == "beam") {
        int n = atoi(argv[2]), W = atoi(argv[3]); TB = atoi(argv[4]); int B = atoi(argv[5]), obj = atoi(argv[6]);
        struct Node { St s; vector<signed char> l; long long sc; };
        vector<Node> beam(1); initSt(beam[0].s); beam[0].sc = 0;
        mt19937 rng(12345 + obj * 7 + W);
        int bestLag = 0, bestExc = -1000000, bestNR = 0; vector<signed char> bl, be, bn;
        for (int t = 1; t <= n; t++) {
            vector<Node> nx; nx.reserve(beam.size() * W);
            for (auto& nd : beam) for (int a = 0; a < W; a++) {
                Node c; c.s = nd.s; roundStep(c.s, PAD + a); c.l = nd.l; c.l.push_back(a);
                // current (not historical) quantities drive the search
                int curM = 0, curE = -1000000, sumsq = 0; for (int x = c.s.lo - 1; x <= c.s.hi + 1; x++) { int l = L5(c.s, x); curM = max(curM, l); sumsq += l * l; curE = max(curE, 5 * c.s.eta[x] - c.s.t); }
                if (obj == 0) c.sc = 1000LL * curM + sumsq / 8; else if (obj == 1) c.sc = 1000LL * curE + curM; else c.sc = 1000LL * L5(c.s, PAD + W / 2) + curM;
                c.sc = c.sc * 16 + (rng() & 15);
                if (c.s.maxlag5 > bestLag) { bestLag = c.s.maxlag5; bl = c.l; }
                if (c.s.maxexc > bestExc) { bestExc = c.s.maxexc; be = c.l; }
                if (c.s.minNotReady5 > bestNR) { bestNR = c.s.minNotReady5; bn = c.l; }
                nx.push_back(std::move(c));
            }
            if ((int)nx.size() > B) { nth_element(nx.begin(), nx.begin() + B, nx.end(), [](const Node& x, const Node& y) { return x.sc > y.sc; }); nx.resize(B); }
            beam.swap(nx);
        }
        printf("beam n=%d W=%d TB=%d B=%d obj=%d : max lag=%.1f  max(height - t/5)=%.1f  largest not-ready max-lag=%.1f\n", n, W, TB, B, obj, bestLag / 5.0, bestExc / 5.0, bestNR / 5.0);
        printf("  lag list:"); for (int a : bl) printf(" %d", a); printf("\n  height list:"); for (int a : be) printf(" %d", a); printf("\n");
    } else if (mode == "list") {
        TB = atoi(argv[2]); St s; initSt(s);
        for (int i = 3; i < argc; i++) roundStep(s, PAD + atoi(argv[i]));
        printf("list len=%d TB=%d : max lag=%.1f max(height - t/5)=%.1f min not-ready max-lag=%.1f\n", s.t, TB, s.maxlag5 / 5.0, s.maxexc / 5.0, s.minNotReady5 / 5.0);
    }
    return 0;
}
