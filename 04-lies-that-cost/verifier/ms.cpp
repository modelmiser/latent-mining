// Referee: obligation-multiset solver (own code).  State = counts c[r] of alive candidates with
// remaining obligation r.  Question = how many of each class go into S.
// modes:
//   fix k n [noell0]            uncapped reachable state space, backward induction to a fixed point
//                               (exact Q or INF; not a depth cutoff)
//   td k n q [noprune]          bounded top-down search: can questioner win within q?
//   sweep k n1 n2               for each n: win at P(n)? win at P(n)-1? (and next q's if not)
//   u1 nmax                     one-lie game by DP on (a,b), compared with P(n)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
using namespace std;
typedef vector<int> St;
int K; bool NOELL0 = false, PRUNE = true, POLICY = false;

static int Pn(int n) {
    for (int q = 1;; q++) {
        long long lhs = (long long)n * (q + 1) + ((n & 1) ? (q - 1) : 0);
        if (lhs <= (1LL << q)) return q;
    }
}

// ------------------------------------------------------------------ fix mode
static void enumQ(const St& c, size_t r, St& a, vector<St>& out) {
    if (r == c.size()) { out.push_back(a); return; }
    for (int v = 0; v <= c[r]; v++) { a[r] = v; enumQ(c, r + 1, a, out); }
}
// child after answer; inS=true means answer "yes" (those in S truthful)
static St child(const St& c, const St& a, bool yes, int ell, int R) {
    St d(R + 1, 0);
    for (int r = 0; r <= R; r++) {
        int tr = yes ? a[r] : c[r] - a[r];      // truthful ones
        int li = c[r] - tr;                     // liars
        d[max(r - 1, 0)] += tr;
        if (r == 0) { int o = K * ell; if (o > R) { fprintf(stderr, "obligation overflow\n"); exit(1); } d[o] += li; }
    }
    return d;
}
static int fixmode(int n) {
    int R = K * (n / 2);
    map<St, int> id; vector<St> states;
    St root(R + 1, 0); root[0] = n;
    id[root] = 0; states.push_back(root);
    vector<vector<pair<int, int>>> trans; // per state: list of (yes child id or -1, no child id or -1)
    for (size_t i = 0; i < states.size(); i++) {
        St c = states[i];
        int m = 0; for (int v : c) m += v;
        trans.emplace_back();
        if (m == 1) continue;
        vector<St> qs; St a(R + 1, 0); enumQ(c, 0, a, qs);
        for (auto& q : qs) {
            int in = 0; for (int v : q) in += v;
            if (2 * in > m) continue;            // complement symmetry
            int ell = min(in, m - in);
            if (NOELL0 && ell == 0) continue;
            int ch[2];
            for (int y = 0; y < 2; y++) {
                St d = child(c, q, y == 1, ell, R);
                int dm = 0; for (int v : d) dm += v;
                if (dm == 0) { ch[y] = -1; continue; }
                auto it = id.find(d);
                if (it == id.end()) { int nid = states.size(); id[d] = nid; states.push_back(d); ch[y] = nid; }
                else ch[y] = it->second;
            }
            trans[i].push_back({ch[1], ch[0]});
        }
    }
    int N = states.size();
    vector<int> val(N, -1);
    for (int i = 0; i < N; i++) { int m = 0; for (int v : states[i]) m += v; if (m == 1) val[i] = 0; }
    int round = 0;
    while (true) {
        round++;
        vector<int> add;
        for (int i = 0; i < N; i++) if (val[i] < 0) {
            for (auto& t : trans[i]) {
                bool ok = true;
                if (t.first >= 0 && val[t.first] < 0) ok = false;
                if (t.second >= 0 && val[t.second] < 0) ok = false;
                if (ok) { add.push_back(i); break; }
            }
        }
        if (add.empty()) break;
        for (int i : add) val[i] = round;
    }
    // independent re-check of the trap: every unvalued state, every question, some legal answer stays unvalued
    int trap = 0; bool closed = true;
    for (int i = 0; i < N; i++) if (val[i] < 0) {
        trap++;
        for (auto& t : trans[i]) {
            bool stay = (t.first >= 0 && val[t.first] < 0) || (t.second >= 0 && val[t.second] < 0);
            if (!stay) closed = false;
        }
    }
    printf("fix k=%d n=%d%s states=%d rounds=%d Q=", K, n, NOELL0 ? " noell0" : "", N, round - 1);
    if (val[0] < 0) printf("INF"); else printf("%d", val[0]);
    printf(" P=%d trap_states=%d trap_closed=%d root_in_trap=%d\n", Pn(n), trap, (int)closed, (int)(val[0] < 0));
    fflush(stdout);
    return val[0];
}

