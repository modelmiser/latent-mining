// Clock-class solver, memory-lean.  usage: clock d B prune log2cap [order]
// DFS over reachable-state sets S_t. Dead sets are remembered by 128-bit hash only
// (so INFEASIBLE verdicts are exact up to a ~2^-100 hash-collision chance; a collision
// could only turn a live set dead, i.e. produce a false INFEASIBLE).
// prune = 0: exact sets.
// prune = 1: after each step keep, per (sign-history, flag), only min and max x.
// prune = 2: keep, per (sign-history, flag, sgn x), only min and max x.
//   Pruned sets are SUBSETS of the true reachable sets and the set map is monotone, so
//   INFEASIBLE under pruning is a sound lower bound; FEASIBLE under pruning is only a
//   candidate and the printed clock must be re-verified exactly (verify.py clockwit).
// Negation symmetry: dead(S) <=> dead(-S); keys are canonicalised.
#include <bits/stdc++.h>
using namespace std;
typedef vector<uint64_t> Bits;
typedef unsigned __int128 u128;
int d, B, P3, NS, NW, PRUNE;
inline int enc(int x, int code, int fl) { return ((x + B) * P3 + code) * 2 + fl; }
inline int sg(int x) { return x < 0 ? 0 : (x == 0 ? 1 : 2); }
inline int obs_of(int s) { return ((s >> 1) % P3) / (P3 / 3); }
inline void setb(Bits& b, int i) { b[i >> 6] |= 1ULL << (i & 63); }
inline bool getb(const Bits& b, int i) { return b[i >> 6] >> (i & 63) & 1; }
template <class F> void foreach_bit(const Bits& b, F f) {
    for (int k = 0; k < NW; k++) { uint64_t w = b[k]; while (w) { int j = __builtin_ctzll(w); f(k * 64 + j); w &= w - 1; } }
}
vector<int> negcode; // sign-history code under x -> -x
int mirror_state(int s) { int fl = s & 1, r = s >> 1, code = r % P3, x = r / P3 - B; return enc(-x, negcode[code], fl); }
Bits mirror(const Bits& S) { Bits T(NW, 0); foreach_bit(S, [&](int s) { setb(T, mirror_state(s)); }); return T; }
u128 hsh(const Bits& b) {
    uint64_t h1 = 0x9E3779B97F4A7C15ULL, h2 = 0xC2B2AE3D27D4EB4FULL;
    for (auto w : b) {
        h1 ^= w; h1 *= 0xFF51AFD7ED558CCDULL; h1 ^= h1 >> 32; h1 *= 0x9FB21C651E98DF25ULL; h1 ^= h1 >> 29;
        h2 += w * 0x9E3779B97F4A7C15ULL; h2 ^= h2 >> 31; h2 *= 0xD6E8FEB86659FD93ULL; h2 ^= h2 >> 28;
    }
    return ((u128)h1 << 64) | h2;
}
u128 canon(const Bits& S) { u128 a = hsh(S), b = hsh(mirror(S)); u128 r = a < b ? a : b; return r ? r : 1; }
// image of the obs-class o of S under control u (all adversary moves); false if leaves band
bool image(const Bits& S, int o, int u, Bits& out) {
    bool ok = true;
    foreach_bit(S, [&](int s) {
        if (!ok || obs_of(s) != o) return;
        int fl = s & 1, r = s >> 1, code = r % P3, x = r / P3 - B;
        int ncode = (code % (P3 / 3)) * 3 + sg(x);
        for (int w = -1; w <= 1; w++) {
            if (fl && w) continue;
            int nx = x + u + w;
            if (nx < -B || nx > B) { ok = false; return; }
            setb(out, enc(nx, ncode, w != 0));
        }
    });
    return ok;
}
void prune(Bits& S) {
    if (!PRUNE) return;
    Bits T(NW, 0);
    for (int code = 0; code < P3; code++) for (int fl = 0; fl < 2; fl++) {
        if (PRUNE == 1) {
            int lo = 99, hi = -99;
            for (int x = -B; x <= B; x++) if (getb(S, enc(x, code, fl))) { lo = min(lo, x); hi = max(hi, x); }
            if (lo <= hi) { setb(T, enc(lo, code, fl)); setb(T, enc(hi, code, fl)); }
        } else {
            int lo = 99, hi = -99;
            for (int x = -B; x < 0; x++) if (getb(S, enc(x, code, fl))) { lo = min(lo, x); hi = max(hi, x); }
            if (lo <= hi) { setb(T, enc(lo, code, fl)); setb(T, enc(hi, code, fl)); }
            if (getb(S, enc(0, code, fl))) setb(T, enc(0, code, fl));
            lo = 99, hi = -99;
            for (int x = 1; x <= B; x++) if (getb(S, enc(x, code, fl))) { lo = min(lo, x); hi = max(hi, x); }
            if (lo <= hi) { setb(T, enc(lo, code, fl)); setb(T, enc(hi, code, fl)); }
        }
    }
    S = T;
}
// open-addressing dead table
vector<u128> tab; uint64_t tmask; size_t tcount = 0;
bool tfind(u128 k) { uint64_t i = (uint64_t)(k >> 64) & tmask; while (tab[i]) { if (tab[i] == k) return true; i = (i + 1) & tmask; } return false; }
void tins(u128 k) { uint64_t i = (uint64_t)(k >> 64) & tmask; while (tab[i]) { if (tab[i] == k) return; i = (i + 1) & tmask; } tab[i] = k; tcount++; }
string fstr(const int* f) { const char* c = "-0+"; string s; for (int i = 0; i < 3; i++) s += c[f[i] + 1]; return s; }

