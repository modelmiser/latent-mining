// Lower bounds on G(n) = worst case of the greedy (Gupta-Kumar-Stein) rule, by a pool search.
// Pool[n] holds up to K trees per value of H (max h in the tree), each built by merging two pooled trees
// under the greedy rule (flip a shortest path from the endpoint with smaller h; ties to the Adversary).
// Every pooled tree is really reachable, so best[n] = max cost in Pool[n] is a true lower bound on G(n).
// It also tests Lemma 4 of NOTE.md (2*Phi rises by at least 2*cost) on every round it generates, and that
// cost <= Phi for every tree; exit code 4 on a violation.  This is a sample of reachable rounds, not all of them.
// usage: pool_search NMAX K     build: g++ -O2 -std=c++17 -o pool_search pool_search.cpp
#include <bits/stdc++.h>
using namespace std;
struct Tree { vector<array<int,2>> out; int cost = 0; vector<int> h; int H = 0; uint64_t sig = 0; long score2 = 0; };
static int hrec(Tree& t, int x) {
    if (t.h[x] >= 0) return t.h[x];
    if (t.out[x][1] < 0) return t.h[x] = 0;
    return t.h[x] = 1 + min(hrec(t, t.out[x][0]), hrec(t, t.out[x][1]));
}
static uint64_t mix(uint64_t x) { x ^= x >> 33; x *= 0xff51afd7ed558ccdull; x ^= x >> 33; x *= 0xc4ceb9fe1a85ec53ull; return x ^ (x >> 33); }
static uint64_t vsig(const Tree& t, int x) {            // h of x and of its children
    int a = t.out[x][0] < 0 ? -1 : t.h[t.out[x][0]], b = t.out[x][1] < 0 ? -1 : t.h[t.out[x][1]];
    if (a > b) swap(a, b);
    return mix(((uint64_t)(t.h[x] + 1) << 40) | ((uint64_t)(a + 1) << 20) | (uint64_t)(b + 1));
}
static void finish(Tree& t) {
    int n = t.out.size(); t.h.assign(n, -1); t.H = 0;
    for (int x = 0; x < n; x++) t.H = max(t.H, hrec(t, x));
    uint64_t s = 0; long c1 = 0, c2 = 0;
    for (int x = 0; x < n; x++) { s += vsig(t, x); if (t.h[x] == t.H) c1++; if (t.h[x] == t.H - 1) c2++; }
    t.sig = mix(s + n); t.score2 = c1 * 1000 + c2;
}
static void addout(Tree& t, int a, int b) { if (t.out[a][0] < 0) t.out[a][0] = b; else if (t.out[a][1] < 0) t.out[a][1] = b; else { fprintf(stderr, "full\n"); exit(3); } }
static void delout(Tree& t, int a, int b) { if (t.out[a][0] == b) { t.out[a][0] = t.out[a][1]; t.out[a][1] = -1; } else if (t.out[a][1] == b) t.out[a][1] = -1; else { fprintf(stderr, "no edge\n"); exit(3); } }
static void spaths(const Tree& t, int x, vector<int>& path, vector<vector<int>>& res) {   // all shortest paths to an unsaturated vertex
    if (t.h[x] == 0) { res.push_back(path); return; }
    for (int c : t.out[x]) if (t.h[c] == t.h[x] - 1) { path.push_back(c); spaths(t, c, path, res); path.pop_back(); }
}
static int NMAX, K; static long nmerge = 0, nviol = 0; static long byk[32]; static long minslack2 = 1000;
static long phi2(const Tree& t) {                        // 2*Phi = sum over edges x->c of min(h(c), h(x)+1)
    long s = 0; for (int x = 0; x < (int)t.out.size(); x++) for (int c : t.out[x]) if (c >= 0) s += min(t.h[c], t.h[x] + 1); return s;
}
static vector<map<int, vector<Tree>>> pool;              // pool[n][H] -> trees
static void offer(Tree&& t) {
    int n = t.out.size(); auto& b = pool[n][t.H];
    for (auto& o : b) if (o.sig == t.sig) { if (t.cost > o.cost) o = move(t); return; }
    if ((int)b.size() < K) { b.push_back(move(t)); return; }
    int w = 0;
    for (int i = 1; i < (int)b.size(); i++) if (make_pair(b[i].cost, b[i].score2) < make_pair(b[w].cost, b[w].score2)) w = i;
    if (make_pair(b[w].cost, b[w].score2) < make_pair(t.cost, t.score2)) b[w] = move(t);
}
static vector<int> reps(const Tree& t) {                 // one vertex per signature class
    map<uint64_t,int> m; for (int x = 0; x < (int)t.out.size(); x++) m.insert({vsig(t, x), x});
    vector<int> r; for (auto& kv : m) r.push_back(kv.second); return r;
}
static void mergeinto(const Tree& A, int p, const Tree& B, int q) {   // flip in A from p, new edge p -> q
    if (A.h[p] > B.h[q]) return;                                       // greedy flips from the smaller h
    vector<vector<int>> ps; vector<int> path{p}; spaths(A, p, path, ps);
    int na = A.out.size();
    for (auto& s : ps) {
        Tree t; t.out = A.out;
        for (auto o : B.out) { for (int& w : o) if (w >= 0) w += na; t.out.push_back(o); }
        for (size_t i = 0; i + 1 < s.size(); i++) delout(t, s[i], s[i + 1]);
        for (size_t i = 0; i + 1 < s.size(); i++) addout(t, s[i + 1], s[i]);
        addout(t, p, q + na); t.cost = A.cost + B.cost + (int)s.size() - 1;
        finish(t);
        { long sl = phi2(t) - phi2(A) - phi2(B) - 2 * ((long)s.size() - 1); nmerge++; byk[min<size_t>(31, s.size() - 1)]++; minslack2 = min(minslack2, sl); if (sl < 0) nviol++;
          if (2 * t.cost > phi2(t)) { fprintf(stderr, "cost exceeds Phi\n"); exit(4); } }
        offer(move(t));
    }
}
int main(int argc, char** argv) {
    if (argc < 3 || atoi(argv[1]) < 2 || atoi(argv[2]) < 1) { fprintf(stderr, "usage: pool_search NMAX K   (NMAX >= 2, K >= 1)\n"); return 1; }
    NMAX = atoi(argv[1]); K = atoi(argv[2]); pool.resize(NMAX + 1);
    { Tree t; t.out.push_back({-1, -1}); finish(t); offer(move(t)); }
    vector<int> f(NMAX + 1, 0);
    for (int n = 2; n <= NMAX; n++) {
        for (int a = 1; a <= n / 2; a++) f[n] = max(f[n], f[a] + f[n - a] + (int)floor(log2(a + 1)) - 1);
        for (int a = 1; a <= n - 1; a++) {
            int b = n - a;
            for (auto& ha : pool[a]) for (auto& A : ha.second) { vector<int> ra = reps(A);
                for (auto& hb : pool[b]) for (auto& B : hb.second) { vector<int> rb = reps(B);
                    for (int p : ra) for (int q : rb) mergeinto(A, p, B, q);
                } }
        }
        int best = 0; string per;
        for (auto& hb : pool[n]) { int m = 0; for (auto& t : hb.second) m = max(m, t.cost); best = max(best, m); per += " H" + to_string(hb.first) + ":" + to_string(m); }
        printf("%d best %d f %d  n/2 %.1f  ratio %.4f |%s\n", n, best, f[n], n / 2.0, (double)best / n, per.c_str()); fflush(stdout);
    }
    printf("# Lemma 4 on every generated round: %ld merges, violations %ld, min 2*slack %ld; by cost k:", nmerge, nviol, minslack2);
    for (int k = 0; k < 32; k++) if (byk[k]) printf(" k=%d:%ld", k, byk[k]);
    printf("\n"); return nviol ? 4 : 0;
}
