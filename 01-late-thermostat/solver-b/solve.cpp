// Exact solver for the delayed-sign relay/clock/history game.
// Physical state at tick t: (x_t, signs of x_{t-d}..x_{t-1}, rest flag).
// usage: solve d B mode [dump]     mode in {R, C, H}
// Prints FEASIBLE / INFEASIBLE for bound B, plus a witness.
#include <bits/stdc++.h>
using namespace std;
typedef vector<uint64_t> Bits;
int d, B, P3, NS, NW; // P3 = 3^d
struct BH { size_t operator()(const Bits& b) const { uint64_t h = 1469598103934665603ULL; for (auto w : b) { h ^= w; h *= 1099511628211ULL; h ^= h >> 29; } return h; } };
inline int enc(int x, int code, int fl) { return ((x + B) * P3 + code) * 2 + fl; }
inline int sg(int x) { return x < 0 ? 0 : (x == 0 ? 1 : 2); }
// successor of state s under u,w ; returns -1 if out of band
inline int stepst(int s, int u, int w) {
    int fl = s & 1, r = s >> 1, code = r % P3, x = r / P3 - B;
    (void)fl;
    int nx = x + u + w;
    if (nx < -B || nx > B) return -1;
    int ncode = (code % (P3 / 3)) * 3 + sg(x);
    return enc(nx, ncode, w != 0);
}
inline int obs_of(int s) { return ((s >> 1) % P3) / (P3 / 3); }
inline bool get(const Bits& b, int i) { return b[i >> 6] >> (i & 63) & 1; }
inline void setb(Bits& b, int i) { b[i >> 6] |= 1ULL << (i & 63); }
template <class F> void foreach_bit(const Bits& b, F f) {
    for (int k = 0; k < NW; k++) { uint64_t w = b[k]; while (w) { int j = __builtin_ctzll(w); f(k * 64 + j); w &= w - 1; } }
}
// apply map f (f[obs] in -1..1) to set; false if leaves band
bool apply(const Bits& S, const int* f, Bits& out) {
    out.assign(NW, 0);
    bool ok = true;
    foreach_bit(S, [&](int s) {
        if (!ok) return;
        int u = f[obs_of(s)];
        int fl = s & 1;
        for (int w = -1; w <= 1; w++) {
            if (fl && w != 0) continue;
            int t = stepst(s, u, w);
            if (t < 0) { ok = false; return; }
            setb(out, t);
        }
    });
    return ok;
}
string fstr(const int* f) { const char* c = "-0+"; string s; for (int i = 0; i < 3; i++) s += c[f[i] + 1]; return s; }

