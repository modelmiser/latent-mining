// Exhaustive safety-game solver for the allocation games N, F, O.
// State = layout of H cells, 2 bits per cell: 0 free, 1 = a 1-block, 2 = cell of a 2-block,
// 3 = cell of a 3-block (runs of 2s pair up left to right, runs of 3s triple up).
// Greatest fixpoint: start with all layouts (live <= M) safe, delete until stable.
// solve2p = solve2 with the sweep split over 2 threads (flags only ever go 1->0, and the run stops
// only after a sweep with no change, so the result is the same fixpoint) and an optional start H in table mode
// (solve2p G table Mlo Mhi sizesMask Hstart).
// solve2 = solve.cpp with a hash index instead of binary search (same algorithm).
// usage: solve G M H [sizesMask=7] [dumpfile]   -> prints WIN/LOSE for the manager
//        solve G table Mlo Mhi [sizesMask=7]    -> least H for each M
#include <bits/stdc++.h>
using namespace std;
typedef uint64_t u64;
static int H, M, SZ = 7; static char G;
static vector<u64> st; static vector<u64> keys; static vector<char> flg; static u64 mask_;
static const u64 PAT[4] = {0, 1, 10, 63};
static void gen(int pos, u64 code, int live) {
    if (pos == H) { st.push_back(code); return; }
    gen(pos + 1, code, live);
    for (int s = 1; s <= 3; s++)
        if ((SZ >> (s - 1) & 1) && pos + s <= H && live + s <= M)
            gen(pos + s, code | (PAT[s] << (2 * pos)), live + s);
}
static inline size_t slot(u64 c) { u64 h = (c + 1) * 0x9E3779B97F4A7C15ULL; h ^= h >> 29; size_t i = h & mask_;
    while (keys[i] != c + 1 && keys[i] != 0) i = (i + 1) & mask_; return i; }
static inline bool ok(u64 c) { return flg[slot(c)]; }
static inline int cell(u64 c, int p) { return (c >> (2 * p)) & 3; }
// any single move of a 1 leading to a safe layout; if deeper, allow a second 1-move
static bool move1(u64 c, bool deeper) {
    for (int p = 0; p < H; p++) if (cell(c, p) == 1) {
        u64 c1 = c & ~(3ULL << (2 * p));
        for (int q = 0; q < H; q++) if (q != p && cell(c1, q) == 0) {
            u64 c2 = c1 | (1ULL << (2 * q));
            if (ok(c2)) return true;
            if (deeper && move1(c2, false)) return true;
        }
    }
    return false;
}
static bool move2(u64 c) {
    for (int p = 0; p + 1 < H; p++) if (cell(c, p) == 2) {
        u64 c1 = c & ~(15ULL << (2 * p));
        for (int q = 0; q + 1 < H; q++) if (q != p && cell(c1, q) == 0 && cell(c1, q + 1) == 0)
            if (ok(c1 | (10ULL << (2 * q)))) return true;
        p++;
    }
    return false;
}
static bool afterFree(u64 c, int s) {
    if (ok(c)) return true;
    if (G != 'F' || s == 1) return false;
    if (s == 2) return move1(c, false);
    return move2(c) || move1(c, true);
}
static bool eval(u64 c) {
    int live = 0;
    for (int p = 0; p < H; p++) if (cell(c, p)) live++;
    for (int s = 1; s <= 3; s++) if ((SZ >> (s - 1) & 1) && live + s <= M) {
        bool f = false;
        for (int p = 0; p + s <= H && !f; p++) {
            bool fr = true;
            for (int k = 0; k < s; k++) if (cell(c, p + k)) { fr = false; break; }
            if (fr && ok(c | (PAT[s] << (2 * p)))) f = true;
        }
        if (!f) return false;
    }
    for (int p = 0; p < H;) {
        int s = cell(c, p);
        if (!s) { p++; continue; }
        if (!(G == 'O' && s != 1)) {
            u64 c2 = c & ~(PAT[s] << (2 * p));
            if (!afterFree(c2, s)) return false;
        }
        p += s;
    }
    return true;
}
static int sweeps;
static bool solve(bool full) {
    st.clear(); gen(0, 0, 0); sort(st.begin(), st.end());
    size_t cap = 16; while (cap < 2 * st.size()) cap <<= 1; mask_ = cap - 1;
    keys.assign(cap, 0); flg.assign(cap, 0);
    for (u64 c : st) { size_t i = slot(c); keys[i] = c + 1; flg[i] = 1; }
    sweeps = 0; bool ch = true; size_t s0 = slot(0);
    while (ch) {
        ch = false; sweeps++;
        long n = (long)st.size(); int chg = 0;
        #pragma omp parallel for num_threads(2) schedule(dynamic, 8192) reduction(|:chg)
        for (long i = 0; i < n; i++) { size_t k = slot(st[i]); if (flg[k] && !eval(st[i])) { flg[k] = 0; chg = 1; } }
        ch = chg;
        if (!flg[s0] && !full) return false;
    }
    return flg[s0];
}

