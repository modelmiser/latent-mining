// Exhaustive test of a FIXED deterministic manager rule against every adversary (games N and O).
// usage: rules G rule Mlo Mhi   -> least H at which the rule never fails, for each M
// rules: 0 first fit; 1 best fit (smallest gap, leftmost); 2 = 1s leftmost, 2s and 3s rightmost;
//        3 = 1s,2s leftmost, 3s rightmost; 4 = 1s,3s leftmost, 2s rightmost; 5 last fit (deliberately
//        symmetric to 0, must give the same numbers); 6 best fit, exact gaps first then LARGEST gap leftmost;
//        7 deliberately bad: always the middle-most free position.
#include <bits/stdc++.h>
using namespace std;
typedef uint64_t u64;
static int H, M, R; static char G;
static const u64 PAT[4] = {0, 1, 10, 63};
static inline int cell(u64 c, int p) { return (c >> (2 * p)) & 3; }
static int place(u64 c, int s) {
    vector<int> pos; vector<int> gl;  // feasible starts and the length of the gap containing them
    for (int p = 0; p < H;) {
        if (cell(c, p)) { p++; continue; }
        int q = p; while (q < H && !cell(c, q)) q++;
        for (int x = p; x + s <= q; x++) { pos.push_back(x); gl.push_back(q - p); }
        p = q;
    }
    if (pos.empty()) return -1;
    bool right = (R == 5) || (R == 2 && s >= 2) || (R == 3 && s == 3) || (R == 4 && s == 2);
    if (R == 1 || R == 6) {
        int b = 0;
        for (size_t i = 1; i < pos.size(); i++) {
            if (R == 1 && gl[i] < gl[b]) b = i;
            if (R == 6) { bool ei = gl[i] == s, eb = gl[b] == s; if ((ei && !eb) || (!ei && !eb && gl[i] > gl[b])) b = i; }
        }
        return pos[b];
    }
    if (R == 7) { int b = 0; for (size_t i = 1; i < pos.size(); i++) if (abs(2 * pos[i] + s - H) < abs(2 * pos[b] + s - H)) b = i; return pos[b]; }
    return right ? pos.back() : pos[0];
}
static bool survives() {
    unordered_set<u64> seen; vector<u64> todo; seen.insert(0); todo.push_back(0);
    while (!todo.empty()) {
        u64 c = todo.back(); todo.pop_back();
        int live = 0; for (int p = 0; p < H; p++) if (cell(c, p)) live++;
        for (int s = 1; s <= 3; s++) if (live + s <= M) {
            int p = place(c, s); if (p < 0) return false;
            u64 c2 = c | (PAT[s] << (2 * p)); if (seen.insert(c2).second) todo.push_back(c2);
        }
        for (int p = 0; p < H;) { int s = cell(c, p); if (!s) { p++; continue; }
            if (!(G == 'O' && s != 1)) { u64 c2 = c & ~(PAT[s] << (2 * p)); if (seen.insert(c2).second) todo.push_back(c2); }
            p += s; }
    }
    return true;
}
int main(int argc, char** argv) {
    G = argv[1][0]; R = atoi(argv[2]); int lo = atoi(argv[3]), hi = atoi(argv[4]);
    for (M = lo; M <= hi; M++) {
        for (H = M; !survives(); H++);
        printf("G=%c rule=%d M=%d H=%d\n", G, R, M, H); fflush(stdout);
    }
}
