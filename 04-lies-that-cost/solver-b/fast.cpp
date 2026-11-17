// fast.cpp -- Q_k(n) by memoised search on the multiset of remaining honesty obligations.
// State with q questions left: c[r] = number of alive candidates whose remaining obligation is r
// (r = number of upcoming questions on which the candidate may not lie), r capped at q
// (r >= q  <=> cannot lie again within the horizon; exact, not a heuristic).
// usage: fast K NMIN NMAX PRUNE(0/1) [QMAX] [DUMP n q file]
#include <bits/stdc++.h>
using namespace std;
int K, PRUNE;
unordered_map<string, char> memo;
// best question found for winning states (for witness extraction)
unordered_map<string, vector<int>> bestq;
bool RECORD = false;

static string key(const vector<int>& c, int q) {
    string s; s.reserve(c.size() + 1);
    s.push_back((char)q);
    for (int v : c) { s.push_back((char)(v & 255)); s.push_back((char)(v >> 8)); }
    return s;
}
bool win(const vector<int>& c, int q);

// apply question a to state c (size q+1) -> two outcome states of size q (indices 0..q-1)
static void outcomes(const vector<int>& c, const vector<int>& a, int q, vector<int>& Y, vector<int>& N) {
    int tot = 0, sa = 0;
    for (int r = 0; r <= q; r++) { tot += c[r]; sa += a[r]; }
    int ell = min(sa, tot - sa);
    int nr = min(K * ell, q - 1);
    Y.assign(q, 0); N.assign(q, 0);
    for (int r = 0; r <= q; r++) {
        int d = max(r - 1, 0);
        Y[d] += a[r];            // in S, answer yes: truthful
        N[d] += c[r] - a[r];     // not in S, answer no: truthful
    }
    Y[nr] += c[0] - a[0];        // answer yes: those outside S with r=0 lie
    N[nr] += a[0];               // answer no: those inside S with r=0 lie
}
static int total(const vector<int>& c) { int t = 0; for (int v : c) t += v; return t; }

bool win(const vector<int>& c, int q) {
    int tot = total(c);
    if (tot == 1) return true;
    if (q == 0) return false;
    if (PRUNE) {
        // volume bound: responder restricted to "each candidate lies at most once more"
        long long w = 0;
        for (int r = 0; r <= q; r++) w += (long long)c[r] * (r == 0 ? q + 1 : 1 + (q - r));
        if (w > (1LL << q)) return false;
    }
    string k = key(c, q);
    auto it = memo.find(k);
    if (it != memo.end()) return it->second;
    vector<int> a(q + 1, 0), Y, N;
    bool res = false;
    while (true) {
        int sa = 0; for (int v : a) sa += v;
        if (2 * sa <= tot) {
            outcomes(c, a, q, Y, N);
            int ty = total(Y), tn = total(N);
            bool ok = true;
            if (ty > 0 && !win(Y, q - 1)) ok = false;
            if (ok && tn > 0 && !win(N, q - 1)) ok = false;
            if (ok) { res = true; if (RECORD) bestq[k] = a; break; }
        }
        int i = 0;
        while (i <= q) { if (a[i] < c[i]) { a[i]++; break; } a[i] = 0; i++; }
        if (i > q) break;
    }
    memo[k] = res;
    return res;
}

// ---- witness dump: explicit strategy tree over elements 1..n ----
FILE* out;
void dump(vector<vector<int>> cls /* cls[r] = elements with obligation r */, int q, string path) {
    vector<int> c(q + 1, 0);
    for (int r = 0; r <= q; r++) c[r] = cls[r].size();
    if (total(c) == 1) return;
    string k = key(c, q);
    if (!win(c, q) || !bestq.count(k)) { fprintf(stderr, "dump: non-winning state reached\n"); exit(2); }
    vector<int> a = bestq[k];
    int tot = total(c), sa = 0; for (int v : a) sa += v;
    int ell = min(sa, tot - sa), nr = min(K * ell, q - 1);
    fprintf(out, "%s:", path.empty() ? "-" : path.c_str());
    vector<vector<int>> Y(q), N(q);
    for (int r = 0; r <= q; r++) {
        int d = max(r - 1, 0);
        for (int i = 0; i < (int)cls[r].size(); i++) {
            int x = cls[r][i];
            if (i < a[r]) { fprintf(out, " %d", x); Y[d].push_back(x); if (r == 0) N[nr].push_back(x); }
            else { N[d].push_back(x); if (r == 0) Y[nr].push_back(x); }
        }
    }
    fprintf(out, "\n");
    int ty = 0, tn = 0; for (auto& v : Y) ty += v.size(); for (auto& v : N) tn += v.size();
    if (ty > 0) dump(Y, q - 1, path + "1");
    if (tn > 0) dump(N, q - 1, path + "0");
}

int main(int argc, char** argv) {
    K = atoi(argv[1]); int nmin = atoi(argv[2]), nmax = atoi(argv[3]); PRUNE = atoi(argv[4]);
    int qmax = argc > 5 ? atoi(argv[5]) : 14;
    if (argc > 6 && string(argv[6]) == "DUMP") {
        int n = atoi(argv[7]), q = atoi(argv[8]);
        RECORD = true;
        vector<int> c(q + 1, 0); c[0] = n;
        if (!win(c, q)) { printf("not winnable\n"); return 1; }
        out = fopen(argv[9], "w");
        fprintf(out, "# strategy for k=%d n=%d within q=%d questions. line = answers-so-far (1=yes,0=no; - = root): elements of S\n", K, n, q);
        fprintf(out, "k %d n %d q %d\n", K, n, q);
        vector<vector<int>> cls(q + 1);
        for (int x = 1; x <= n; x++) cls[0].push_back(x);
        dump(cls, q, "");
        fclose(out);
        printf("dumped\n");
        return 0;
    }
    for (int n = nmin; n <= nmax; n++) {
        int ans = -1;
        for (int q = 1; q <= qmax; q++) {
            vector<int> c(q + 1, 0); c[0] = n;
            if (win(c, q)) { ans = q; break; }
        }
        if (ans < 0) printf("k=%d n=%d Q>%d  (memo %zu)\n", K, n, qmax, memo.size());
        else printf("k=%d n=%d Q=%d  (memo %zu)\n", K, n, ans, memo.size());
        fflush(stdout);
    }
}