// ---- open-f-upper/strat.cpp: solver-b/solve2p.cpp core (unchanged above) + strategy statistics on the maximal safe set ----
static string sh(u64 c) { string s; for (int p = 0; p < H; p++) s += ".123"[cell(c, p)]; return s; }
static int top1(u64 c) { for (int p = H - 1; p >= 0; p--) if (cell(c, p) == 1) return p; return -1; }
static u64 compact(u64 c) { int t = top1(c); if (t < 0) return c; for (int p = 0; p < t; p++) if (cell(c, p) == 0) return (c & ~(3ULL << (2 * t))) | (1ULL << (2 * p)); return c; }
int main(int argc, char** argv) {
    G = argv[1][0]; M = atoi(argv[2]); H = atoi(argv[3]); int stride = argc > 4 ? atoi(argv[4]) : 1000;
    bool w = solve(true); size_t ns = 0; for (size_t i = 0; i < st.size(); i++) ns += ok(st[i]);
    printf("G=%c M=%d H=%d states=%zu safe=%zu sweeps=%d %s\n", G, M, H, st.size(), ns, sweeps, w ? "WIN" : "LOSE"); if (!w) return 0;
    long aTot[4] = {0}, aFFok[4] = {0}, aLFok[4] = {0}; long f2 = 0, f2nomove = 0, f2need = 0, f2comp = 0, f2nmoves = 0; long f3 = 0, f3nomove = 0, f3need = 0, f3two = 0, f3one = 0, f3comp2 = 0, f3onlytwo1 = 0;
    long ex = 0; long hist[64] = {0};
    for (size_t i = 0; i < st.size(); i++) { u64 c = st[i]; if (!ok(c)) continue; int live = 0; for (int p = 0; p < H; p++) if (cell(c, p)) live++;
        for (int s = 1; s <= 3; s++) if (live + s <= M) { aTot[s]++; int ff = -1, lf = -1; vector<int> good;
            for (int p = 0; p + s <= H; p++) { bool fr = true; for (int k = 0; k < s; k++) if (cell(c, p + k)) fr = false; if (!fr) continue; if (ff < 0) ff = p; lf = p; if (ok(c | (PAT[s] << (2 * p)))) good.push_back(p); }
            bool ffok = !good.empty() && good[0] == ff; if (ffok) aFFok[s]++; if (!good.empty() && good.back() == lf) aLFok[s]++;
            if (!ffok && (i % stride) == 0 && ex < 60) { ex++; printf("ALLOC%d first-fit UNSAFE  %s  ff=%d safe=", s, sh(c).c_str(), ff); for (int g : good) printf("%d ", g); printf("\n"); } }
        for (int p = 0; p < H;) { int s = cell(c, p); if (!s) { p++; continue; } u64 c2 = c & ~(PAT[s] << (2 * p));
            if (s == 2) { f2++; if (ok(c2)) f2nomove++; else { f2need++; if (ok(compact(c2))) f2comp++;
                    int n = 0; string lst; for (int a = 0; a < H; a++) if (cell(c2, a) == 1) { u64 c1 = c2 & ~(3ULL << (2 * a)); for (int q = 0; q < H; q++) if (q != a && cell(c1, q) == 0 && ok(c1 | (1ULL << (2 * q)))) { n++; if (n <= 8) lst += to_string(a) + ">" + to_string(q) + " "; } }
                    f2nmoves += n; hist[min(n, 63)]++; if ((f2need % (stride / 10 + 1)) == 0 && ex < 200) { ex++; printf("FREE2@%d move NEEDED  before=%s after=%s  safe moves(%d): %s\n", p, sh(c).c_str(), sh(c2).c_str(), n, lst.c_str()); } } }
            if (s == 3) { f3++; if (ok(c2)) f3nomove++; else { f3need++; bool m2 = move2(c2), m1 = move1(c2, false), m11 = move1(c2, true); if (m2) f3two++; if (m1) f3one++; if (!m2 && !m1 && m11) f3onlytwo1++; if (ok(compact(compact(c2))) || ok(compact(c2))) f3comp2++;
                    if ((f3need % (stride / 10 + 1)) == 0 && ex < 320) { ex++; printf("FREE3@%d move NEEDED  before=%s after=%s  a 2-move works:%d one 1-move works:%d two 1-moves work:%d\n", p, sh(c).c_str(), sh(c2).c_str(), m2, m1, m11); } } }
            p += s; } }
    for (int s = 1; s <= 3; s++) printf("alloc %d from safe states: %ld cases, first fit lands in the safe set in %ld (%.2f%%), last fit in %ld (%.2f%%)\n", s, aTot[s], aFFok[s], 100.0 * aFFok[s] / max(1L, aTot[s]), aLFok[s], 100.0 * aLFok[s] / max(1L, aTot[s]));
    printf("free 2: %ld cases; safe without a move %ld; move NEEDED %ld; of those compaction (highest 1 -> lowest free cell) is safe in %ld; mean #safe single moves %.2f\n", f2, f2nomove, f2need, f2comp, f2need ? (double)f2nmoves / f2need : 0.0);
    printf("  histogram of #safe moves when needed:"); for (int n = 0; n < 64; n++) if (hist[n]) printf(" %d:%ld", n, hist[n]); printf("\n");
    printf("free 3: %ld cases; safe without a move %ld; move NEEDED %ld; a 2-move works %ld; one 1-move works %ld; only two 1-moves work %ld; compaction(1 or 2 steps) works %ld\n", f3, f3nomove, f3need, f3two, f3one, f3onlytwo1, f3comp2);
    return 0;
}