// ------------------------------------------------------------------ td mode
// state at j questions left: c[0..j], index j = cannot lie any more
unordered_map<string, char> memo[64];
long long nodesTD = 0;
static bool win(const St& c, int j);
struct Ctx { const St* c; int j; St a; long long lim; int tot; };
static bool leaf(Ctx& x) {
    const St& c = *x.c; int j = x.j;
    int in = 0; for (int r = 0; r <= j; r++) in += x.a[r];
    if (2 * in > x.tot) return false;
    int ell = min(in, x.tot - in);
    if (NOELL0 && ell == 0) return false;
    St ch[2]; int cnt[2]; long long w[2];
    for (int y = 0; y < 2; y++) {
        St d(j, 0);
        for (int r = 0; r <= j; r++) {
            int tr = y ? x.a[r] : c[r] - x.a[r];
            int li = c[r] - tr;
            int nr = max(r - 1, 0); if (nr > j - 1) nr = j - 1;
            d[nr] += tr;
            if (r == 0) { int o = K * ell; if (o > j - 1) o = j - 1; d[o] += li; }
        }
        cnt[y] = 0; w[y] = 0;
        for (int r = 0; r < j; r++) { cnt[y] += d[r]; w[y] += (long long)d[r] * (j - r); }
        ch[y] = d;
    }
    int first = w[1] >= w[0] ? 1 : 0;
    for (int t = 0; t < 2; t++) {
        int y = t == 0 ? first : 1 - first;
        if (cnt[y] <= 1) continue;               // illegal (0) or immediate win (1)
        if (PRUNE && w[y] > x.lim) return false;
    }
    for (int t = 0; t < 2; t++) {
        int y = t == 0 ? first : 1 - first;
        if (cnt[y] <= 1) continue;
        if (!win(ch[y], j - 1)) return false;
    }
    return true;
}
static bool dfs(Ctx& x, int r, long long yLB, long long nLB) {
    const St& c = *x.c; int j = x.j;
    if (PRUNE && (yLB > x.lim || nLB > x.lim)) return false;
    if (r > j) return leaf(x);
    if (c[r] == 0) { x.a[r] = 0; return dfs(x, r + 1, yLB, nLB); }
    int nr = max(r - 1, 0); if (nr > j - 1) nr = j - 1;
    long long wt = j - nr;                     // weight in child (j-1 left) of a truthful candidate
    int mid = c[r] / 2;
    for (int d = 0; d <= c[r]; d++) {          // from the middle outward
        int v = (d & 1) ? mid + (d + 1) / 2 : mid - d / 2;
        if (v < 0 || v > c[r]) continue;
        x.a[r] = v;
        long long y2 = yLB + v * wt + (r == 0 ? (c[r] - v) : 0);
        long long n2 = nLB + (c[r] - v) * wt + (r == 0 ? v : 0);
        if (dfs(x, r + 1, y2, n2)) return true;
    }
    return false;
}
static bool win(const St& c, int j) {   // at least 2 alive
    nodesTD++;
    if (j == 0) return false;
    long long w = 0; int tot = 0;
    for (int r = 0; r <= j; r++) { w += (long long)c[r] * (j - r + 1); tot += c[r]; }
    if (PRUNE && w > (1LL << j)) return false;
    string key((const char*)c.data(), c.size() * sizeof(int));
    auto it = memo[j].find(key);
    if (it != memo[j].end()) return it->second;
    Ctx x; x.c = &c; x.j = j; x.a.assign(j + 1, 0); x.lim = 1LL << (j - 1); x.tot = tot;
    bool res = dfs(x, 0, 0, 0);
    memo[j][key] = res;
    if (res && POLICY) { printf("%d :", j); for (int v : c) printf(" %d", v); printf(" :"); for (int v : x.a) printf(" %d", v); printf("\n"); }
    return res;
}
static bool tdroot(int n, int q) {
    St c(q + 1, 0); c[0] = n;
    return win(c, q);
}

