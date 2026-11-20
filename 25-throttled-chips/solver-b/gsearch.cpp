// Exhaustive search for G(h): is there a winning list of length <= m ?
// State after a prefix = set S of all Firer-reachable positions in which no vertex ever reached h.
// List wins iff S becomes empty.  DFS over prefixes, memo on (S up to translation/reflection, P, rounds left)
// where P = set of add-positions so far is only needed for the gap rule; see below.
//
// PRUNINGS (all claimed safe; see proofs.txt):
//  (1) a_1 = 0 (translation).
//  (2) gap rule: WLOG consecutive distinct add positions of the final list differ by <= g0(m)
//      (bigger gaps are game-equivalent to gap g0).  Enforced as: number of fillers still needed <= rounds left.
//      Can be disabled with g0 = 0 => then positions are restricted to window [-R,R] (for cross-checks).
//  (3) pass-forever bound: if some c in S has max(c) + r < h with r rounds left, Adder cannot win in r rounds.
//  (4) memo: failure of (S, r) implies failure for same S and r' <= r; only used when gap rule is off OR
//      keyed together with the gap structure (we key with the sorted add-position set, translated with S).
#include <bits/stdc++.h>
using namespace std;
typedef string Cfg;              // heights, index = position + OFF
static const int OFF = 100, W = 200;
int H, G0, RWIN;
long long nodes = 0;
unordered_map<string, int> failmemo;  // key -> max r known to fail
vector<int> path, best;

static void stepSet(const vector<Cfg>& S, int a, vector<Cfg>& out) {
    out.clear();
    for (const Cfg& c0 : S) {
        Cfg c = c0;
        if (++c[a + OFF] >= H) continue;
        out.push_back(c);
        for (int x = 1; x < W - 1; x++) if (c[x] >= 2) {
            if (c[x-1] + 1 >= H || c[x+1] + 1 >= H) continue;
            Cfg e = c; e[x] -= 2; e[x-1]++; e[x+1]++;
            out.push_back(e);
        }
    }
    sort(out.begin(), out.end());
    out.erase(unique(out.begin(), out.end()), out.end());
}
static string keyOf(const vector<Cfg>& S, const set<int>& P) {
    // canonical under translation + reflection. Window = [lo,hi] covering all chips and all P.
    int lo = W, hi = -1;
    for (const Cfg& c : S) for (int x = 0; x < W; x++) if (c[x]) { lo = min(lo, x); hi = max(hi, x); }
    if (G0) { lo = min(lo, *P.begin() + OFF); hi = max(hi, *P.rbegin() + OFF); }
    else { lo = 0; hi = W - 1; }   // window mode: absolute coordinates, no translation/reflection
    string best;
    for (int refl = 0; refl < (G0 ? 2 : 1); refl++) {
        vector<string> v;
        for (const Cfg& c : S) {
            string s = c.substr(lo, hi - lo + 1);
            if (refl) reverse(s.begin(), s.end());
            v.push_back(s);
        }
        sort(v.begin(), v.end());
        string k;
        for (auto& s : v) { k += s; k.push_back((char)127); }
        if (G0) {
            k.push_back((char)126);
            vector<int> q;
            for (int p : P) q.push_back(refl ? hi - (p + OFF) : (p + OFF) - lo);
            sort(q.begin(), q.end());
            for (int p : q) k.push_back((char)p);
        }
        if (refl == 0 || k < best) best = k;
    }
    return best;
}
static int fillers(const set<int>& P) {
    int f = 0, prev = INT_MIN;
    for (int p : P) { if (prev != INT_MIN) f += (p - prev + G0 - 1) / G0 - 1; prev = p; }
    return f;
}
bool dfs(const vector<Cfg>& S, set<int>& P, int r) {
    nodes++;
    if (S.empty()) { best = path; return true; }
    if (r == 0) return false;
    for (const Cfg& c : S) {
        int mx = 0; for (char ch : c) mx = max(mx, (int)ch);
        if (mx + r < H) return false;
    }
    string key = keyOf(S, P);
    auto it = failmemo.find(key);
    if (it != failmemo.end() && it->second >= r) return false;
    int lo, hi;
    if (G0) { lo = *P.begin() - G0 * r; hi = *P.rbegin() + G0 * r; }
    else { lo = -RWIN; hi = RWIN; }
    vector<Cfg> T;
    for (int a = lo; a <= hi; a++) {
        bool ins = P.insert(a).second;
        bool ok = true;
        if (G0 && fillers(P) > r - 1) ok = false;
        if (ok) {
            stepSet(S, a, T);
            path.push_back(a);
            bool w = dfs(T, P, r - 1);
            path.pop_back();
            if (w) { if (ins) P.erase(a); return true; }
        }
        if (ins) P.erase(a);
    }
    int& f = failmemo[key];
    f = max(f, r);
    return false;
}
int main(int argc, char** argv) {
    H = atoi(argv[1]); int m = atoi(argv[2]); G0 = atoi(argv[3]); RWIN = argc > 4 ? atoi(argv[4]) : 0;
    vector<Cfg> S0(1, Cfg(W, 0)), S1;
    set<int> P; P.insert(0);
    stepSet(S0, 0, S1);
    path.push_back(0);
    bool w = dfs(S1, P, m - 1);
    printf("h=%d m=%d g0=%d R=%d : %s  nodes=%lld memo=%zu\n", H, m, G0, RWIN, w ? "WIN" : "no winning list", nodes, failmemo.size());
    if (w) { printf("list:"); for (int a : best) printf(" %d", a); printf("\n"); }
    return 0;
}
