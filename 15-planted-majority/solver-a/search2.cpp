// search2.cpp -- faster exhaustive lazy search (same question as search.cpp, independent checker code).
// usage: search2 s N [all=0] [wmLo wmHi]
// Is there an s-state protocol winning every planted instance with 2 <= a+b <= N ?
// Reductions (proofs.txt, Lemma R): qA=0,qB=1; unordered swap-free rule (one outcome multiset per unordered pair).
// Symmetry pruning on the output map only: (i) mirror A<->B (states 0<->1, outputs flipped),
// (ii) permutations of states 2..s-1  => only output masks that are minimal in their orbit are searched.
// Lazy: an entry is assigned only when a reachable configuration needs it.  Resume: instances that passed
// without touching an undefined entry are not re-checked deeper in the recursion.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;
int s, N; int w[8]; int d[8][8]; // d[p][q] = outcome lo*s+hi, or -1 ; identity encoded as min(p,q)*s+max(p,q)
struct Inst { int a, b, x, n; };
vector<Inst> insts;
// per population size n: list of configs (packed 4 bits per state)
vector<vector<uint32_t>> cfgs(40); vector<unordered_map<uint32_t,int>> cid(40);
void gen(int n) { if (!cfgs[n].empty()) return; vector<int> c(s, 0);
    // enumerate compositions
    struct R { static void go(int i, int left, uint32_t key, int s, vector<uint32_t>& out) { if (i == s - 1) { out.push_back(key | ((uint32_t)left << (4 * i))); return; } for (int k = 0; k <= left; k++) go(i + 1, left - k, key | ((uint32_t)k << (4 * i)), s, out); } };
    R::go(0, n, 0, s, cfgs[n]); for (size_t i = 0; i < cfgs[n].size(); i++) cid[n][cfgs[n][i]] = (int)i; }
