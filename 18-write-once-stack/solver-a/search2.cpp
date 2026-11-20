// search2: same exhaustive search as search.cpp, plus a bottom-up lemma table
//   M[r][s] = least number of cells with which a scheme can start in a config whose Q is already
//             fixed to top(s), with stack s (only its last r+1 bits matter) and r operations to go.
// Pruning in a larger instance: a history arriving at config c2 with stack s2 and r2 ops to go needs
//   n - weight(c2) >= M[r2][trunc(s2, r2+1)]   (the part of the scheme above c2 is such a sub-scheme
//   on the n - weight(c2) cells still 0 in c2).  Sound: prunes only infeasible branches.
// usage: search2 T [rootempty=0] [maxn=10]
//   prints M tables for r < T, then Mroot(t) = least n surviving t from the real start, for t <= T.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
int n, t;
long long nodes = 0;
int E[1024][3], Q[1024];
static unsigned char seen[1024][2048];
struct Node { int c, code, d; };
vector<Node> q;
int usedmask = 0;
int ROOTSKIP = 0; // 1: root history has unchecked Q (the real start under the literal reading)
struct Mem { int code, r; };
vector<Mem> mem[1024];
int M[12][4096]; // M[r][code]
int MAXR = -1;   // tables valid for r <= MAXR

