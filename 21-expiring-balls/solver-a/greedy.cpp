// Tests ONE fixed Rule strategy against every FIFO Adversary: exhaustive search of all
// states reachable when Rule plays "give the ball to the endpoint with the smaller load;
// on a tie use TIE".  Reports SAFE (no load >= L reachable) or a losing line.
// TIE: 0 = bin that appears earlier in the age-ordered list of present balls (untouched bins last)
//      1 = bin that appears later (untouched bins first)
//      2 = bin in the smaller component, then as 0     3 = bin in the larger component, then as 0
// Usage: greedy n L TIE [log2 table size]
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 u128;
int n, L, TIE;
struct St { int m; unsigned char w[16], o[16]; };
static u128 enc(const St& s) { u128 k = 0; for (int i = 0; i < s.m; i++) k |= (u128)(s.w[i] * 16 + s.o[i]) << (8 * i); return k; }
static St dec(u128 k) { St s; s.m = 0; while (k & 255) { s.w[s.m] = (unsigned)(k & 255) >> 4; s.o[s.m] = (unsigned)(k & 15); s.m++; k >>= 8; } return s; }
static void canon(St& s) {
    int map[16]; for (int i = 0; i < 16; i++) map[i] = -1; int k = 0;
    for (int i = 0; i < s.m; i++) {
        if (map[s.w[i]] < 0) map[s.w[i]] = k++;
        if (map[s.o[i]] < 0) map[s.o[i]] = k++;
        s.w[i] = map[s.w[i]]; s.o[i] = map[s.o[i]];
    }
}
vector<u128> tab; vector<unsigned> par; u128 mask; size_t cnt = 0;
static inline size_t hsh(u128 k) { unsigned long long x = (unsigned long long)k ^ ((unsigned long long)(k >> 64) * 0x9E3779B97F4A7C15ULL); x ^= x >> 31; x *= 0x9E3779B97F4A7C15ULL; x ^= x >> 29; x *= 0xBF58476D1CE4E5B9ULL; x ^= x >> 32; return x; }
static long add(u128 k) { size_t h = hsh(k) & (size_t)mask; while (tab[h]) { if (tab[h] == k + 1) return -1; h = (h + 1) & (size_t)mask; } tab[h] = k + 1; cnt++; return h; }
static string show(const St& s) { string r; for (int i = 0; i < s.m; i++) { if (i) r += ' '; r += to_string(s.w[i]) + ":" + to_string(s.o[i]); } return r; }
int main(int argc, char** argv) {
    n = atoi(argv[1]); L = atoi(argv[2]); TIE = atoi(argv[3]);
    size_t cap = (size_t)1 << (argc > 4 ? atoi(argv[4]) : 24); tab.assign(cap, 0); mask = cap - 1;
    vector<u128> stack; stack.push_back(0); add(0);
    while (!stack.empty()) {
        if (cnt > cap * 0.75) { printf("n=%d L=%d TIE=%d UNDECIDED table full at %zu states\n", n, L, TIE, cnt); return 4; }
        St s = dec(stack.back()); stack.pop_back();
        if (s.m) { St t; t.m = s.m - 1; for (int i = 1; i < s.m; i++) { t.w[i - 1] = s.w[i]; t.o[i - 1] = s.o[i]; } canon(t); u128 e = enc(t); if (add(e) >= 0) stack.push_back(e); }
        int k = 0, load[18] = {0}, c[18], sz[18] = {0};
        for (int i = 0; i < s.m; i++) { k = max(k, (int)s.w[i] + 1); k = max(k, (int)s.o[i] + 1); load[s.w[i]]++; }
        for (int i = 0; i < 18; i++) c[i] = i;
        for (int i = 0; i < s.m; i++) { int a = c[s.w[i]], b = c[s.o[i]]; if (a != b) for (int x = 0; x < k; x++) if (c[x] == a) c[x] = b; }
        for (int i = 0; i < k + 2; i++) sz[c[i]]++;
        int top = min(n, k + 2);
        for (int a = 0; a < top; a++) for (int b = a + 1; b < top; b++) {
            if (a < k && b < k && c[a] == c[b]) continue;
            if (a < k && b > k) continue;
            int w;   // receiver
            if (load[a] != load[b]) w = load[a] < load[b] ? a : b;
            else if (TIE == 0) w = a;
            else if (TIE == 1) w = b;
            else if (TIE == 2) w = sz[c[a]] <= sz[c[b]] ? a : b;
            else w = sz[c[a]] >= sz[c[b]] ? a : b;
            St t = s; t.w[t.m] = w; t.o[t.m] = a + b - w; t.m++;
            if (load[w] + 1 >= L) { printf("n=%d L=%d TIE=%d LOSES: offer {%d,%d} at state [%s] gives load %d (states seen %zu)\n", n, L, TIE, a, b, show(s).c_str(), load[w] + 1, cnt); return 1; }
            canon(t); u128 e = enc(t); if (add(e) >= 0) stack.push_back(e);
        }
    }
    printf("n=%d L=%d TIE=%d SAFE: reachable states=%zu, no load >= %d\n", n, L, TIE, cnt, L);
    return 0;
}