int main(int argc, char** argv) {
    string mode = argv[1];
    if (mode == "fix") {
        K = atoi(argv[2]); int n = atoi(argv[3]); if (argc > 4) NOELL0 = true;
        fixmode(n);
    } else if (mode == "td") {
        K = atoi(argv[2]); int n = atoi(argv[3]), q = atoi(argv[4]);
        for (int i = 5; i < argc; i++) { if (!strcmp(argv[i], "noprune")) PRUNE = false; if (!strcmp(argv[i], "noell0")) NOELL0 = true; }
        bool w = tdroot(n, q);
        printf("td k=%d n=%d q=%d prune=%d noell0=%d win=%d nodes=%lld\n", K, n, q, (int)PRUNE, (int)NOELL0, (int)w, nodesTD);
    } else if (mode == "policy") {
        K = atoi(argv[2]); int n = atoi(argv[3]), q = atoi(argv[4]); POLICY = true;
        printf("# k %d n %d q %d ; lines: j : counts c[0..j] (index = obligation capped at j) : a[0..j] put in S\n", K, n, q);
        bool w = tdroot(n, q); printf("# win=%d\n", (int)w);
    } else if (mode == "cmp") {
        // compare Q_k game from (a zeros, b permanently blocked, j left) with the one-lie game from (a,b,j)
        K = atoi(argv[2]); int N = atoi(argv[3]), J = atoi(argv[4]);
        vector<vector<vector<char>>> W(J + 1, vector<vector<char>>(N + 1, vector<char>(N + 1, 0)));
        long long tested = 0, diff = 0, qharder = 0;
        for (int j = 0; j <= J; j++)
            for (int a = 0; a <= N; a++) for (int b = 0; a + b <= N; b++) {
                if (a + b <= 1) { W[j][a][b] = 1; continue; }
                if (j == 0) continue;
                bool ok = false;
                for (int a1 = 0; a1 <= a && !ok; a1++) for (int b1 = 0; b1 <= b; b1++) {
                    int a2 = a - a1, b2 = b - b1;
                    if (W[j - 1][a1][b1 + a2] && W[j - 1][a2][b2 + a1]) { ok = true; break; }
                }
                W[j][a][b] = ok;
                St c(j + 1, 0); c[0] = a; c[j] += b;
                bool qw = win(c, j);
                tested++;
                if (qw != ok) { diff++; if (!qw) qharder++; if (diff <= 40) printf("DIFF k=%d j=%d a=%d b=%d onelie=%d Qk=%d\n", K, j, a, b, (int)ok, (int)qw); }
            }
        printf("cmp k=%d N=%d J=%d states=%lld differ=%lld (Qk loses where one-lie wins: %lld)\n", K, N, J, tested, diff, qharder);
    } else if (mode == "sweep") {
        K = atoi(argv[2]); int n1 = atoi(argv[3]), n2 = atoi(argv[4]);
        for (int n = n1; n <= n2; n++) {
            int p = Pn(n);
            bool lo = tdroot(n, p - 1);
            bool hi = tdroot(n, p);
            printf("k=%d n=%d P=%d win(P-1)=%d win(P)=%d", K, n, p, (int)lo, (int)hi);
            if (!hi) { for (int q = p + 1; q <= p + 3; q++) { bool w = tdroot(n, q); printf(" win(%d)=%d", q, (int)w); if (w) break; } }
            printf(" %s\n", (!lo && hi) ? "EQ" : "NEQ"); fflush(stdout);
            for (int j = 0; j < 64; j++) if (memo[j].size() > 3000000) memo[j].clear();
        }
    } else if (mode == "u1") {
        int nmax = atoi(argv[2]); int J = 14;
        // W[j][a][b] = questioner identifies within j from a never-lied, b lied-once
        vector<vector<vector<char>>> W(J + 1, vector<vector<char>>(nmax + 1, vector<char>(nmax + 1, 0)));
        for (int j = 0; j <= J; j++)
            for (int a = 0; a <= nmax; a++) for (int b = 0; a + b <= nmax; b++) {
                if (a + b <= 1) { W[j][a][b] = 1; continue; }
                if (j == 0) continue;
                bool ok = false;
                for (int a1 = 0; a1 <= a && !ok; a1++) for (int b1 = 0; b1 <= b; b1++) {
                    int a2 = a - a1, b2 = b - b1;
                    if (W[j - 1][a1][b1 + a2] && W[j - 1][a2][b2 + a1]) { ok = true; break; }
                }
                W[j][a][b] = ok;
            }
        int bad = 0;
        for (int n = 2; n <= nmax; n++) {
            int q = 0; while (!W[q][n][0]) q++;
            if (q != Pn(n)) { bad++; printf("U1 MISMATCH n=%d U1=%d P=%d\n", n, q, Pn(n)); }
        }
        printf("u1: n=2..%d mismatches with P(n): %d\n", nmax, bad);
    }
    return 0;
}