static inline int trunc_(int code, int k) {
    int len = 31 - __builtin_clz(code);
    if (len <= k) return code;
    return (1 << k) | (code & ((1 << k) - 1));
}
static bool compatAll(int c2, int code2, int r2) {
    for (const Mem &m : mem[c2]) {
        if (m.r == t && ROOTSKIP) continue;
        int k = min(m.r, r2) + 1;
        if (trunc_(m.code, k) != trunc_(code2, k)) return false;
    }
    return true;
}
bool solve(size_t i, int op);
bool step(size_t i, int op, int c2, int code2, int top, int d2) {
    int oldQ = Q[c2];
    if (oldQ != -1 && oldQ != top) return false;
    bool pushed = false;
    if (!seen[c2][code2]) {
        int r2 = t - d2;
        if (r2 <= MAXR && n - __builtin_popcount(c2) < M[r2][trunc_(code2, r2 + 1)]) return false;
        if (!compatAll(c2, code2, r2)) return false;
        seen[c2][code2] = 1; q.push_back({c2, code2, d2}); mem[c2].push_back({code2, r2}); pushed = true;
    }
    Q[c2] = top;
    bool r = solve(i, op + 1);
    if (r) return true;
    if (pushed) { seen[c2][code2] = 0; q.pop_back(); mem[c2].pop_back(); }
    Q[c2] = oldQ;
    return false;
}
bool solve(size_t i, int op) {
    if (i == q.size()) return true;
    Node nd = q[i];
    if (nd.d == t || op == 3) return solve(i + 1, 0);
    int code2, top;
    if (op == 2) {
        if (nd.code == 1) return solve(i, op + 1);
        code2 = nd.code >> 1;
        top = (code2 == 1) ? 2 : (code2 & 1);
    } else {
        code2 = (nd.code << 1) | op;
        top = op;
    }
    int d2 = nd.d + 1;
    int c = nd.c;
    if (E[c][op] != -1) return step(i, op, E[c][op], code2, top, d2);
    ++nodes;
    int full = (1 << n) - 1;
    int freeb = full & ~c;
    int fresh = full & ~usedmask;
    int subs[1024], ns = 0;
    for (int a = freeb;; a = (a - 1) & freeb) { subs[ns++] = a; if (a == 0) break; }
    stable_sort(subs, subs + ns, [](int x, int y) { return __builtin_popcount(x) < __builtin_popcount(y); });
    for (int k = 0; k < ns; k++) {
        int a = subs[k];
        int c2 = c | a;
        if (n - __builtin_popcount(c2) < t - d2) continue;
        int af = a & fresh;
        int rest = fresh & ~af;
        if (af && rest && (rest & -rest) < (1 << (31 - __builtin_clz(af)))) continue;
        if (Q[c2] != -1 && Q[c2] != top) continue;
        int oldused = usedmask;
        usedmask |= c2;
        E[c][op] = c2;
        if (step(i, op, c2, code2, top, d2)) return true;
        E[c][op] = -1;
        usedmask = oldused;
    }
    return false;
}
// does a scheme on nn cells exist for start stack s0, budget r, root Q checked (or not)?
bool feasible(int nn, int s0, int r, int rootskip, bool dump, const char *tag) {
    n = nn; t = r; ROOTSKIP = rootskip;
    for (int c = 0; c < (1 << n); c++) {
        Q[c] = -1; for (int o = 0; o < 3; o++) E[c][o] = -1; mem[c].clear();
        memset(seen[c], 0, sizeof seen[c]);
    }
    q.clear(); usedmask = 0;
    seen[0][s0] = 1; q.push_back({0, s0, 0}); mem[0].push_back({s0, r});
    if (!rootskip) Q[0] = (s0 == 1) ? 2 : (s0 & 1);
    bool ok = solve(0, 0);
    if (ok && dump) {
        char fn[64]; sprintf(fn, "witness_n%d_t%d%s.txt", n, t, tag);
        FILE *f = fopen(fn, "w");
        fprintf(f, "# top-scheme n=%d surviving t=%d. Config = n-char binary string, cell 1 first.\n", n, t);
        fprintf(f, "# line: config Q E(PUSH0) E(PUSH1) E(POP); unlisted configs: Q=EMPTY, all E = stay\n");
        fprintf(f, "n %d t %d\n", n, t);
        const char *qs[] = {"0", "1", "EMPTY"};
        for (int c = 0; c < (1 << n); c++) {
            bool any = Q[c] != -1; for (int o = 0; o < 3; o++) any |= E[c][o] != -1;
            if (!any) continue;
            auto pr = [&](int x) { for (int b = 0; b < n; b++) fputc('0' + ((x >> b) & 1), f); };
            pr(c); fprintf(f, " %s", qs[Q[c] == -1 ? 2 : Q[c]]);
            for (int o = 0; o < 3; o++) { fputc(' ', f); pr(E[c][o] == -1 ? c : E[c][o]); }
            fputc('\n', f);
        }
        fclose(f);
    }
    return ok;
}
int main(int argc, char **argv) {
    int T = atoi(argv[1]);
    int rootempty = argc > 2 ? atoi(argv[2]) : 0;
    int maxn = argc > 3 ? atoi(argv[3]) : 10;
    for (int r = 0; r < T; r++) {
        printf("M[r=%d]:", r); fflush(stdout);
        long long tot = 0;
        for (int code = 1; code < (1 << (r + 2)); code++) {
            int len = 31 - __builtin_clz(code);
            int comp = code ^ ((1 << len) - 1); // complement symmetry
            if (comp < code) { M[r][code] = M[r][comp]; }
            else {
                int lb = r;
                if (r > 0) for (int op = 0; op < 3; op++) {
                    if (op == 2 && code == 1) continue;
                    int c2 = op == 2 ? code >> 1 : (code << 1) | op;
                    lb = max(lb, M[r - 1][trunc_(c2, r)]);
                }
                int nn = lb;
                nodes = 0;
                while (nn <= maxn && !feasible(nn, code, r, 0, false, "")) nn++;
                tot += nodes;
                M[r][code] = nn;
            }
            // print as stack string (bottom..top), '-' for empty
            printf(" ");
            if (len == 0) printf("-");
            for (int b = len - 1; b >= 0; b--) printf("%d", (code >> b) & 1);
            printf("=%d", M[r][code]); fflush(stdout);
        }
        printf("  [nodes %lld]\n", tot);
        MAXR = r;
    }
    for (int tt = 0; tt <= T; tt++) {
        int nn = tt > 0 ? tt - 1 : 0;
        if (nn < 1) nn = 1;
        MAXR = min(MAXR, T - 1);
        int savedMAXR = MAXR; MAXR = min(MAXR, tt - 1);
        nodes = 0;
        while (nn <= maxn) {
            long long before = nodes;
            bool ok = feasible(nn, 1, tt, rootempty ? 0 : 1, true, rootempty ? "_rootempty" : "");
            printf("t=%d n=%d %s nodes=%lld\n", tt, nn, ok ? "SURVIVES" : "IMPOSSIBLE", nodes - before); fflush(stdout);
            if (ok) break;
            nn++;
        }
        printf("Mroot(t=%d) = %d  (rootempty=%d)\n", tt, nn, rootempty); fflush(stdout);
        MAXR = savedMAXR;
    }
    return 0;
}
