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
int main(int argc, char** argv) {
    G = argv[1][0];
    if (string(argv[2]) == "table") {
        int lo = atoi(argv[3]), hi = atoi(argv[4]); if (argc > 5) SZ = atoi(argv[5]);
        int h = lo; if (argc > 6) h = atoi(argv[6]);
        for (M = lo; M <= hi; M++) {
            for (H = max(h, M);; H++) {
                bool w = solve(false);
                printf("G=%c M=%d H=%d states=%zu sweeps=%d %s\n", G, M, H, st.size(), sweeps, w ? "WIN" : "LOSE");
                fflush(stdout);
                if (w) break;
            }
            h = H; printf("RESULT G=%c sizes=%d M=%d h=%d\n", G, SZ, M, h); fflush(stdout);
        }
        return 0;
    }
    M = atoi(argv[2]); H = atoi(argv[3]); if (argc > 4) SZ = atoi(argv[4]);
    bool w = solve(true);
    size_t ns = 0; for (char x : flg) ns += x;
    printf("G=%c M=%d H=%d sizes=%d states=%zu safe=%zu sweeps=%d %s\n", G, M, H, SZ, st.size(), ns, sweeps, w ? "WIN" : "LOSE");
    if (argc > 5) {
        FILE* f = fopen(argv[5], "w");
        for (size_t i = 0; i < st.size(); i++) if (ok(st[i])) {
            for (int p = 0; p < H; p++) fputc(".123"[cell(st[i], p)], f);
            fputc('\n', f);
        }
        fclose(f);
    }
    return 0;
}
