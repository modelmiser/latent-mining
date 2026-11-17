// Referee's independent solver. Written from the game definition only.
// usage: v3 G M H [dumpfile]   G in N,F,O.  Prints WIN/LOSE for manager confined to cells 0..H-1.
// Layout: per cell 0 free, 1/2/3 = first cell of a block of that size, 4 = later cell of a block.
// Greatest fixpoint: start with all layouts (live<=M) "safe", delete until stable, WIN iff empty is safe.
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int M, H; char G;
typedef array<unsigned char, 28> Lay;
u64 key(const Lay& c) { u64 k = 0; for (int i = H - 1; i >= 0; i--) k = k * 5 + c[i]; return k; }
vector<Lay> all; unordered_map<u64, int> idx; vector<char> safe;
void gen(Lay& c, int pos, int live) {
    if (pos == H) { idx[key(c)] = all.size(); all.push_back(c); return; }
    c[pos] = 0; gen(c, pos + 1, live);
    for (int s = 1; s <= 3; s++) if (pos + s <= H && live + s <= M) {
        c[pos] = s; for (int j = 1; j < s; j++) c[pos + j] = 4;
        gen(c, pos + s, live + s);
        for (int j = 0; j < s; j++) c[pos + j] = 0;
    }
}
bool isSafe(const Lay& c) { return safe[idx[key(c)]]; }
bool fits(const Lay& c, int p, int s) { if (p + s > H) return false; for (int j = 0; j < s; j++) if (c[p + j]) return false; return true; }
void put(Lay& c, int p, int s) { c[p] = s; for (int j = 1; j < s; j++) c[p + j] = 4; }
void lift(Lay& c, int p) { int s = c[p]; for (int j = 0; j < s; j++) c[p + j] = 0; }
// F: can the manager reach a safe layout from c using moves of total moved size <= budget?
bool reachSafe(Lay c, int budget) {
    if (isSafe(c)) return true;
    if (budget <= 0) return false;
    for (int p = 0; p < H; p++) {
        int s = c[p];
        if (s >= 1 && s <= 2 && s <= budget) {
            Lay d = c; lift(d, p);
            for (int q = 0; q < H; q++) if (q != p && fits(d, q, s)) {
                Lay e = d; put(e, q, s);
                if (reachSafe(e, budget - s)) return true;
            }
        }
    }
    return false;
}
bool check(const Lay& c) {
    int live = 0; for (int i = 0; i < H; i++) if (c[i]) live++;
    for (int s = 1; s <= 3; s++) if (live + s <= M) {
        bool ok = false;
        for (int p = 0; p + s <= H && !ok; p++) if (fits(c, p, s)) { Lay d = c; put(d, p, s); if (isSafe(d)) ok = true; }
        if (!ok) return false;
    }
    for (int p = 0; p < H; p++) {
        int s = c[p];
        if (s < 1 || s > 3) continue;
        if (G == 'O' && s != 1) continue;
        Lay d = c; lift(d, p);
        if (G == 'F') { if (!reachSafe(d, s - 1)) return false; }
        else if (!isSafe(d)) return false;
    }
    return true;
}
int main(int argc, char** argv) {
    G = argv[1][0]; M = atoi(argv[2]); H = atoi(argv[3]);
    Lay c; c.fill(0); gen(c, 0, 0);
    safe.assign(all.size(), 1);
    bool changed = true; int sweeps = 0;
    while (changed) {
        changed = false; sweeps++;
        for (size_t i = 0; i < all.size(); i++) if (safe[i] && !check(all[i])) { safe[i] = 0; changed = true; }
        Lay z; z.fill(0); if (!isSafe(z)) break;
    }
    Lay z; z.fill(0); bool win = isSafe(z);
    printf("G=%c M=%d H=%d layouts=%zu sweeps=%d %s\n", G, M, H, all.size(), sweeps, win ? "WIN" : "LOSE");
    if (win && argc > 4) { // dump safe set (complete fixpoint, since loop ran to stability)
        FILE* f = fopen(argv[4], "w");
        for (size_t i = 0; i < all.size(); i++) if (safe[i]) { for (int j = 0; j < H; j++) fputc('0' + all[i][j], f); fputc('\n', f); }
        fclose(f);
    }
    return 0;
}
