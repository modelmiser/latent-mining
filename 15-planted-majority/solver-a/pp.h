// pp.h -- fast checker for "planted majority" protocols.
// A protocol: s states, qA, qB, output w[q] in {0 (=A), 1 (=B)}, rule d[p][q] = p'*s+q' (or -1 = undefined).
// checkInstance returns 0 PASS, 1 FAIL, 2 NEED (an undefined entry d[needp][needq] was required).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
using namespace std;
const int MAXS = 40;
struct Proto { int s, qA, qB; int w[MAXS]; int d[MAXS][MAXS]; };
struct Cfg { uint8_t c[MAXS]; };
struct Checker {
    const Proto* P; int s;
    int needp, needq; bool partial = false;
    long long lastNodes = 0;
    string failWhy;
    struct H { int s; size_t operator()(const string& k) const { return hash<string>()(k); } };
    unordered_map<string,int> id;
    vector<Cfg> nodes; vector<int> estart, edges; vector<char> good, selfloop, terminal;
    // maj: 0 = A, 1 = B
    int checkInstance(int a, int b, int x) {
        s = P->s; int maj = (a > b) ? 0 : 1;
        id.clear(); nodes.clear(); estart.clear(); edges.clear(); good.clear(); selfloop.clear(); terminal.clear();
        Cfg c0; memset(&c0, 0, sizeof c0); c0.c[P->qA] += a; c0.c[P->qB] += b; c0.c[x] += 1;
        id[string((char*)c0.c, s)] = 0; nodes.push_back(c0); bool need = false;
        for (size_t i = 0; i < nodes.size(); i++) {
            Cfg c = nodes[i];
            estart.push_back((int)edges.size());
            bool g = true; for (int q = 0; q < s; q++) if (c.c[q] && P->w[q] != maj) g = false;
            bool legal = false, sl = false, incomplete = false;
            for (int p = 0; p < s; p++) if (c.c[p]) for (int q = 0; q < s; q++) if (c.c[q] && (p != q || c.c[p] >= 2)) {
                int v = P->d[p][q];
                if (v < 0) { if (!need) { needp = p; needq = q; need = true; } if (!partial) { lastNodes = nodes.size(); return 2; } incomplete = true; continue; }
                if (v == p * s + q) continue;
                legal = true;
                int p2 = v / s, q2 = v % s;
                Cfg n = c; n.c[p]--; n.c[q]--; n.c[p2]++; n.c[q2]++;
                if (memcmp(n.c, c.c, s) == 0) { sl = true; continue; }
                string k((char*)n.c, s);
                auto it = id.find(k); int j;
                if (it == id.end()) { j = (int)nodes.size(); id.emplace(k, j); nodes.push_back(n); } else j = it->second;
                edges.push_back(j);
            }
            good.push_back(g); selfloop.push_back(sl); terminal.push_back(!legal && !incomplete);
            if (!g && !legal && !incomplete) { lastNodes = nodes.size(); failWhy = "bad terminal " + show(c); return 1; }
            if (!g && sl) { lastNodes = nodes.size(); failWhy = "bad self-loop " + show(c); return 1; }
        }
        estart.push_back((int)edges.size());
        lastNodes = nodes.size();
        // Tarjan SCC, iterative; fail if a non-good node lies in an SCC of size > 1
        int n = (int)nodes.size();
        vector<int> idx(n, -1), low(n, 0), st, ptr(n, 0); vector<char> onst(n, 0);
        vector<int> cs; int counter = 0;
        for (int r = 0; r < n; r++) if (idx[r] < 0) {
            cs.push_back(r); idx[r] = low[r] = counter++; st.push_back(r); onst[r] = 1; ptr[r] = estart[r];
            while (!cs.empty()) {
                int v = cs.back();
                if (ptr[v] < estart[v + 1]) {
                    int u = edges[ptr[v]++];
                    if (idx[u] < 0) { idx[u] = low[u] = counter++; st.push_back(u); onst[u] = 1; ptr[u] = estart[u]; cs.push_back(u); }
                    else if (onst[u]) low[v] = min(low[v], idx[u]);
                } else {
                    cs.pop_back();
                    if (!cs.empty()) low[cs.back()] = min(low[cs.back()], low[v]);
                    if (low[v] == idx[v]) {
                        int sz = 0; bool bad = false; int badnode = -1;
                        while (true) { int u = st.back(); st.pop_back(); onst[u] = 0; sz++; if (!good[u]) { bad = true; badnode = u; } if (u == v) break; }
                        if (sz > 1 && bad) { failWhy = "bad cycle through " + show(nodes[badnode]); return 1; }
                    }
                }
            }
        }
        return need ? 2 : 0;
    }
    string show(const Cfg& c) { string r = "["; for (int q = 0; q < s; q++) { r += to_string((int)c.c[q]); if (q + 1 < s) r += ","; } return r + "]"; }
    // check all instances with 2 <= a+b <= N (in increasing size). returns 0/1/2; on FAIL sets fa,fb,fx
    int fa, fb, fx;
    int checkUpTo(int N, int from = 2) {
        for (int t = from; t <= N; t++) for (int a = 0; a <= t; a++) { int b = t - a; if (abs(a - b) < 2) continue;
            for (int x = 0; x < P->s; x++) { int r = checkInstance(a, b, x); if (r) { fa = a; fb = b; fx = x; return r; } } }
        return 0;
    }
};
