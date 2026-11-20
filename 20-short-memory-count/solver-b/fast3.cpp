// fast3: same search and same pruning as fast2 (node counts must agree), but the reachable-window sets are
// maintained incrementally along the DFS (they only grow when a transition is added).
//   fast3 W N find S [all [printmax]]
// env PREFIX="t0 t1 ..." restricts the first transitions (work splitting); env NOSYM10=1 skips the branch
// d(0,0)=1,d(0,1)=0, which is the bit-complement image of the branch d(0,0)=0,d(0,1)=1 (complementing bits maps
// C(q) -> w-C(q), so spreads and g are unchanged; safe for existence, changes counts).
// env SPLITDEPTH=k : do not search; print every surviving prefix of length k (one per line) for splitting.
#include <bits/stdc++.h>
#include <immintrin.h>
using namespace std;
const int MAXN = 13, MAXWD = 32, MAXW = 11, MAXD = 2 * MAXN + 2;
int W, N, NW, HW;
struct BS { uint64_t w[MAXWD]; };
BS PC[MAXW + 1];
int tr[MAXN][2];
struct Frame { BS L[MAXW][MAXN]; BS R[MAXN]; int spr[MAXN]; };
Frame* fr; // fr[depth]
static inline void stepset(const BS& S, int b, BS& R) {
    if (W <= 6) {
        int half = 1 << (W - 1);
        uint64_t s = S.w[0]; uint64_t F = (s | (s >> half)) & ((1ULL << half) - 1);
        R.w[0] = _pdep_u64(F, 0x5555555555555555ULL) << b; return;
    }
    for (int i = 0; i < HW; i++) {
        uint64_t F = S.w[i] | S.w[i + HW];
        R.w[2 * i] = _pdep_u64(F & 0xffffffffULL, 0x5555555555555555ULL) << b;
        R.w[2 * i + 1] = _pdep_u64(F >> 32, 0x5555555555555555ULL) << b;
    }
}
static inline bool orin(BS& t, const BS& r) { bool ch = false; for (int i = 0; i < NW; i++) { uint64_t n = t.w[i] | r.w[i]; if (n != t.w[i]) { t.w[i] = n; ch = true; } } return ch; }
static inline bool anyb(const BS& a) { for (int i = 0; i < NW; i++) if (a.w[i]) return true; return false; }
static inline bool inter(const BS& a, const BS& b) { for (int i = 0; i < NW; i++) if (a.w[i] & b.w[i]) return true; return false; }
// add edge (q,b)->t to frame f (states 0..m-1 exist, tr already set)
void addedge(Frame& f, int m, int q, int b, int t) {
    bool chg[MAXW + 1][MAXN]; memset(chg, 0, sizeof chg);
    bool rch[MAXN]; memset(rch, 0, sizeof rch);
    BS r;
    for (int k = 0; k < W; k++) {
        // new edge at level k
        if (anyb(f.L[k][q])) {
            stepset(f.L[k][q], b, r);
            if (k + 1 < W) { if (orin(f.L[k + 1][t], r)) chg[k + 1][t] = true; }
            else { if (orin(f.R[t], r)) rch[t] = true; }
        }
        if (k >= 1) for (int x = 0; x < m; x++) if (chg[k][x]) for (int c = 0; c < 2; c++) {
            int y = tr[x][c]; if (y < 0) continue; if (x == q && c == b) continue; // already done above with full set
            stepset(f.L[k][x], c, r);
            if (k + 1 < W) { if (orin(f.L[k + 1][y], r)) chg[k + 1][y] = true; }
            else { if (orin(f.R[y], r)) rch[y] = true; }
        }
    }
    if (anyb(f.R[q])) { stepset(f.R[q], b, r); if (orin(f.R[t], r)) rch[t] = true; }
    bool again = true;
    while (again) {
        again = false;
        for (int x = 0; x < m; x++) if (rch[x]) {
            rch[x] = false;
            for (int c = 0; c < 2; c++) { int y = tr[x][c]; if (y < 0) continue; stepset(f.R[x], c, r); if (orin(f.R[y], r)) { rch[y] = true; again = true; } }
        }
    }
    for (int x = 0; x < m; x++) {
        f.spr[x] = -1;
        if (!anyb(f.R[x])) continue;
        int lo = 0, hi = W;
        while (!inter(f.R[x], PC[lo])) lo++;
        while (!inter(f.R[x], PC[hi])) hi--;
        f.spr[x] = hi - lo;
    }
}
bool cyclic(const int* spr, int m, int s) {
    int indeg[MAXN] = {0}; bool in[MAXN];
    for (int q = 0; q < m; q++) in[q] = spr[q] > s;
    for (int q = 0; q < m; q++) if (in[q]) for (int b = 0; b < 2; b++) { int t = tr[q][b]; if (t >= 0 && in[t]) indeg[t]++; }
    int st[MAXN], sp = 0, removed = 0, tot = 0;
    for (int q = 0; q < m; q++) if (in[q]) { tot++; if (!indeg[q]) st[sp++] = q; }
    while (sp) { int q = st[--sp]; removed++; for (int b = 0; b < 2; b++) { int t = tr[q][b]; if (t >= 0 && in[t] && --indeg[t] == 0) st[sp++] = t; } }
    return removed != tot;
}
int S; bool countall = false; int printmax = 20; bool nosym = false; int splitdepth = -1;
unsigned long long nodes = 0, found = 0; bool stop = false;
vector<int> prefix;
void copyframe(Frame& d, const Frame& s, int m) {
    for (int k = 0; k < W; k++) for (int q = 0; q < m; q++) memcpy(d.L[k][q].w, s.L[k][q].w, 8 * NW);
    for (int q = 0; q < m; q++) memcpy(d.R[q].w, s.R[q].w, 8 * NW);
}
int order = 0; // 0 = BFS numbering (queue), 1 = DFS numbering, 0-edge first (stack), 2 = DFS, 1-edge first
int ag[MAXD][2 * MAXN + 2][2]; int agn[MAXD]; // agenda per depth
int choice[MAXD];
void rec(int pos, int m) {
    if (stop) return;
    if (pos == splitdepth || (agn[pos] == 0 && splitdepth >= 0)) { for (int i = 0; i < pos; i++) printf("%d ", choice[i]); printf("\n"); return; }
    if (agn[pos] == 0) {
        found++;
        if (found == 1 || (countall && (long long)found <= printmax)) {
            int g = 0; while (cyclic(fr[pos].spr, m, g)) g++;
            printf("%d %d", W, m); for (int x = 0; x < m; x++) printf(" %d %d", tr[x][0], tr[x][1]); printf(" %d\n", g); fflush(stdout);
        }
        if (!countall) stop = true; return;
    }
    int q, b, n = agn[pos];
    // take next agenda item: queue front for BFS, stack top for DFS
    int idx = (order == 0) ? 0 : n - 1;
    q = ag[pos][idx][0]; b = ag[pos][idx][1];
    int lim = min(m, N - 1);
    for (int t = 0; t <= lim; t++) {
        if (pos < (int)prefix.size() && prefix[pos] != t) continue;
        tr[q][b] = t; choice[pos] = t;
        int m2 = max(m, t + 1);
        nodes++;
        // build agenda for child
        int k = 0;
        for (int i = 0; i < n; i++) if (i != idx) { ag[pos + 1][k][0] = ag[pos][i][0]; ag[pos + 1][k][1] = ag[pos][i][1]; k++; }
        if (m2 > m) {
            if (order == 0) { ag[pos + 1][k][0] = m; ag[pos + 1][k][1] = 0; k++; ag[pos + 1][k][0] = m; ag[pos + 1][k][1] = 1; k++; }
            else if (order == 1) { ag[pos + 1][k][0] = m; ag[pos + 1][k][1] = 1; k++; ag[pos + 1][k][0] = m; ag[pos + 1][k][1] = 0; k++; }
            else { ag[pos + 1][k][0] = m; ag[pos + 1][k][1] = 0; k++; ag[pos + 1][k][0] = m; ag[pos + 1][k][1] = 1; k++; }
        }
        agn[pos + 1] = k;
        Frame& f = fr[pos + 1];
        copyframe(f, fr[pos], m);
        if (m2 > m) { for (int kk = 0; kk < W; kk++) memset(f.L[kk][m].w, 0, 8 * NW); memset(f.R[m].w, 0, 8 * NW); }
        addedge(f, m2, q, b, t);
        if (!cyclic(f.spr, m2, S)) rec(pos + 1, m2);
        tr[q][b] = -1;
        if (stop) return;
    }
}
int main(int argc, char** argv) {
    W = atoi(argv[1]); N = atoi(argv[2]); S = atoi(argv[4]);
    if (argc > 5) countall = true;
    if (argc > 6) printmax = atoi(argv[6]);
    if (W > MAXW || N > MAXN) { fprintf(stderr, "too big\n"); return 2; }
    NW = W <= 6 ? 1 : (1 << (W - 6)); HW = NW / 2;
    memset(PC, 0, sizeof PC);
    for (int u = 0; u < (1 << W); u++) PC[__builtin_popcount(u)].w[u >> 6] |= 1ULL << (u & 63);
    memset(tr, -1, sizeof tr);
    if (getenv("PREFIX")) { stringstream ss(getenv("PREFIX")); int x; while (ss >> x) prefix.push_back(x); }
    if (getenv("NOSYM10")) nosym = true;
    if (getenv("SPLITDEPTH")) splitdepth = atoi(getenv("SPLITDEPTH"));
    fr = (Frame*)calloc(MAXD, sizeof(Frame));
    fr[0].L[0][0].w[0] = 1; fr[0].spr[0] = -1;
    if (getenv("ORDER")) order = atoi(getenv("ORDER"));
    if (order == 0) { ag[0][0][0] = 0; ag[0][0][1] = 0; ag[0][1][0] = 0; ag[0][1][1] = 1; }
    else if (order == 1) { ag[0][0][0] = 0; ag[0][0][1] = 1; ag[0][1][0] = 0; ag[0][1][1] = 0; }
    else { ag[0][0][0] = 0; ag[0][0][1] = 0; ag[0][1][0] = 0; ag[0][1][1] = 1; }
    agn[0] = 2;
    rec(0, 1);
    fprintf(stderr, "w=%d N=%d find S=%d%s%s: %s (found=%llu, nodes=%llu, exhaustive=%s)\n", W, N, S, nosym ? " NOSYM10" : "", getenv("PREFIX") ? (string(" PREFIX=") + getenv("PREFIX")).c_str() : "", found ? "EXISTS" : "NONE", found, nodes, (stop ? "no(stopped at first)" : "yes"));
}