int main(int argc, char** argv) {
    d = atoi(argv[1]); B = atoi(argv[2]); PRUNE = atoi(argv[3]); int lg = atoi(argv[4]);
    int ordermode = argc > 5 ? atoi(argv[5]) : 0;
    P3 = 1; for (int i = 0; i < d; i++) P3 *= 3;
    NS = (2 * B + 1) * P3 * 2; NW = (NS + 63) / 64;
    negcode.resize(P3);
    for (int c = 0; c < P3; c++) { int r = c, n = 0, p = 1; for (int i = 0; i < d; i++) { n += (2 - r % 3) * p; p *= 3; r /= 3; } negcode[c] = n; }
    tab.assign((size_t)1 << lg, 0); tmask = ((uint64_t)1 << lg) - 1;
    Bits init(NW, 0); setb(init, enc(0, (P3 - 1) / 2, 0));
    struct Fr { Bits S; u128 key; int m; int nopt; vector<Bits> opts; vector<int> optm; };
    // map order: try "restoring" maps first: f(-) descending, f(+) ascending
    vector<int> mapord;
    for (int m = 0; m < 27; m++) mapord.push_back(m);
    auto score = [&](int m) { int fm = m % 3 - 1, f0 = (m / 3) % 3 - 1, fp = m / 9 - 1; return ordermode == 0 ? -(fm - fp) * 3 + abs(f0) : (ordermode == 1 ? m : -m); };
    stable_sort(mapord.begin(), mapord.end(), [&](int a, int b) { return score(a) < score(b); });
    vector<Fr> stack; vector<int> choice;
    unordered_map<uint64_t, vector<pair<u128, int>>> onstack; // key -> depth
    auto os_find = [&](u128 k) { auto it = onstack.find((uint64_t)k); if (it == onstack.end()) return -1; for (auto& p : it->second) if (p.first == k) return p.second; return -1; };
    auto os_add = [&](u128 k, int dep) { onstack[(uint64_t)k].push_back({ k, dep }); };
    auto os_del = [&](u128 k) { auto& v = onstack[(uint64_t)k]; for (size_t i = 0; i < v.size(); i++) if (v[i].first == k) { v.erase(v.begin() + i); break; } if (v.empty()) onstack.erase((uint64_t)k); };
    auto expand = [&](Fr& fr) {
        // 9 partial images
        Bits img[3][3]; bool okk[3][3]; bool used[3] = { 0, 0, 0 };
        foreach_bit(fr.S, [&](int s) { used[obs_of(s)] = true; });
        for (int o = 0; o < 3; o++) for (int u = 0; u < 3; u++) { img[o][u].assign(NW, 0); okk[o][u] = used[o] ? image(fr.S, o, u - 1, img[o][u]) : (u == 1); }
        for (int m : mapord) {
            int f[3] = { m % 3, (m / 3) % 3, m / 9 };
            if (!okk[0][f[0]] || !okk[1][f[1]] || !okk[2][f[2]]) continue;
            Bits T(NW, 0);
            for (int o = 0; o < 3; o++) if (used[o]) for (int k = 0; k < NW; k++) T[k] |= img[o][f[o]][k];
            prune(T);
            fr.opts.push_back(T); fr.optm.push_back(m);
        }
    };
    Fr f0; f0.S = init; f0.key = canon(init); f0.m = 0; expand(f0);
    stack.push_back(f0); os_add(f0.key, 0);
    bool found = false; int cycdepth = -1; bool mirrored = false; size_t maxdepth = 0; uint64_t nodes = 1;
    while (!stack.empty() && !found) {
        Fr& fr = stack.back(); maxdepth = max(maxdepth, stack.size());
        bool pushed = false;
        while (fr.m < (int)fr.opts.size()) {
            int i = fr.m++;
            Bits& T = fr.opts[i];
            u128 k = canon(T);
            if (tfind(k)) continue;
            int dep = os_find(k);
            if (dep >= 0) { found = true; cycdepth = dep; mirrored = !(stack[dep].S == T); choice.push_back(fr.optm[i]); break; }
            Fr nf; nf.S = T; nf.key = k; nf.m = 0; expand(nf);
            choice.push_back(fr.optm[i]);
            stack.push_back(std::move(nf)); os_add(k, stack.size() - 1); nodes++; pushed = true;
            break;
        }
        if (found) break;
        if (!pushed) {
            os_del(stack.back().key); tins(stack.back().key); stack.pop_back(); if (!choice.empty()) choice.pop_back();
            if (tcount * 10 > tab.size() * 7) { printf("d=%d B=%d prune=%d CLOCK UNKNOWN (dead table full, %zu)\n", d, B, PRUNE, tcount); return 2; }
        }
        if ((nodes & 0xFFFFF) == 0 && pushed) { fprintf(stderr, "nodes=%lu dead=%zu depth=%zu\n", (unsigned long)nodes, tcount, stack.size()); }
    }
    printf("d=%d B=%d prune=%d CLOCK %s sets_visited=%lu maxdepth=%zu\n", d, B, PRUNE, found ? (PRUNE ? "CANDIDATE" : "FEASIBLE") : "INFEASIBLE", (unsigned long)nodes, maxdepth);
    if (found) {
        vector<int> pre(choice.begin(), choice.begin() + cycdepth), cyc(choice.begin() + cycdepth, choice.end());
        if (mirrored) { size_t n = cyc.size(); for (size_t i = 0; i < n; i++) { int m = cyc[i]; int f[3] = { m % 3 - 1, (m / 3) % 3 - 1, m / 9 - 1 }; int g[3] = { -f[2], -f[1], -f[0] }; cyc.push_back((g[0] + 1) + 3 * (g[1] + 1) + 9 * (g[2] + 1)); } }
        printf("  preperiod=%zu period=%zu\n  prefix:", pre.size(), cyc.size());
        for (int m : pre) { int f[3] = { m % 3 - 1, (m / 3) % 3 - 1, m / 9 - 1 }; printf(" %s", fstr(f).c_str()); }
        printf("\n  cycle:");
        for (int m : cyc) { int f[3] = { m % 3 - 1, (m / 3) % 3 - 1, m / 9 - 1 }; printf(" %s", fstr(f).c_str()); }
        printf("\n");
    }
    return 0;
}
