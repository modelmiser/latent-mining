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
    return best;
}
int main(int argc, char** argv) {
    K = atoi(argv[1]); N = atoi(argv[2]);
    array<unsigned char,10> p{}; for (int i = 0; i < N; i++) p[i] = i + 1;
    do P.push_back(p); while (next_permutation(p.begin(), p.begin() + N));
    vector<u32> S(P.size()); iota(S.begin(), S.end(), 0);
    Cfg c; memset(&c, 0, sizeof c); c.m = 1;
    u32 v = solve(c, S);
    printf("k=%d n=%d N=%u  states=%llu memo=%zu\n", K, N, v, nstates, memo.size());
    return 0;
}