int main(int argc, char** argv) {
    d = atoi(argv[1]); B = atoi(argv[2]); char mode = argv[3][0];
    bool dump = argc > 4;
    P3 = 1; for (int i = 0; i < d; i++) P3 *= 3;
    NS = (2 * B + 1) * P3 * 2; NW = (NS + 63) / 64;
    Bits init(NW, 0); setb(init, enc(0, (P3 - 1) / 2, 0));
    if (mode == 'R') {
        vector<string> good;
        for (int m = 0; m < 27; m++) {
            int f[3] = { m % 3 - 1, (m / 3) % 3 - 1, m / 9 - 1 };
            Bits S = init; bool ok = true;
            while (ok) {
                Bits T; ok = apply(S, f, T);
                if (!ok) break;
                bool ch = false;
                for (int k = 0; k < NW; k++) { uint64_t n = S[k] | T[k]; if (n != S[k]) ch = true; S[k] = n; }
                if (!ch) break;
            }
            if (ok) good.push_back(fstr(f));
        }
        printf("d=%d B=%d RELAY %s maps(f(-)f(0)f(+)):", d, B, good.empty() ? "INFEASIBLE" : "FEASIBLE");
        for (auto& g : good) printf(" %s", g.c_str());
        printf("\n");
        return 0;
    }
    if (mode == 'C') {
        // DFS over reachable-state sets; success = revisit a set on the stack.
        unordered_map<Bits, int, BH> st; // 1 = on stack, 2 = dead
        struct Fr { Bits S; int m; int usedmask; };
        vector<Fr> stack; vector<int> choice;
        stack.push_back({ init, 0, 0 }); st[init] = 1;
        size_t maxdepth = 0; bool found = false; Bits cyc;
        while (!stack.empty() && !found) {
            Fr& fr = stack.back();
            maxdepth = max(maxdepth, stack.size());
            if (fr.m == 0) { fr.usedmask = 0; foreach_bit(fr.S, [&](int s) { fr.usedmask |= 1 << obs_of(s); }); }
            bool pushed = false;
            while (fr.m < 27) {
                int m = fr.m++;
                int f[3] = { m % 3 - 1, (m / 3) % 3 - 1, m / 9 - 1 };
                // canonical: unused observations get 0, skip duplicates
                bool skip = false;
                for (int o = 0; o < 3; o++) if (!(fr.usedmask >> o & 1) && f[o] != 0) skip = true;
                if (skip) continue;
                Bits T;
                if (!apply(fr.S, f, T)) continue;
                auto it = st.find(T);
                if (it != st.end()) {
                    if (it->second == 2) continue;
                    // on stack: cycle
                    choice.push_back(m); found = true; cyc = T; break;
                }
                st[T] = 1; choice.push_back(m);
                stack.push_back({ T, 0, 0 }); pushed = true; break;
            }
            if (found) break;
            if (!pushed) { st[stack.back().S] = 2; stack.pop_back(); if (!choice.empty()) choice.pop_back(); }
        }
        printf("d=%d B=%d CLOCK %s sets_visited=%zu maxdepth=%zu\n", d, B, found ? "FEASIBLE" : "INFEASIBLE", st.size(), maxdepth);
        if (found) {
            int pre = 0; for (size_t i = 0; i < stack.size(); i++) if (stack[i].S == cyc) pre = i;
            printf("  preperiod=%d period=%zu\n  prefix:", pre, choice.size() - pre);
            for (size_t i = 0; i < choice.size(); i++) {
                if ((int)i == pre) printf("\n  cycle:");
                int m = choice[i]; int f[3] = { m % 3 - 1, (m / 3) % 3 - 1, m / 9 - 1 };
                printf(" %s", fstr(f).c_str());
            }
            printf("\n");
        }
        return 0;
    }
    // HISTORY: lazy safety game on beliefs.
    unordered_map<Bits, int, BH> id; vector<Bits> bel; vector<int> cu; vector<char> losing; vector<vector<int>> preds; vector<array<int, 3>> succ;
    vector<int> work;
    auto getid = [&](const Bits& b) { auto it = id.find(b); if (it != id.end()) return it->second; int n = bel.size(); id[b] = n; bel.push_back(b); cu.push_back(0); losing.push_back(0); preds.emplace_back(); succ.push_back({ -1, -1, -1 }); work.push_back(n); return n; };
    getid(init);
    size_t limit = 40000000;
    while (!work.empty()) {
        int n = work.back(); work.pop_back();
        if (losing[n]) continue;
        // preferred order: oppose the observation first
        int o;
        { int s0 = -1; Bits& b = bel[n]; for (int k = 0; k < NW && s0 < 0; k++) if (b[k]) s0 = k * 64 + __builtin_ctzll(b[k]); o = obs_of(s0) - 1; }
        int order[3]; order[0] = -o; order[1] = (o == 0) ? 1 : 0; order[2] = (o == 0) ? -1 : o;
        bool ok = false;
        while (cu[n] < 3) {
            int u = order[cu[n]]; int f[3] = { u, u, u };
            Bits T;
            Bits cur = bel[n];
            if (!apply(cur, f, T)) { cu[n]++; continue; }
            Bits part[3]; bool ne[3] = { 0, 0, 0 };
            for (int k = 0; k < 3; k++) part[k].assign(NW, 0);
            foreach_bit(T, [&](int s) { int k = obs_of(s); setb(part[k], s); ne[k] = true; });
            int sid[3] = { -1, -1, -1 }; bool bad = false;
            for (int k = 0; k < 3; k++) if (ne[k]) { auto it = id.find(part[k]); if (it != id.end() && losing[it->second]) bad = true; }
            if (bad) { cu[n]++; continue; }
            for (int k = 0; k < 3; k++) if (ne[k]) { sid[k] = getid(part[k]); preds[sid[k]].push_back(n); }
            succ[n] = { sid[0], sid[1], sid[2] };
            ok = true; break;
        }
        if (!ok) { losing[n] = 1; for (int p : preds[n]) if (!losing[p]) work.push_back(p); if (n == 0) break; }
        if (bel.size() > limit) { printf("d=%d B=%d HISTORY UNKNOWN (limit)\n", d, B); return 1; }
    }
    bool win = !losing[0];
    printf("d=%d B=%d HISTORY %s beliefs_created=%zu\n", d, B, win ? "FEASIBLE" : "INFEASIBLE", bel.size());
    if (win) {
        // extract reachable strategy automaton
        vector<int> mapid(bel.size(), -1), ord; ord.push_back(0); mapid[0] = 0;
        for (size_t i = 0; i < ord.size(); i++) { int n = ord[i]; for (int k = 0; k < 3; k++) { int s = succ[n][k]; if (s >= 0 && mapid[s] < 0) { mapid[s] = ord.size(); ord.push_back(s); } } }
        size_t maxsz = 0;
        for (int n : ord) { size_t c = 0; for (auto w : bel[n]) c += __builtin_popcountll(w); maxsz = max(maxsz, c); }
        printf("  strategy automaton: %zu belief nodes, max belief size %zu\n", ord.size(), maxsz);
        if (dump) {
            FILE* fp = fopen(argv[4], "w");
            fprintf(fp, "%d %d %zu\n", d, B, ord.size());
            for (int n : ord) {
                int s0 = -1; Bits& b = bel[n]; for (int k = 0; k < NW && s0 < 0; k++) if (b[k]) s0 = k * 64 + __builtin_ctzll(b[k]);
                int o = obs_of(s0) - 1; int order[3]; order[0] = -o; order[1] = (o == 0) ? 1 : 0; order[2] = (o == 0) ? -1 : o;
                fprintf(fp, "%d %d %d %d\n", order[cu[n]], succ[n][0] < 0 ? -1 : mapid[succ[n][0]], succ[n][1] < 0 ? -1 : mapid[succ[n][1]], succ[n][2] < 0 ? -1 : mapid[succ[n][2]]);
            }
            fclose(fp);
        }
    }
    return 0;
}
