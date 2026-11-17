// Exhaustive safety-game solver for the allocation games N, F, O.
// State = layout of H cells, 2 bits per cell: 0 free, 1 = a 1-block, 2 = cell of a 2-block,
// 3 = cell of a 3-block (runs of 2s pair up left to right, runs of 3s triple up).
// Greatest fixpoint: start with all layouts (live <= M) safe, delete until stable.
// usage: solve G M H [sizesMask=7] [dumpfile]   -> prints WIN/LOSE for the manager
//        solve G table Mlo Mhi [sizesMask=7]    -> least H for each M
#include <bits/stdc++.h>
using namespace std;
typedef uint64_t u64;
static int H, M, SZ = 7; static char G;
static vector<u64> st; static vector<char> safe_;
static const u64 PAT[4] = {0, 1, 10, 63};
static void gen(int pos, u64 code, int live) {
    if (pos == H) { st.push_back(code); return; }
    gen(pos + 1, code, live);
    for (int s = 1; s <= 3; s++)
        if ((SZ >> (s - 1) & 1) && pos + s <= H && live + s <= M)
            gen(pos + s, code | (PAT[s] << (2 * pos)), live + s);
}
static inline size_t idx(u64 c) { return lower_bound(st.begin(), st.end(), c) - st.begin(); }
static inline bool ok(u64 c) { return safe_[idx(c)]; }
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
static int sweeps; static vector<int> dt; static int dtc;
static bool solve(bool full) {
    st.clear(); gen(0, 0, 0); sort(st.begin(), st.end());
    safe_.assign(st.size(), 1); sweeps = 0; dt.assign(st.size(),0); dtc=0;
    bool ch = true;
    while (ch) {
        ch = false; sweeps++;
        for (size_t i = st.size(); i-- > 0;) if (safe_[i] && !eval(st[i])) { safe_[i] = 0; dt[i]=++dtc; ch = true; }
        for (size_t i = 0; i < st.size(); i++) if (safe_[i] && !eval(st[i])) { safe_[i] = 0; dt[i]=++dtc; ch = true; }
        if (!safe_[0] && !full) return false;
    }
    return safe_[0];
}
static string show(u64 c){ string s; for(int p=0;p<H;p++) s.push_back(".123"[cell(c,p)]); return s; }
// principal variation for a LOSING H in games N/O: adversary picks a request all of whose answers died earlier;
// manager answers with the latest-dying layout.
static void pv() {
    u64 c = 0;
    while (true) {
        int T = dt[idx(c)]; int live = 0; for (int p = 0; p < H; p++) if (cell(c, p)) live++;
        bool done = false, found = false;
        for (int s = 1; s <= 3 && !found; s++) if ((SZ >> (s - 1) & 1) && live + s <= M) {
            bool all = true; u64 best = 0; int bt = -1;
            for (int p = 0; p + s <= H; p++) {
                bool fr = true; for (int k = 0; k < s; k++) if (cell(c, p + k)) fr = false;
                if (!fr) continue;
                u64 c2 = c | (PAT[s] << (2 * p)); int t = dt[idx(c2)];
                if (t == 0 || t >= T) { all = false; break; }
                if (t > bt) { bt = t; best = c2; }
            }
            if (all) { found = true; printf("%s  alloc %d", show(c).c_str(), s); if (bt < 0) { printf("  -> NO FIT\n"); done = true; } else { printf("\n"); c = best; } }
        }
        if (done) return;
        if (found) continue;
        for (int p = 0; p < H && !found;) { int s = cell(c, p); if (!s) { p++; continue; }
            if (!(G == 'O' && s != 1)) { u64 c2 = c & ~(PAT[s] << (2 * p)); int t = dt[idx(c2)];
                if (t != 0 && t < T) { printf("%s  free %d@%d\n", show(c).c_str(), s, p); c = c2; found = true; } }
            p += s; }
        if (!found) { printf("stuck\n"); return; }
    }
}
int main(int argc, char** argv) {
    G = argv[1][0];
    if (string(argv[2]) == "table") {
        int lo = atoi(argv[3]), hi = atoi(argv[4]); if (argc > 5) SZ = atoi(argv[5]);
        int h = lo;
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
    size_t ns = 0; for (char x : safe_) ns += x;
    printf("G=%c M=%d H=%d sizes=%d states=%zu safe=%zu sweeps=%d %s\n", G, M, H, SZ, st.size(), ns, sweeps, w ? "WIN" : "LOSE");
    if (argc > 5 && string(argv[5]) == "pv") { pv(); return 0; }
    if (argc > 5) {
        FILE* f = fopen(argv[5], "w");
        for (size_t i = 0; i < st.size(); i++) if (safe_[i]) {
            for (int p = 0; p < H; p++) fputc(".123"[cell(st[i], p)], f);
            fputc('\n', f);
        }
        fclose(f);
    }
    return 0;
}
