// FIFO game with a HANDICAP on Adversary: an Offer is only allowed while fewer than CAP
// balls are present. A win for the handicapped Adversary is a win in the real game, so
// "adversary_wins=1" is a sound UPPER bound witness; "adversary_wins=0" proves nothing.
// Same state encoding as fast.cpp (bins named by first appearance, balls oldest first).
// States with CAP balls are not stored: such a state is terminal (load >= L) or its only
// move is Expire.  Usage: capfifo n L CAP [strategyfile]
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int n, L, CAP;
struct St { int m; unsigned char w[8], o[8]; };
static u64 enc(const St& s) { u64 k = 0; for (int i = 0; i < s.m; i++) k |= (u64)(s.w[i] * 16 + s.o[i]) << (8 * i); return k; } // w!=o so no zero byte
static St dec(u64 k) { St s; s.m = 0; while (k & 255) { s.w[s.m] = (k & 255) >> 4; s.o[s.m] = k & 15; s.m++; k >>= 8; } return s; }
static void canon(St& s) {
    int map[16]; for (int i = 0; i < 16; i++) map[i] = -1; int k = 0;
    for (int i = 0; i < s.m; i++) {
        if (map[s.w[i]] < 0) map[s.w[i]] = k++;
        if (map[s.o[i]] < 0) map[s.o[i]] = k++;
        s.w[i] = map[s.w[i]]; s.o[i] = map[s.o[i]];
    }
}
static int maxload(const St& s) { int l[16] = {0}, b = 0; for (int i = 0; i < s.m; i++) b = max(b, ++l[s.w[i]]); return b; }
static u64 expire(const St& s) { St t; t.m = s.m - 1; for (int i = 1; i < s.m; i++) { t.w[i - 1] = s.w[i]; t.o[i - 1] = s.o[i]; } canon(t); return enc(t); }
// open addressing table: key+1 stored (0 = empty)
vector<u64> tab; vector<u64> order; vector<int> tidx; u64 mask;
static inline u64 hsh(u64 k) { k ^= k >> 31; k *= 0x9E3779B97F4A7C15ULL; k ^= k >> 29; k *= 0xBF58476D1CE4E5B9ULL; k ^= k >> 32; return k; }
static int find(u64 k) { u64 h = hsh(k) & mask; while (tab[h]) { if (tab[h] == k + 1) return tidx[h]; h = (h + 1) & mask; } return -1; }
static int add(u64 k) { u64 h = hsh(k) & mask; while (tab[h]) { if (tab[h] == k + 1) return tidx[h]; h = (h + 1) & mask; } tab[h] = k + 1; tidx[h] = order.size(); order.push_back(k); return tidx[h]; }
struct Opt { u64 s[2]; int desc; };
static void offers(const St& s, vector<Opt>& out) {
    out.clear();
    int k = 0; for (int i = 0; i < s.m; i++) { k = max(k, (int)s.w[i] + 1); k = max(k, (int)s.o[i] + 1); }
    int c[16]; for (int i = 0; i < 16; i++) c[i] = i;
    for (int i = 0; i < s.m; i++) { int a = c[s.w[i]], b = c[s.o[i]]; if (a != b) for (int x = 0; x < k; x++) if (c[x] == a) c[x] = b; }
    int top = min(n, k + 2);
    for (int a = 0; a < top; a++) for (int b = a + 1; b < top; b++) {
        if (a < k && b < k && c[a] == c[b]) continue;
        if (a < k && b > k) continue;
        St t = s; t.w[t.m] = a; t.o[t.m] = b; t.m++; St t2 = s; t2.w[t2.m] = b; t2.o[t2.m] = a; t2.m++;
        canon(t); canon(t2);
        out.push_back({{enc(t), enc(t2)}, a * 16 + b});
    }
}
vector<unsigned char> win; vector<unsigned short> rk;
// value of a state that may have CAP balls: returns rank+1 if winning (0 if not)
static int val(u64 k) {
    St s = dec(k);
    if (s.m < CAP) { int i = find(k); return win[i] ? rk[i] + 1 : 0; }
    if (maxload(s) >= L) return 1;
    int i = find(expire(s)); return win[i] ? rk[i] + 2 : 0;
}
static string show(u64 k) { St s = dec(k); string r; for (int i = 0; i < s.m; i++) { if (i) r += ' '; r += to_string(s.w[i]) + ":" + to_string(s.o[i]); } return r; }
int main(int argc, char** argv) {
    n = atoi(argv[1]); L = atoi(argv[2]); CAP = atoi(argv[3]);
    if (CAP > 8 || n > 15) return 3;
    size_t cap = (size_t)1 << (argc > 5 ? atoi(argv[5]) : 26); tab.assign(cap, 0); tidx.assign(cap, 0); mask = cap - 1;
    add(0);
    vector<Opt> opts;
    for (size_t i = 0; i < order.size(); i++) {
        if (order.size() > cap * 0.7) { fprintf(stderr, "table full\n"); return 4; }
        St s = dec(order[i]);
        if (maxload(s) >= L) continue;
        if (s.m) add(expire(s));
        offers(s, opts);
        for (auto& o : opts) for (int j = 0; j < 2; j++) {
            St t = dec(o.s[j]);
            if (t.m < CAP) add(o.s[j]); else if (maxload(t) < L) add(expire(t));
        }
    }
    size_t N = order.size();
    win.assign(N, 0); rk.assign(N, 0); vector<int> mv(N, -2);
    for (size_t i = 0; i < N; i++) if (maxload(dec(order[i])) >= L) win[i] = 1;
    bool changed = true; int sweeps = 0;
    while (changed && !win[0]) {
        changed = false; sweeps++;
        for (size_t i = N; i-- > 0;) {
            if (win[i]) continue;
            St s = dec(order[i]);
            if (s.m) { int e = find(expire(s)); if (win[e]) { win[i] = 1; rk[i] = rk[e] + 1; mv[i] = -1; changed = true; continue; } }
            offers(s, opts);
            for (auto& o : opts) {
                int a = val(o.s[0]); if (!a) continue; int b = val(o.s[1]); if (!b) continue;
                win[i] = 1; rk[i] = max(a, b); mv[i] = o.desc; changed = true; break;
            }
        }
        fprintf(stderr, "sweep %d\n", sweeps);
    }
    size_t nw = 0; for (size_t i = 0; i < N; i++) nw += win[i];
    printf("n=%d L=%d CAP=%d stored_states=%zu handicapped_adversary_wins=%d winstates=%zu sweeps=%d rank0=%d\n", n, L, CAP, N, (int)win[0], nw, sweeps, (int)rk[0]);
    if (argc > 4 && win[0]) {
        FILE* f = fopen(argv[4], "w");
        fprintf(f, "# ADVERSARY strategy n=%d L=%d mode=fifo (never offers with %d balls present). line: rank|move|state ; same format as adv_n7_L3.txt\n", n, L, CAP);
        unordered_set<u64> seen; vector<u64> q; q.push_back(0); seen.insert(0); size_t lines = 0;
        for (size_t h = 0; h < q.size(); h++) {
            u64 k = q[h]; St s = dec(k); lines++;
            if (maxload(s) >= L) { fprintf(f, "0|T|%s\n", show(k).c_str()); continue; }
            if (s.m == CAP) { u64 e = expire(s); fprintf(f, "%d|E|%s\n", val(k) - 1, show(k).c_str()); if (seen.insert(e).second) q.push_back(e); continue; }
            int i = find(k);
            if (mv[i] == -1) { u64 e = expire(s); fprintf(f, "%d|E|%s\n", (int)rk[i], show(k).c_str()); if (seen.insert(e).second) q.push_back(e); continue; }
            offers(s, opts);
            for (auto& o : opts) if (o.desc == mv[i]) {
                fprintf(f, "%d|O %d %d|%s\n", (int)rk[i], o.desc >> 4, o.desc & 15, show(k).c_str());
                for (int j = 0; j < 2; j++) if (seen.insert(o.s[j]).second) q.push_back(o.s[j]);
            }
        }
        fclose(f); printf("strategy lines=%zu\n", lines);
    }
    return 0;
}