int needp, needq;
vector<int> seenStamp, order, estart, edges, idxv, lowv, ptrv; vector<char> goodv, onst; int stamp = 0;
// returns 0 PASS, 1 FAIL, 2 NEED
int checkInst(const Inst& I) {
    int n = I.n; int maj = I.a > I.b ? 0 : 1; const vector<uint32_t>& L = cfgs[n]; auto& M = cid[n];
    int tot = (int)L.size();
    if ((int)seenStamp.size() < tot) { seenStamp.assign(tot, 0); stamp = 0; }
    stamp++;
    vector<int> local(tot, -1); // config id -> local node
    order.clear(); estart.clear(); edges.clear(); goodv.clear();
    uint32_t k0 = 0; k0 += (uint32_t)I.a << 0; k0 += (uint32_t)I.b << 4; k0 += 1u << (4 * I.x);
    int i0 = M[k0]; local[i0] = 0; order.push_back(i0); bool need = false;
    for (size_t i = 0; i < order.size(); i++) {
        uint32_t key = L[order[i]]; estart.push_back((int)edges.size());
        bool g = true, legal = false, incomplete = false;
        for (int q = 0; q < s; q++) if (((key >> (4 * q)) & 15) && w[q] != maj) g = false;
        for (int p = 0; p < s; p++) { int cp = (key >> (4 * p)) & 15; if (!cp) continue;
            for (int q = p; q < s; q++) { int cq = (key >> (4 * q)) & 15; if (!cq) continue; if (p == q && cp < 2) continue;
                int v = d[p][q];
                if (v < 0) { if (!need) { need = true; needp = p; needq = q; } incomplete = true; continue; }
                if (v == p * s + q) continue;
                legal = true; int x = v / s, y = v % s;
                uint32_t nk = key - (1u << (4 * p)) - (1u << (4 * q)) + (1u << (4 * x)) + (1u << (4 * y));
                int j = M[nk]; if (local[j] < 0) { local[j] = (int)order.size(); order.push_back(j); }
                edges.push_back(local[j]);
            } }
        goodv.push_back(g);
        if (!g && !legal && !incomplete) return 1;
    }
    estart.push_back((int)edges.size());
    int m = (int)order.size();
    idxv.assign(m, -1); lowv.assign(m, 0); ptrv.assign(m, 0); onst.assign(m, 0);
    vector<int> st, cs; int counter = 0;
    for (int r = 0; r < m; r++) if (idxv[r] < 0) {
        cs.push_back(r); idxv[r] = lowv[r] = counter++; st.push_back(r); onst[r] = 1; ptrv[r] = estart[r];
        while (!cs.empty()) { int v = cs.back();
            if (ptrv[v] < estart[v + 1]) { int u = edges[ptrv[v]++];
                if (idxv[u] < 0) { idxv[u] = lowv[u] = counter++; st.push_back(u); onst[u] = 1; ptrv[u] = estart[u]; cs.push_back(u); }
                else if (onst[u]) lowv[v] = min(lowv[v], idxv[u]); }
            else { cs.pop_back(); if (!cs.empty()) lowv[cs.back()] = min(lowv[cs.back()], lowv[v]);
                if (lowv[v] == idxv[v]) { int sz = 0; bool bad = false; while (true) { int u = st.back(); st.pop_back(); onst[u] = 0; sz++; if (!goodv[u]) bad = true; if (u == v) break; } if (sz > 1 && bad) return 1; } } } }
    return need ? 2 : 0;
}
long long nodesVisited = 0, found = 0; bool wantAll = false;
void printProto() {
    printf("FOUND N=%d: s=%d qA=0 qB=1 w=", N, s); for (int q = 0; q < s; q++) printf("%c", w[q] ? 'B' : 'A'); printf(" d:");
    for (int p = 0; p < s; p++) for (int q = 0; q < s; q++) { int v = d[min(p, q)][max(p, q)]; if (v < 0) printf(" %d,%d->?", p, q); else if (v != min(p, q) * s + max(p, q)) printf(" %d,%d->%d,%d", p, q, v / s, v % s); }
    printf("\n"); fflush(stdout);
}
bool solve(int from) {
    nodesVisited++;
    int r = 0; int i = from;
    for (; i < (int)insts.size(); i++) { r = checkInst(insts[i]); if (r) break; }
    if (r == 1) return false;
    if (r == 0) { found++; if (found == 1 || wantAll) printProto(); return !wantAll; }
    int p = needp, q = needq;
    for (int x = 0; x < s; x++) for (int y = x; y < s; y++) { d[p][q] = x * s + y; if (solve(i)) return true; }
    d[p][q] = -1; return false;
}
int maskOf(const int* ww) { int m = 0; for (int q = 0; q < s; q++) m |= ww[q] << q; return m; }
bool canonical(int wm) {
    int base[8]; for (int q = 0; q < s; q++) base[q] = (wm >> q) & 1;
    vector<int> perm; for (int q = 2; q < s; q++) perm.push_back(q);
    do { int a[8], b[8]; a[0] = base[0]; a[1] = base[1]; for (int q = 2; q < s; q++) a[q] = base[perm[q - 2]];
        b[0] = 1 - a[1]; b[1] = 1 - a[0]; for (int q = 2; q < s; q++) b[q] = 1 - a[q];
        if (maskOf(a) < wm || maskOf(b) < wm) return false; } while (next_permutation(perm.begin(), perm.end()));
    return true;
}
int main(int argc, char** argv) {
    s = atoi(argv[1]); N = atoi(argv[2]); if (argc > 3) wantAll = atoi(argv[3]);
    int lo = 0, hi = (1 << s); if (argc > 5) { lo = atoi(argv[4]); hi = atoi(argv[5]); }
    for (int t = 2; t <= N; t++) for (int a = 0; a <= t; a++) { int b = t - a; if (abs(a - b) < 2) continue; for (int x = 0; x < s; x++) insts.push_back({a, b, x, t + 1}); }
    for (int n = 3; n <= N + 1; n++) gen(n);
    bool ok = false;
    for (int wm = lo; wm < hi && !ok; wm++) {
        if (!getenv("NOCANON") && !canonical(wm)) continue;
        if (wm == 0 || wm == (1 << s) - 1) continue; // an output letter that no state has can never be shown: every instance of that majority fails
        for (int q = 0; q < s; q++) w[q] = (wm >> q) & 1;
        memset(d, -1, sizeof d);
        long long before = nodesVisited; ok = solve(0);
        fprintf(stderr, "  wm=%d nodes=%lld found=%lld\n", wm, nodesVisited - before, found);
    }
    printf("s=%d N=%d masks[%d,%d) result=%s found=%lld searchnodes=%lld\n", s, N, lo, hi, found ? "EXISTS" : "NONE", found, nodesVisited);
    return 0;
}
