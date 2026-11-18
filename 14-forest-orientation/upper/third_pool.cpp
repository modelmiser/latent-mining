// Theorem 5 on larger trees, by a pool search (not exhaustive).  Quantities Phi, gamma are in THIRDS.
// Pool[n] holds up to K trees per value of H (the largest h in the tree).  Every pooled tree is built from
// single vertices by rounds of rule L: for the named pair, flip a shortest directed path from an endpoint x
// with 2 h(x) + eps(x) <= 2 h(y) + eps(y) and direct the new edge from x to y.  The search is NOT exhaustive in
// two ways: only K trees are kept per size and H, and for a pair of pooled trees only one endpoint is tried
// per class of vertices with the same h and slots (function reps), although vertices of one class can differ
// in their in-neighbours and out-trees.  For each endpoint pair tried, both endpoints (when both qualify) and
// every shortest path are generated.  On every generated round the program checks
//     Phi(new tree) - Phi(A) - Phi(B) >= 3 * cost of the round            (Lemmas 7 and 8 together)
// and on every generated tree
//     Phi(T) <= n - max gamma        and        3 * (total cost of building T) <= n - max gamma.
// It prints, for each n, the largest total cost found (a lower bound on the worst case of rule L) and
// floor((n-1)/3).  Exit code 1 on any violation.
// usage: third_pool NMAX K   (NMAX >= 2, K >= 1)      build: g++ -O2 -std=c++17 -o third_pool third_pool.cpp
#include <bits/stdc++.h>
#include <sys/resource.h>
using namespace std;
struct Tree { vector<array<int,2>> out; int cost = 0; vector<int> h; int H = 0; uint64_t sig = 0; long score2 = 0; long phi = 0; long gmax = 0; };
static int hrec(Tree& t, int x) {
    if (t.h[x] >= 0) return t.h[x];
    if (t.out[x][1] < 0) return t.h[x] = 0;
    return t.h[x] = 1 + min(hrec(t, t.out[x][0]), hrec(t, t.out[x][1]));
}
static uint64_t mix(uint64_t x) { x ^= x >> 33; x *= 0xff51afd7ed558ccdull; x ^= x >> 33; x *= 0xc4ceb9fe1a85ec53ull; return x ^ (x >> 33); }
static array<int,2> slots(const Tree& t, int x) {        // h of the out-neighbours, -1 for a missing one, sorted
    int a = t.out[x][0] < 0 ? -1 : t.h[t.out[x][0]], b = t.out[x][1] < 0 ? -1 : t.h[t.out[x][1]];
    if (a > b) swap(a, b); return {a, b};
}
static int eps(const Tree& t, int x) { auto s = slots(t, x); return s[0] != s[1]; }
static int code(const Tree& t, int x) { return 2 * t.h[x] + eps(t, x); }
static uint64_t vsig(const Tree& t, int x) { auto s = slots(t, x); return mix(((uint64_t)(t.h[x] + 1) << 40) | ((uint64_t)(s[0] + 1) << 20) | (uint64_t)(s[1] + 1)); }
static long weight(const Tree& t, int x) {               // the table of the note
    int c1 = t.out[x][0], c2 = t.out[x][1];
    if (c1 < 0) return 0;
    if (c2 < 0) return t.h[c1] == 0 ? eps(t, c1) : 1;
    if (t.h[c1] > t.h[c2]) swap(c1, c2);
    long p = t.h[c1], q = t.h[c2], ep = eps(t, c1), eq = eps(t, c2);
    if (q == p) return 3 * p - 1 + ep + eq;
    if (q == p + 1) return 3 * p + 1 + ep + eq;
    return 3 * p + 2 + ep;
}
static void finish(Tree& t) {
    int n = t.out.size(); t.h.assign(n, -1); t.H = 0;
    for (int x = 0; x < n; x++) t.H = max(t.H, hrec(t, x));
    uint64_t s = 0; long c1 = 0, c2 = 0; t.phi = 0; t.gmax = 0;
    for (int x = 0; x < n; x++) {
        s += vsig(t, x); if (t.h[x] == t.H) c1++; if (t.h[x] == t.H - 1) c2++;
        t.phi += weight(t, x); t.gmax = max(t.gmax, 3L * t.h[x] + 1 + eps(t, x));
    }
    t.sig = mix(s + n); t.score2 = c1 * 1000 + c2;
}
static void addout(Tree& t, int a, int b) { if (t.out[a][0] < 0) t.out[a][0] = b; else if (t.out[a][1] < 0) t.out[a][1] = b; else { fprintf(stderr, "internal error: vertex full\n"); exit(3); } }
static void delout(Tree& t, int a, int b) { if (t.out[a][0] == b) { t.out[a][0] = t.out[a][1]; t.out[a][1] = -1; } else if (t.out[a][1] == b) t.out[a][1] = -1; else { fprintf(stderr, "internal error: no edge\n"); exit(3); } }
static void spaths(const Tree& t, int x, vector<int>& path, vector<vector<int>>& res) {   // all shortest paths to an unsaturated vertex
    if (t.h[x] == 0) { res.push_back(path); return; }
    for (int c : t.out[x]) if (t.h[c] == t.h[x] - 1) { path.push_back(c); spaths(t, c, path, res); path.pop_back(); }
}
static int NMAX, K;
static vector<map<int, vector<Tree>>> pool;              // pool[n][H] -> trees
static long nrounds = 0, nviol = 0, ntreeviol = 0, minslack = LONG_MAX; static vector<long> byk(64, 0);
static void offer(Tree&& t) {
    int n = t.out.size(); auto& b = pool[n][t.H];
    for (auto& o : b) if (o.sig == t.sig) { if (t.cost > o.cost) o = move(t); return; }
    if ((int)b.size() < K) { b.push_back(move(t)); return; }
    int w = 0;
    for (int i = 1; i < (int)b.size(); i++) if (make_pair(b[i].cost, b[i].score2) < make_pair(b[w].cost, b[w].score2)) w = i;
    if (make_pair(b[w].cost, b[w].score2) < make_pair(t.cost, t.score2)) b[w] = move(t);
}
static vector<int> reps(const Tree& t) {                 // one vertex per class (h and slots)
    map<uint64_t,int> m; for (int x = 0; x < (int)t.out.size(); x++) m.insert({vsig(t, x), x});
    vector<int> r; for (auto& kv : m) r.push_back(kv.second); return r;
}
static void gen(const Tree& A, int p, const Tree& B, int q) {   // every reply of rule L that flips in A from p
    vector<vector<int>> ps; vector<int> path{p}; spaths(A, p, path, ps);
    int na = A.out.size();
    for (auto& s : ps) {
        Tree t; t.out = A.out;
        for (auto o : B.out) { for (int& w : o) if (w >= 0) w += na; t.out.push_back(o); }
        for (size_t i = 0; i + 1 < s.size(); i++) delout(t, s[i], s[i + 1]);
        for (size_t i = 0; i + 1 < s.size(); i++) addout(t, s[i + 1], s[i]);
        int k = (int)s.size() - 1;
        addout(t, p, q + na); t.cost = A.cost + B.cost + k; finish(t);
        long sl = t.phi - A.phi - B.phi - 3L * k; nrounds++; byk[min(k, 63)]++; minslack = min(minslack, sl);
        if (sl < 0) { if (nviol++ < 8) printf("VIOLATION of the round inequality: sizes %d + %zu, cost %d, slack %ld thirds\n", na, B.out.size(), k, sl); }
        long n = t.out.size();
        if (t.phi > n - t.gmax || 3L * t.cost > n - t.gmax) { if (ntreeviol++ < 8) printf("VIOLATION on a tree: n %ld Phi %ld max gamma %ld cost %d\n", n, t.phi, t.gmax, t.cost); }
        offer(move(t));
    }
}
int main(int argc, char** argv) {
    if (argc != 3 || (NMAX = atoi(argv[1])) < 2 || (K = atoi(argv[2])) < 1) { fprintf(stderr, "usage: third_pool NMAX K   (NMAX >= 2, K >= 1)\n"); return 2; }
    struct rlimit rl{12ull << 30, 12ull << 30}; setrlimit(RLIMIT_AS, &rl);      // hard memory limit, 12 GB
    pool.resize(NMAX + 1);
    { Tree t; t.out.push_back({-1, -1}); finish(t); offer(move(t)); }
    for (int n = 2; n <= NMAX; n++) {
        for (int a = 1; a <= n / 2; a++) {
            int b = n - a;
            for (auto& ha : pool[a]) for (auto& A : ha.second) { vector<int> ra = reps(A);
                for (auto& hb : pool[b]) for (auto& B : hb.second) { vector<int> rb = reps(B);
                    for (int p : ra) for (int q : rb) {
                        if (code(A, p) <= code(B, q)) gen(A, p, B, q);
                        if (code(B, q) <= code(A, p)) gen(B, q, A, p);
                    } } }
        }
        int best = 0; for (auto& hb : pool[n]) for (auto& t : hb.second) best = max(best, t.cost);
        printf("n %d  largest cost found %d  floor((n-1)/3) %d\n", n, best, (n - 1) / 3); fflush(stdout);
    }
    printf("rounds %ld, violations of the round inequality %ld, violations on trees %ld, least slack %ld thirds\n", nrounds, nviol, ntreeviol, minslack);
    printf("rounds by length of the flipped path:"); for (int k = 0; k < 64; k++) if (byk[k]) printf(" %d:%ld", k, byk[k]); printf("\n");
    printf(nviol || ntreeviol ? "FAILED\n" : "ALL OK\n");
    return nviol || ntreeviol ? 1 : 0;
}
