// OTHER READING (not the headline): Adversary may remove ANY present ball.
// Then arrival order is irrelevant and a state is an oriented forest up to isomorphism
// (each edge points at the bin that received the ball).
// Canonical form: per non-trivial tree, AHU string rooted at the centre (min over the two
// centres if bicentral), each child string prefixed by 'c' (child received the ball) or
// 'p' (parent received it); trees sorted.  Usage: anyiso n L
#include <bits/stdc++.h>
using namespace std;
int n, L;
struct St { int m; unsigned char w[12], o[12]; };
static string encv(int v, int par, const vector<vector<pair<int,int>>>& adj) {
    vector<string> ch;
    for (auto& e : adj[v]) if (e.first != par) ch.push_back(string(1, e.second ? 'p' : 'c') + encv(e.first, v, adj));
    sort(ch.begin(), ch.end());
    string r = "("; for (auto& c : ch) r += c; r += ")"; return r;
}
static string canon(const St& s) {
    vector<vector<pair<int,int>>> adj(n);   // (nbr, 1 if THIS vertex received the ball)
    for (int i = 0; i < s.m; i++) { adj[s.w[i]].push_back({s.o[i], 1}); adj[s.o[i]].push_back({s.w[i], 0}); }
    vector<int> seen(n, 0); vector<string> trees;
    for (int v0 = 0; v0 < n; v0++) if (!seen[v0] && !adj[v0].empty()) {
        vector<int> comp{v0}; seen[v0] = 1;
        for (size_t h = 0; h < comp.size(); h++) for (auto& e : adj[comp[h]]) if (!seen[e.first]) { seen[e.first] = 1; comp.push_back(e.first); }
        // centres by leaf stripping
        vector<int> deg(n, 0), gone(n, 0); vector<int> layer; int left = comp.size();
        for (int v : comp) { deg[v] = adj[v].size(); if (deg[v] == 1) layer.push_back(v); }
        while (left > 2) {
            vector<int> nl; left -= layer.size();
            for (int v : layer) gone[v] = 1;
            for (int v : layer) for (auto& e : adj[v]) if (!gone[e.first] && --deg[e.first] == 1) nl.push_back(e.first);
            layer = nl;
        }
        string best;
        vector<int> cs; for (int v : comp) if (!gone[v]) cs.push_back(v);
        if (cs.empty() || cs.size() > 2) { fprintf(stderr, "centre bug\n"); exit(2); }
        for (int c : cs) { string t = encv(c, -1, adj); if (best.empty() || t < best) best = t; }
        trees.push_back(best);
    }
    sort(trees.begin(), trees.end());
    string r; for (auto& t : trees) r += t; return r;
}
static int maxload(const St& s) { int l[16] = {0}, b = 0; for (int i = 0; i < s.m; i++) b = max(b, ++l[s.w[i]]); return b; }
int main(int argc, char** argv) {
    n = atoi(argv[1]); L = atoi(argv[2]);
    unordered_map<string, int> idx; vector<St> order;
    St e; e.m = 0; idx[canon(e)] = 0; order.push_back(e);
    vector<unsigned char> win;
    vector<vector<int>> rem, off;   // rem: successor indices by removal; off: pairs
    auto get = [&](const St& t) { string k = canon(t); auto it = idx.find(k); if (it != idx.end()) return it->second; int id = order.size(); idx.emplace(k, id); order.push_back(t); return id; };
    for (size_t i = 0; i < order.size(); i++) {
        St s = order[i];
        rem.emplace_back(); off.emplace_back();
        if (maxload(s) >= L) { win.push_back(1); continue; }
        win.push_back(0);
        vector<int> R, O;
        for (int j = 0; j < s.m; j++) {
            St t; t.m = 0; for (int q = 0; q < s.m; q++) if (q != j) { t.w[t.m] = s.w[q]; t.o[t.m] = s.o[q]; t.m++; }
            R.push_back(get(t));
        }
        int c[16]; bool touched[16] = {0}; for (int v = 0; v < n; v++) c[v] = v;
        for (int q = 0; q < s.m; q++) { touched[s.w[q]] = touched[s.o[q]] = 1; int a = c[s.w[q]], b = c[s.o[q]]; if (a != b) for (int x = 0; x < n; x++) if (c[x] == a) c[x] = b; }
        int f1 = -1, f2 = -1; for (int v = 0; v < n; v++) if (!touched[v]) { if (f1 < 0) f1 = v; else if (f2 < 0) f2 = v; }
        set<pair<int,int>> seenp;
        for (int a = 0; a < n; a++) for (int b = a + 1; b < n; b++) {
            if (c[a] == c[b]) continue;
            if (!touched[a] && a != f1) continue;     // untouched bins are interchangeable:
            if (!touched[b] && b != f1 && !(a == f1 && b == f2)) continue;   // use f1 (and f2 with f1)
            if (!touched[a] && !touched[b] && !(a == f1 && b == f2)) continue;
            St t = s; t.w[t.m] = a; t.o[t.m] = b; t.m++; St t2 = s; t2.w[t2.m] = b; t2.o[t2.m] = a; t2.m++;
            int x = get(t), y = get(t2); if (x > y) swap(x, y);
            if (seenp.insert({x, y}).second) { O.push_back(x); O.push_back(y); }
        }
        sort(R.begin(), R.end()); R.erase(unique(R.begin(), R.end()), R.end());
        rem[i] = R; off[i] = O;
    }
    size_t N = order.size();
    bool changed = true; int sweeps = 0;
    while (changed) {
        changed = false; sweeps++;
        for (size_t i = N; i-- > 0;) {
            if (win[i]) continue;
            bool w = false;
            for (int t : rem[i]) if (win[t]) { w = true; break; }
            if (!w) for (size_t j = 0; j < off[i].size(); j += 2) if (win[off[i][j]] && win[off[i][j + 1]]) { w = true; break; }
            if (w) { win[i] = 1; changed = true; }
        }
    }
    size_t nw = 0; for (size_t i = 0; i < N; i++) nw += win[i];
    printf("n=%d L=%d mode=any(iso) states=%zu adversary_wins=%d winstates=%zu sweeps=%d\n", n, L, N, (int)win[0], nw, sweeps);
    if (argc > 3 && !win[0]) {   // Rule safe set: closed under removals, one safe answer per offer
        FILE* f = fopen(argv[3], "w");
        fprintf(f, "# RULE safe set, ANY-removal game, n=%d L=%d. One state per line: balls as receiver:other (order irrelevant), bins 0..n-1; states are meant up to isomorphism.\n", n, L);
        vector<char> seen(N, 0); vector<int> q{0}; seen[0] = 1;
        for (size_t h = 0; h < q.size(); h++) {
            int i = q[h]; const St& s = order[i];
            for (int j = 0; j < s.m; j++) fprintf(f, "%s%d:%d", j ? " " : "", s.w[j], s.o[j]);
            fprintf(f, "\n");
            for (int t : rem[i]) if (!seen[t]) { seen[t] = 1; q.push_back(t); }
            for (size_t j = 0; j < off[i].size(); j += 2) {
                int t0 = off[i][j], t1 = off[i][j + 1];
                int t = (!win[t0] && seen[t0]) ? t0 : (!win[t1] && seen[t1]) ? t1 : (!win[t0] ? t0 : t1);
                if (!seen[t]) { seen[t] = 1; q.push_back(t); }
            }
        }
        fclose(f); printf("safe set lines=%zu\n", q.size());
    }
    return 0;
}
