// Referee's definition-level solver. No flag abstraction.
// A knowledge state is: where every arrived item (named by ARRIVAL INDEX) sits, how many
// have arrived, how many have been output, and the EXPLICIT set of arrival orders
// (permutations) consistent with all answers so far.  Value = max number of orders in the
// set that a continuation beats.  Attempts whose answer is the same for every order in the
// set AND change nothing (illegal IN/SHIFT, OUT on empty stack, OUT refused for all) are
// no-ops and skipped.  Memo key = 128-bit hash of (positions, counters, full sorted set).
// usage: defsolve k n
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64; typedef unsigned int u32;
int K, N; vector<array<unsigned char,10>> P; // P[p][arrival index] = value (1..N)
struct Cfg { unsigned char st[6][2]; unsigned char cnt[6]; unsigned char arrived, m; };
struct H { u64 a, b; bool operator==(const H&o) const { return a==o.a && b==o.b; } };
struct HH { size_t operator()(const H&h) const { return h.a; } };
unordered_map<H, u32, HH> memo;
u64 nstates = 0, maxset = 0;
static inline u64 mix(u64 x){ x ^= x>>30; x *= 0xbf58476d1ce4e5b9ULL; x ^= x>>27; x *= 0x94d049bb133111ebULL; x ^= x>>31; return x; }
H key(const Cfg& c, const vector<u32>& S) {
    u64 a = 0x1234567, b = 0xabcdef01;
    auto feed = [&](u64 v){ a = mix(a ^ (v + 0x9e3779b97f4a7c15ULL)); b = mix(b*31 + v + 0x7f4a7c15ULL) ^ (b>>7); };
    for (int i = 0; i < K; i++) { feed(c.cnt[i]); for (int j = 0; j < c.cnt[i]; j++) feed(100 + c.st[i][j]); }
    feed(1000 + c.arrived); feed(2000 + c.m); feed(S.size());
    for (u32 p : S) feed(p);
    return H{a, b};
}

// ---- independent flag DP (referee's own), used ONLY to test the reduction lemma at every definition-level state
map<vector<int>, u64> fmemo;
u64 flagV(vector<int> s) { // s = [cnt_1, f_1b, f_1t, ..., cnt_K, f_Kb, f_Kt, u]
    auto it = fmemo.find(s); if (it != fmemo.end()) return it->second;
    int u = s[3*K]; bool empty = true; for (int i = 0; i < K; i++) if (s[3*i]) empty = false;
    u64 best = 0;
    if (empty && u == 0) best = 1;
    else {
        if (u > 0 && s[0] < 2) { auto t = s; t[1 + t[0]] = 0; t[0]++; t[3*K]--; best = max(best, flagV(t)); }
        for (int i = 0; i + 1 < K; i++) if (s[3*i] > 0 && s[3*i+3] < 2) {
            auto t = s; int fl = t[3*i + t[3*i]]; t[3*i + t[3*i]] = 0; t[3*i]--; t[3*i+3]++; t[3*i+3 + t[3*i+3]] = fl; best = max(best, flagV(t)); }
        for (int i = 0; i < K; i++) if (s[3*i] > 0 && s[3*i + s[3*i]] == 0) {
            auto t = s; t[3*i + t[3*i]] = 1; u64 v = flagV(t);
            auto w = s; w[3*i + w[3*i]] = 0; w[3*i]--; for (int j = 0; j < K; j++) w[3*j+1] = w[3*j+2] = 0; v += flagV(w);
            best = max(best, v); }
    }
    fmemo[s] = best; return best;
}
u64 nchecked = 0, nmism = 0, nsetmism = 0;
u64 fact(int x){ u64 r = 1; for (int i = 2; i <= x; i++) r *= i; return r; }
void lemma_check(const Cfg& c, const vector<u32>& S, u32 val) {
    vector<int> s(3*K + 1, 0); int t = N - c.arrived, f = 0;
    for (int i = 0; i < K; i++) { s[3*i] = c.cnt[i]; t += c.cnt[i];
        for (int j = 0; j < c.cnt[i]; j++) { bool can = false; unsigned char x = c.st[i][j];
            for (u32 p : S) if (P[p][x] == c.m) { can = true; break; }
            s[3*i+1+j] = can ? 0 : 1; f += !can; } }
    s[3*K] = N - c.arrived;
    nchecked++;
    if (S.size() != (u64)(t - f) * fact(t - 1)) nsetmism++;   // consistent set = all bijections with m off flagged items
    if (flagV(s) != val) { nmism++; if (nmism < 5) fprintf(stderr, "LEMMA MISMATCH flagV=%llu def=%u\n", flagV(s), val); }
}
u32 solve(const Cfg& c, const vector<u32>& S) {
    if (S.empty()) return 0;
    if (c.m == N + 1) { if (S.size() != 1) { fprintf(stderr, "BUG terminal set size %zu\n", S.size()); exit(2); } return 1; }
    H h = key(c, S);
    auto it = memo.find(h);
    if (it != memo.end()) return it->second;
    nstates++; if (S.size() > maxset) maxset = S.size();
    u32 best = 0;
    // IN
    if (c.cnt[0] < 2 && c.arrived < N) {
        Cfg d = c; d.st[0][d.cnt[0]++] = d.arrived; d.arrived++;
        best = max(best, solve(d, S));
    }
    // SHIFT i
    for (int i = 0; i + 1 < K; i++) if (c.cnt[i] > 0 && c.cnt[i+1] < 2) {
        Cfg d = c; unsigned char x = d.st[i][--d.cnt[i]]; d.st[i+1][d.cnt[i+1]++] = x;
        best = max(best, solve(d, S));
    }
    // OUT i : answer depends on the order; split the set
    for (int i = 0; i < K; i++) if (c.cnt[i] > 0) {
        unsigned char x = c.st[i][c.cnt[i]-1];
        vector<u32> yes, no;
        for (u32 p : S) (P[p][x] == c.m ? yes : no).push_back(p);
        if (yes.empty()) continue;            // refused for every consistent order: no-op
        Cfg d = c; d.cnt[i]--; d.m++;
        u32 v = solve(d, yes);
        if (!no.empty()) v += solve(c, no);   // same positions, smaller set
        best = max(best, v);
    }
    memo[h] = best;
    lemma_check(c, S, best);
    return best;
}
int main(int argc,char**argv){ K=atoi(argv[1]); int nmax=atoi(argv[2]); printf("%d:",K); for(N=1;N<=nmax;N++){ vector<int> z(3*K+1,0); z[3*K]=N; printf(" %llu",flagV(z)); } printf("\n"); }
