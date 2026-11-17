// Fast BFS over all reachable states of the charged-pebble game.
// Usage: fast p k N [witnessfile]
// State = multiset of p pebbles (pos,charge), pos=0 means in hand. Packed 12 bits each
// (pos<<4|charge), sorted descending -> canonical (equal-charge pebbles interchangeable).
// Board truncated at node N; harmless iff reported maxnode < N.
// Prints: p k N maxclean maxnode nstates
// Witness file: one move per line "P i c" / "R i c" / "F i c" (c = charge before the move).
#include <bits/stdc++.h>
using namespace std;
typedef uint64_t u64;
int p, k, N;
vector<u64> states;
vector<uint32_t> par;
vector<uint32_t> table; // open addressing, stores index+1
u64 mask;
static inline u64 hsh(u64 x) { x ^= x >> 33; x *= 0xff51afd7ed558ccdULL; x ^= x >> 33; x *= 0xc4ceb9fe1a85ec53ULL; x ^= x >> 33; return x; }
void grow() {
    size_t ns = table.size() * 2; table.assign(ns, 0); mask = ns - 1;
    for (size_t i = 0; i < states.size(); i++) { u64 h = hsh(states[i]) & mask; while (table[h]) h = (h + 1) & mask; table[h] = i + 1; }
}
bool insert(u64 s, uint32_t parent) {
    u64 h = hsh(s) & mask;
    while (table[h]) { if (states[table[h] - 1] == s) return false; h = (h + 1) & mask; }
    states.push_back(s); par.push_back(parent); table[h] = states.size();
    if (states.size() * 2 > table.size()) grow();
    return true;
}
static inline u64 packs(int *a) { // a: p values of 12 bits; sort descending
    sort(a, a + p, greater<int>()); u64 s = 0; for (int i = 0; i < p; i++) s = (s << 12) | (u64)a[i]; return s;
}
static inline void unpack(u64 s, int *a) { for (int i = p - 1; i >= 0; i--) { a[i] = s & 4095; s >>= 12; } }
int main(int argc, char **argv) {
    p = atoi(argv[1]); k = atoi(argv[2]); N = atoi(argv[3]);
    if (p > 5 || k > 15 || N > 255) { fprintf(stderr, "range\n"); return 2; }
    table.assign(1 << 20, 0); mask = table.size() - 1;
    int a[8], b[8];
    for (int i = 0; i < p; i++) a[i] = k;
    insert(packs(a), 0xffffffffu);
    int best = 0, maxnode = 0; uint32_t bestidx = 0;
    vector<signed char> occ(N + 3);
    for (size_t idx = 0; idx < states.size(); idx++) {
        u64 s = states[idx]; unpack(s, a);
        fill(occ.begin(), occ.end(), -1);
        int nb = 0, top = 0;
        for (int i = 0; i < p; i++) { int pos = a[i] >> 4; if (pos) { occ[pos] = a[i] & 15; nb++; top = max(top, pos); } }
        if (top > maxnode) maxnode = top;
        if (nb == 1 && top > best) { best = top; bestidx = idx; }
        // removals
        for (int i = 0; i < p; i++) {
            int pos = a[i] >> 4, c = a[i] & 15; if (!pos) continue;
            bool predp = (pos == 1) || occ[pos - 1] >= 0;
            int nc;
            if (predp) nc = c; else if (c >= 1) nc = c - 1; else continue;
            memcpy(b, a, sizeof(int) * p); b[i] = nc; insert(packs(b), idx);
        }
        // placements
        int lastc = -1;
        for (int i = 0; i < p; i++) {
            int pos = a[i] >> 4, c = a[i] & 15; if (pos) continue; if (c == lastc) continue; lastc = c;
            for (int t = 1; t <= N; t++) {
                if (occ[t] >= 0) continue;
                if (!(t == 1 || occ[t - 1] >= 0)) continue;
                memcpy(b, a, sizeof(int) * p); b[i] = (t << 4) | c; insert(packs(b), idx);
            }
        }
    }
    printf("%d %d %d %d %d %zu\n", p, k, N, best, maxnode, states.size());
    if (argc > 4) {
        vector<uint32_t> path; for (uint32_t x = bestidx; x != 0xffffffffu; x = par[x]) path.push_back(x);
        reverse(path.begin(), path.end());
        FILE *f = fopen(argv[4], "w");
        fprintf(f, "# p=%d k=%d clean at %d ; moves: P/R/F node charge_before\n", p, k, best);
        for (size_t j = 1; j < path.size(); j++) {
            // diff the two states as multisets
            int x[8], y[8]; unpack(states[path[j - 1]], x); unpack(states[path[j]], y);
            multiset<int> X(x, x + p), Y(y, y + p);
            for (int i = 0; i < p; i++) { auto it = X.find(y[i]); if (it != X.end()) { X.erase(it); Y.erase(Y.find(y[i])); } }
            int from = *X.begin(), to = *Y.begin();
            if ((from >> 4) == 0) fprintf(f, "P %d %d\n", to >> 4, from & 15);
            else if ((from & 15) == (to & 15)) fprintf(f, "R %d %d\n", from >> 4, from & 15);
            else fprintf(f, "F %d %d\n", from >> 4, from & 15);
        }
        fclose(f);
    }
    if (argc > 5) { // certificate: every reachable state, format of check_cert.py
        FILE *f = fopen(argv[5], "w");
        for (u64 s : states) { int x[8]; unpack(s, x);
            for (int i = p - 1; i >= 0; i--) if (x[i] >> 4) fprintf(f, "%d:%d ", x[i] >> 4, x[i] & 15);
            fprintf(f, "|");
            for (int i = 0; i < p; i++) if (!(x[i] >> 4)) fprintf(f, " %d", x[i] & 15);
            fprintf(f, "\n"); }
        fclose(f);
    }
    return 0;
}
