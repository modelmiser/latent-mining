// RELAXED game, fast: q uncharged pebbles, total budget of F-moves. Layered 0-1 BFS.
// Usage: relaxedc q fmax N
// State: sorted (descending) positions of the q pebbles, 8 bits each, 0 = in hand.
// Layer f = all board sets whose minimum number of F-moves is exactly f.
// Prints per layer f: maxclean(<=f) maxtop(<=f) cumulative_states ; TRUNC if a pebble sits on node N.
// Every real play with p pebbles of charge k is a relaxed play with q=p, f<=p*k,
// so relaxed maxclean at f=p*k is an UPPER bound on C(p,k).
#include <bits/stdc++.h>
using namespace std;
typedef uint64_t u64;
int q, fmax_, N;
vector<u64> states; vector<uint32_t> table; u64 mask;
static inline u64 hsh(u64 x) { x ^= x >> 33; x *= 0xff51afd7ed558ccdULL; x ^= x >> 33; x *= 0xc4ceb9fe1a85ec53ULL; x ^= x >> 33; return x; }
void grow() { size_t ns = table.size() * 2; table.assign(ns, 0); mask = ns - 1;
    for (size_t i = 0; i < states.size(); i++) { u64 h = hsh(states[i]) & mask; while (table[h]) h = (h + 1) & mask; table[h] = i + 1; } }
bool insert(u64 s) { u64 h = hsh(s) & mask;
    while (table[h]) { if (states[table[h] - 1] == s) return false; h = (h + 1) & mask; }
    states.push_back(s); table[h] = states.size(); if (states.size() * 2 > table.size()) grow(); return true; }
static inline u64 packs(int *a) { sort(a, a + q, greater<int>()); u64 s = 0; for (int i = 0; i < q; i++) s = (s << 8) | (u64)a[i]; return s; }
static inline void unpack(u64 s, int *a) { for (int i = q - 1; i >= 0; i--) { a[i] = s & 255; s >>= 8; } }
int main(int argc, char **argv) {
    q = atoi(argv[1]); fmax_ = atoi(argv[2]); N = atoi(argv[3]);
    if (q > 8 || N > 255) return 2;
    table.assign(1 << 20, 0); mask = table.size() - 1;
    int a[8], b[8]; for (int i = 0; i < q; i++) a[i] = 0;
    insert(packs(a));
    vector<u64> nextc; size_t idx = 0; int best = 0, top = 0; vector<char> occ(N + 3);
    for (int f = 0; f <= fmax_; f++) {
        for (u64 s : nextc) insert(s);
        nextc.clear();
        for (; idx < states.size(); idx++) {
            unpack(states[idx], a);
            fill(occ.begin(), occ.end(), 0); int nb = 0, tp = 0;
            for (int i = 0; i < q; i++) if (a[i]) { occ[a[i]] = 1; nb++; tp = max(tp, a[i]); }
            top = max(top, tp); if (nb == 1) best = max(best, tp);
            for (int i = 0; i < q; i++) { int pos = a[i]; if (!pos) continue;
                memcpy(b, a, sizeof(int) * q); b[i] = 0; u64 t = packs(b);
                if (pos == 1 || occ[pos - 1]) insert(t); else if (f < fmax_) nextc.push_back(t); }
            if (nb < q) { int hi = -1; for (int i = 0; i < q; i++) if (!a[i]) { hi = i; break; }
                for (int t = 1; t <= N; t++) { if (occ[t]) continue; if (!(t == 1 || occ[t - 1])) continue;
                    memcpy(b, a, sizeof(int) * q); b[hi] = t; insert(packs(b)); } }
        }
        long long formula = q >= 2 ? (long long)(f + 2) << (q - 2) : 1;
        printf("q=%d f=%d clean=%d top=%d states=%zu formula=%lld %s%s\n", q, f, best, top, states.size(), formula,
               best == formula ? "EQ" : "DIFF", top >= N ? " TRUNC" : ""); fflush(stdout);
        sort(nextc.begin(), nextc.end()); nextc.erase(unique(nextc.begin(), nextc.end()), nextc.end());
    }
}
