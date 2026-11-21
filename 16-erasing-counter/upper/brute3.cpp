// Exhaustive check, straight from the definition, over ALL trees of depth <= R on at most MAXI indices,
// up to relabelling of indices (indices are named in order of first appearance, preorder: node, 0-subtree, 1-subtree).
// For every tree and every cycle of its map f it checks / records:
//   E   : number of steps on the cycle using a leaf with R = empty   (claim: <= 1)
//   P1  : cycle length N <= L(T), where L(leaf)=1, L(node)=L(1-child)*(L(0-child)+1)
//   P2  : number of cycle states with root bit 1  <= L(T1)           (mixed cycles)
//   P3  : for each leaf, uses on the cycle <= product over 0-edges on its path of L(1-sibling)
//   RUN : longest run of consecutive root=0 states on a mixed cycle vs W_{depth(T0)}
//   X1  : number of distinct "rest of memory" values among root=1 states that follow a root=0 state
//   PC  : longest orbit segment of distinct states in which no step except possibly the last uses an empty leaf,
//         compared with W_depth(T) and L(T)   (paths, not cycles)
// usage: brute R MAXI [mutate]
//   mutate=1 : deliberately wrong L (L(node)=L(1)*L(0)+0) to watch P1 fail
//   mutate=2 : count write-root leaves as empty to watch E fail
#include <bits/stdc++.h>
using namespace std;
int R, MAXI, MUT = 0;
int kind[64], val[64]; // heap layout, root = 1; kind 1 internal, 2 leaf; val = index or write index (-1 = none)
long long ntrees = 0;
static const long long W[6] = {1, 2, 6, 42, 1806, 3263442};
long long Lv[64]; int dep[64];
int f[1 << 8], leafOf[1 << 8]; bool emp[1 << 8];
int seen[1 << 8], plen[1 << 8];
// statistics
long long maxE = 0, cntE2 = 0, violP1 = 0, violP2 = 0, violP3 = 0, violRUN = 0, violPCW = 0, violPCL = 0, maxX1 = 0, violN1W = 0;
long long violINJ=0, violRDdisj=0, violRDshare=0, nDisj=0, violX=0; int M0cur, M1cur; bool disjCur; long long bestByDepth[6]; long long bestPathByDepth[6];
double maxRatioP1 = 0; long long tightP1 = 0;
string show(int n) {
    if (kind[n] == 1) return "(" + to_string(val[n] + 1) + " " + show(2 * n) + " " + show(2 * n + 1) + ")";
    if (val[n] >= 0) return "[" + to_string(val[n] + 1) + "]";
    return "[]";
}
int wit = 0;
string shapeStr(int n) { if (kind[n] != 1) return "."; return "(" + shapeStr(2 * n) + shapeStr(2 * n + 1) + ")"; }
bool readOnce() { int c[16] = {0}; for (int n = 1; n < 64; n++) if (kind[n] == 1) { bool live = true; for (int p = n >> 1; p >= 1; p >>= 1) if (kind[p] != 1) live = false; if (live) c[val[n]]++; } for (int i = 0; i < 16; i++) if (c[i] > 1) return false; return true; }
unsigned idxMask(int n) { if (kind[n] != 1) return 0; return (1u << val[n]) | idxMask(2 * n) | idxMask(2 * n + 1); }
int longestCycleOf(const int *g, int n) { // g on states 0..n-1
    static int col[1 << 8], pos[1 << 8]; int best = 0;
    for (int s = 0; s < n; s++) col[s] = 0;
    for (int s = 0; s < n; s++) { if (col[s]) continue; int x = s, c = 0; while (!col[x]) { col[x] = s + 1; pos[x] = c++; x = g[x]; }
        if (col[x] == s + 1) best = max(best, c - pos[x]); }
    return best;
}
unsigned long long sigOf(int n, int s) { // exploration signature: always the 1-child; the 0-child iff the bit is 0
    unsigned long long m = 1ull << n;
    if (kind[n] != 1) return m;
    m |= sigOf(2 * n + 1, s);
    if (MUT == 3) return m;
    if (!((s >> val[n]) & 1)) m |= sigOf(2 * n, s);
    return m;
}
void witness(const char *what, int s, long long a, long long b) {
    if (wit++ < 40) printf("WITNESS %s: tree %s state %d : %lld vs %lld\n", what, show(1).c_str(), s, a, b);
}
void shape(int n) {
    if (kind[n] == 2) { Lv[n] = 1; dep[n] = 0; return; }
    shape(2 * n); shape(2 * n + 1);
    Lv[n] = Lv[2 * n + 1] * (Lv[2 * n] + (MUT == 1 ? 0 : 1));
    dep[n] = 1 + max(dep[2 * n], dep[2 * n + 1]);
}
long long capOf(int leaf) { // product over 0-edges on the path of L(1-sibling)
    long long c = 1;
    for (int n = leaf; n > 1; n >>= 1) if ((n & 1) == 0) c *= Lv[n + 1];
    return c;
}
void evaluate(int m) {
    ntrees++;
    shape(1);
    int n = 1 << m;
    for (int s = 0; s < n; s++) {
        int t = 1; unsigned P = 0;
        while (kind[t] == 1) { int i = val[t]; P |= 1u << i; t = 2 * t + ((s >> i) & 1); }
        unsigned o = s & ~P; bool e = val[t] < 0;
        if (MUT == 2 && val[t] == 0) e = true;
        if (val[t] >= 0) o |= 1u << val[t];
        f[s] = o; emp[s] = e; leafOf[s] = t;
    }
    int D = dep[1];
    M0cur = M1cur = 1; disjCur = false;
    if (kind[1] == 1) { // maps of T0°, T1° on the non-root bits (root index is 0 in the canonical naming)
        static int g0[1 << 8], g1[1 << 8];
        for (int x = 0; x < n / 2; x++) { g0[x] = f[x << 1] >> 1; g1[x] = f[(x << 1) | 1] >> 1; }
        M0cur = longestCycleOf(g0, n / 2); M1cur = longestCycleOf(g1, n / 2);
        disjCur = (idxMask(2) & idxMask(3)) == 0; if (disjCur) nDisj++;
    }
    // cycles
    for (int s = 0; s < n; s++) seen[s] = 0;
    static int cnt[64];
    for (int s = 0; s < n; s++) {
        if (seen[s]) continue;
        int x = s; while (!seen[x]) { seen[x] = s + 1; x = f[x]; }
        if (seen[x] != s + 1) continue;
        // x is on a new cycle
        int st = x; { int z = x; do { if (z & 1) { st = f[z]; break; } z = f[z]; } while (z != x); }
        int len = 0, ne = 0, n1 = 0, n0 = 0, run = 0, maxrun = 0; int y = st;
        set<int> x1; int prevRoot = -1;
        // predecessor root bit of st: st follows a root=1 state if any root=1 exists
        memset(cnt, 0, sizeof cnt); vector<unsigned long long> sg;
        // need previous state's root bit for X1: walk once to find predecessor of st
        { int z = st; while (f[z] != st) z = f[z]; prevRoot = z & 1; }
        do {
            len++; if (emp[y]) ne++;
            if (y & 1) { n1++; run = 0; if (prevRoot == 0) x1.insert(y >> 1); }
            else { n0++; run++; maxrun = max(maxrun, run); }
            cnt[leafOf[y]]++; sg.push_back(sigOf(1, y));
            prevRoot = y & 1; y = f[y];
        } while (y != st);
        if (len >= 2) { if (ne > maxE) maxE = ne; if (ne >= 2) { cntE2++; witness("E", st, ne, 1); } }
        sort(sg.begin(), sg.end()); for (size_t q = 1; q < sg.size(); q++) if (sg[q] == sg[q - 1]) { violINJ++; witness("INJ", st, len, 0); break; }
        if (kind[1] == 1) {
            long long bd = (long long)M1cur * (M0cur + (MUT == 4 ? 0 : 1));
            if (len > bd) { if (disjCur) { violRDdisj++; witness("RD_DISJOINT", st, len, bd); } else { violRDshare++; if (violRDshare <= 3) witness("RD_SHARED(informational)", st, len, bd); } }
            if (disjCur && n1 > 0 && n0 > 0 && x1.size() > 0) { // all root=1 states agree on idx(T0)
                unsigned m0 = idxMask(2); int ref = -1; int z = st; bool bad = false;
                do { if (z & 1) { int v = z & m0; if (ref < 0) ref = v; else if (ref != v) bad = true; } z = f[z]; } while (z != st);
                if (bad) { violX++; witness("RD_XSTAR", st, len, 0); }
            }
        }
        if (len > bestByDepth[D]) bestByDepth[D] = len;
        if (len > Lv[1]) { violP1++; witness("P1", st, len, Lv[1]); }
        if (len == Lv[1]) { tightP1++; printf("TIGHT %s readonce=%d tree %s\n", shapeStr(1).c_str(), (int)readOnce(), show(1).c_str()); }
        if (kind[1] == 1 && n1 > 0 && n0 > 0) {
            if (n1 > Lv[3]) { violP2++; witness("P2", st, n1, Lv[3]); }
            if (n1 > W[dep[3]]) { violN1W++; witness("N1W", st, n1, W[dep[3]]); }
            if (maxrun > W[dep[2]]) { violRUN++; witness("RUN", st, maxrun, W[dep[2]]); }
            if ((long long)x1.size() > maxX1) { maxX1 = x1.size(); witness("X1(new max, informational)", st, x1.size(), 1); }
        }
        if (len >= 2) for (int t = 2; t < 2 * (1 << R); t++) if (cnt[t]) {
            long long c = capOf(t);
            if (cnt[t] > c) { violP3++; witness("P3", st, cnt[t], c); }
        }
    }
    // PC: longest empty-free simple path (last step may be empty)
    // states whose step is empty are sinks of length 1; otherwise follow f.
    for (int s = 0; s < n; s++) { seen[s] = 0; plen[s] = 0; }
    int best = 0, bs = 0;
    static int stk[1 << 8];
    for (int s = 0; s < n; s++) {
        if (plen[s]) continue;
        int sp = 0, x = s;
        while (!plen[x] && seen[x] != s + 1) {
            seen[x] = s + 1; stk[sp++] = x;
            if (emp[x]) { plen[x] = 1; sp--; break; }
            x = f[x];
        }
        if (!plen[x]) { // found a cycle of non-empty steps through x, all on current stack
            int c = 0, q = sp; do { q--; c++; } while (stk[q] != x);
            for (int j = q; j < sp; j++) plen[stk[j]] = c;
            sp = q; x = sp ? stk[sp - 1] : x;
            if (sp) x = f[stk[sp - 1]];
        }
        // unwind: stk[sp-1] -> x with plen[x] known
        while (sp) { int u = stk[--sp]; plen[u] = 1 + plen[f[u]]; }
    }
    for (int s = 0; s < n; s++) if (plen[s] > best) { best = plen[s]; bs = s; }
    if (best > bestPathByDepth[D]) bestPathByDepth[D] = best;
    if (best > W[D]) { violPCW++; witness("PC_W", bs, best, W[D]); }
    if (best > Lv[1]) { violPCL++; witness("PC_L", bs, best, Lv[1]); }
}
int stackp[64], sp = 0;
void rec(int used) {
    if (sp == 0) { evaluate(used); return; }
    int p = stackp[--sp];
    int path[8], pl = 0; unsigned P = 0;
    for (int n = p >> 1; n >= 1; n >>= 1) { path[pl++] = val[n]; P |= 1u << val[n]; }
    kind[p] = 2;
    val[p] = -1; rec(used);
    for (int q = 0; q < pl; q++) { val[p] = path[q]; rec(used); }
    if (pl < R) {
        kind[p] = 1;
        stackp[sp++] = 2 * p + 1; stackp[sp++] = 2 * p;
        for (int i = 0; i < used; i++) { if (P >> i & 1) continue; val[p] = i; rec(used); }
        if (used < MAXI) { val[p] = used; rec(used + 1); }
        sp -= 2;
    }
    kind[p] = 0;
    stackp[sp++] = p;
}
int main(int argc, char **argv) {
    R = atoi(argv[1]); MAXI = atoi(argv[2]); if (argc > 3) MUT = atoi(argv[3]);
    stackp[sp++] = 1; rec(0);
    printf("R=%d MAXI=%d MUT=%d trees(up to relabelling)=%lld\n", R, MAXI, MUT, ntrees);
    printf("longest cycle by exact depth:"); for (int i = 0; i <= R; i++) printf(" %lld", bestByDepth[i]); printf("\n");
    printf("longest empty-free path by exact depth:"); for (int i = 0; i <= R; i++) printf(" %lld", bestPathByDepth[i]); printf("\n");
    printf("E: max empty steps on a cycle of length>=2: %lld (cycles with >=2: %lld)\n", maxE, cntE2);
    printf("P1 violations (N > L(T)): %lld ; cycles with N == L(T): %lld\n", violP1, tightP1);
    printf("P2 violations (#root=1 states > L(T1), mixed cycles): %lld ; (> W_depth(T1)): %lld\n", violP2, violN1W);
    printf("INJ violations (two states of one cycle with the same exploration signature): %lld\n", violINJ);
    printf("RD: trees with index-disjoint root subtrees: %lld ; their cycles longer than M(T1o)(M(T0o)+1): %lld ; root=1 states disagreeing on idx(T0): %lld\n", nDisj, violRDdisj, violX);
    printf("RD product bound violated by cycles of trees whose root subtrees SHARE an index: %lld\n", violRDshare);
    printf("P3 violations (leaf use > cap): %lld\n", violP3);
    printf("RUN violations (root=0 run > W_depth(T0)): %lld\n", violRUN);
    printf("X1 max distinct memories at entry to root=1: %lld\n", maxX1);
    printf("PC violations: path > W_depth: %lld ; path > L(T): %lld\n", violPCW, violPCL);
}
