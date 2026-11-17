// History-class solver, sparse beliefs (same lazy safety-game algorithm as solve.cpp mode H).
// usage: hist d B [dumpfile]
// Belief = set of physical states (x_t, signs of x_{t-d..t-1}, rest flag) consistent with the
// observations o_0..o_t. Controller picks u (knowing the belief, hence o_t); successors are split
// by the next observation sgn(x_{t+1-d}); the adversary picks which nonempty part occurs.
#include <bits/stdc++.h>
using namespace std;
typedef vector<uint32_t> Bel;
int d, B, P3;
struct BH { size_t operator()(const Bel& b) const { uint64_t h = 1469598103934665603ULL; for (auto w : b) { h ^= w; h *= 1099511628211ULL; h ^= h >> 31; } return h; } };
inline int enc(int x, int code, int fl) { return ((x + B) * P3 + code) * 2 + fl; }
inline int sg(int x) { return x < 0 ? 0 : (x == 0 ? 1 : 2); }
inline int obs_of(int s) { return ((s >> 1) % P3) / (P3 / 3); }
bool stepall(const Bel& S, int u, Bel part[3]) {
    for (int k = 0; k < 3; k++) part[k].clear();
    for (uint32_t s : S) {
        int fl = s & 1, r = s >> 1, code = r % P3, x = r / P3 - B;
        int ncode = (code % (P3 / 3)) * 3 + sg(x);
        for (int w = -1; w <= 1; w++) {
            if (fl && w) continue;
            int nx = x + u + w;
            if (nx < -B || nx > B) return false;
            int t = enc(nx, ncode, w != 0);
            part[obs_of(t)].push_back(t);
        }
    }
    for (int k = 0; k < 3; k++) { sort(part[k].begin(), part[k].end()); part[k].erase(unique(part[k].begin(), part[k].end()), part[k].end()); }
    return true;
}
int main(int argc, char** argv) {
    d = atoi(argv[1]); B = atoi(argv[2]);
    P3 = 1; for (int i = 0; i < d; i++) P3 *= 3;
    unordered_map<Bel, int, BH> id; vector<const Bel*> bel; vector<char> cu, losing; vector<vector<int>> preds; vector<array<int, 3>> succ; vector<int> work;
    auto getid = [&](const Bel& b) { auto it = id.find(b); if (it != id.end()) return it->second; int n = bel.size(); auto r = id.emplace(b, n); bel.push_back(&r.first->first); cu.push_back(0); losing.push_back(0); preds.emplace_back(); succ.push_back({ -1, -1, -1 }); work.push_back(n); return n; };
    Bel init = { (uint32_t)enc(0, (P3 - 1) / 2, 0) };
    getid(init);
    auto order_of = [&](int n, int* order) { int o = obs_of((*bel[n])[0]) - 1; order[0] = -o; order[1] = (o == 0) ? 1 : 0; order[2] = (o == 0) ? -1 : o; };
    Bel part[3];
    while (!work.empty()) {
        int n = work.back(); work.pop_back();
        if (losing[n]) continue;
        int order[3]; order_of(n, order);
        bool ok = false;
        while (cu[n] < 3) {
            Bel cur = *bel[n];
            if (!stepall(cur, order[(int)cu[n]], part)) { cu[n]++; continue; }
            bool bad = false;
            for (int k = 0; k < 3; k++) if (!part[k].empty()) { auto it = id.find(part[k]); if (it != id.end() && losing[it->second]) bad = true; }
            if (bad) { cu[n]++; continue; }
            int sid[3] = { -1, -1, -1 };
            for (int k = 0; k < 3; k++) if (!part[k].empty()) { sid[k] = getid(part[k]); preds[sid[k]].push_back(n); }
            succ[n] = { sid[0], sid[1], sid[2] };
            ok = true; break;
        }
        if (!ok) { losing[n] = 1; for (int p : preds[n]) if (!losing[p]) work.push_back(p); vector<int>().swap(preds[n]); if (n == 0) break; }
    }
    bool win = !losing[0];
    printf("d=%d B=%d HISTORY %s beliefs_created=%zu\n", d, B, win ? "FEASIBLE" : "INFEASIBLE", bel.size());
    if (win) {
        vector<int> mapid(bel.size(), -1), ord; ord.push_back(0); mapid[0] = 0;
        for (size_t i = 0; i < ord.size(); i++) { int n = ord[i]; for (int k = 0; k < 3; k++) { int s = succ[n][k]; if (s >= 0 && mapid[s] < 0) { mapid[s] = ord.size(); ord.push_back(s); } } }
        size_t maxsz = 0; for (int n : ord) maxsz = max(maxsz, bel[n]->size());
        printf("  strategy automaton: %zu belief nodes, max belief size %zu\n", ord.size(), maxsz);
        if (argc > 3) {
            FILE* fp = fopen(argv[3], "w"); fprintf(fp, "%d %d %zu\n", d, B, ord.size());
            for (int n : ord) { int order[3]; order_of(n, order); fprintf(fp, "%d %d %d %d\n", order[(int)cu[n]], succ[n][0] < 0 ? -1 : mapid[succ[n][0]], succ[n][1] < 0 ? -1 : mapid[succ[n][1]], succ[n][2] < 0 ? -1 : mapid[succ[n][2]]); }
            fclose(fp);
        }
    }
    return 0;
}
