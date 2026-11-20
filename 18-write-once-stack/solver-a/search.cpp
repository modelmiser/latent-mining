// Exhaustive search for a top-scheme on n cells surviving t operations.
// usage: search n t [prune=1] [sym=1] [nodelimit=0] [fwd=1] [rootempty=0]
// BFS over histories (config, stack) with lazy assignment of E(c,op), chronological backtracking.
// prune: reject c' when n - weight(c') < t - depth' (alternating pushes force one new cell per step).
// sym:   fresh cells (never 1 in any assigned target) are interchangeable -> only use lowest fresh ones.
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;
int n, t, PRUNE = 1, SYM = 1;
long long nodes = 0, NODELIM = 0;
int E[256][3], Q[256];
static unsigned char seen[256][1024];
struct Node { int c, code, d; };
vector<Node> q;
int usedmask = 0;
bool aborted = false;
int FWD = 1, ROOTEMPTY = 0;
struct Mem { int code, r; };
vector<Mem> mem[256];
static inline int trunc_(int code, int k) { // keep last k stack bits (sentinel form)
    int len = 31 - __builtin_clz(code);
    if (len <= k) return code;
    return (1 << k) | (code & ((1 << k) - 1));
}
// necessary condition: two histories in one config must have equal tops on every common continuation
static bool compatAll(int c2, int code2, int r2) {
    for (const Mem &m : mem[c2]) {
        if (m.r == t && !ROOTEMPTY) continue; // the unchecked start history: skip (sound: checks less)
        int k = min(m.r, r2) + 1;
        if (trunc_(m.code, k) != trunc_(code2, k)) return false;
    }
    return true;
}

bool solve(size_t i, int op);

bool step(size_t i, int op, int c2, int code2, int top, int d2) {
    // arrive at c2 with stack code2; check Q, enqueue, continue
    int oldQ = Q[c2];
    if (oldQ != -1 && oldQ != top) return false;
    Q[c2] = top;
    bool pushed = false;
    if (!seen[c2][code2]) {
        if (FWD && !compatAll(c2, code2, t - d2)) return false;
        seen[c2][code2] = 1; q.push_back({c2, code2, d2}); mem[c2].push_back({code2, t - d2}); pushed = true; }
    bool r = solve(i, op + 1);
    if (r) return true;
    if (pushed) { seen[c2][code2] = 0; q.pop_back(); mem[c2].pop_back(); }
    Q[c2] = oldQ;
    return false;
}

bool solve(size_t i, int op) {
    if (aborted) return false;
    if (i == q.size()) return true;
    Node nd = q[i];
    if (nd.d == t || op == 3) return solve(i + 1, 0);
    int code2, top;
    if (op == 2) {
        if (nd.code == 1) return solve(i, op + 1); // POP illegal on empty
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
    if (NODELIM && nodes > NODELIM) { aborted = true; return false; }
    int freeb = ((1 << n) - 1) & ~c;
    int fresh = ((1 << n) - 1) & ~usedmask;
    // enumerate subsets of freeb by increasing popcount
    vector<int> subs;
    for (int a = freeb;; a = (a - 1) & freeb) { subs.push_back(a); if (a == 0) break; }
    stable_sort(subs.begin(), subs.end(), [](int x, int y) { return __builtin_popcount(x) < __builtin_popcount(y); });
    for (int a : subs) {
        int c2 = c | a;
        if (PRUNE && n - __builtin_popcount(c2) < t - d2) continue;
        if (SYM) {
            int af = a & fresh; // must be the lowest popcount(af) bits of fresh
            int rest = fresh & ~af;
            if (af && rest && (rest & -rest) < (1 << (31 - __builtin_clz(af)))) continue;
        }
        if (Q[c2] != -1 && Q[c2] != top) continue;
        int oldused = usedmask;
        usedmask |= c2;
        E[c][op] = c2;
        if (step(i, op, c2, code2, top, d2)) return true;
        E[c][op] = -1;
        usedmask = oldused;
        if (aborted) return false;
    }
    return false;
}

int main(int argc, char **argv) {
    n = atoi(argv[1]); t = atoi(argv[2]);
    if (argc > 3) PRUNE = atoi(argv[3]);
    if (argc > 4) SYM = atoi(argv[4]);
    if (argc > 5) NODELIM = atoll(argv[5]);
    if (argc > 6) FWD = atoi(argv[6]);
    if (argc > 7) ROOTEMPTY = atoi(argv[7]);
    for (int c = 0; c < 256; c++) { Q[c] = -1; for (int o = 0; o < 3; o++) E[c][o] = -1; }
    seen[0][1] = 1; q.push_back({0, 1, 0}); mem[0].push_back({1, t});
    if (ROOTEMPTY) Q[0] = 2; // alternative reading: Q(0^n) must be EMPTY
    bool r = solve(0, 0);
    if (aborted) { printf("n=%d t=%d ABORTED nodes=%lld\n", n, t, nodes); return 2; }
    printf("n=%d t=%d %s nodes=%lld prune=%d sym=%d\n", n, t, r ? "SURVIVES" : "IMPOSSIBLE", nodes, PRUNE, SYM);
    if (r) {
        // witness: complete the tables (undefined E -> stay, undefined Q -> EMPTY)
        char fn[64]; sprintf(fn, "witness_n%d_t%d%s.txt", n, t, ROOTEMPTY ? "_rootempty" : "");
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
    return r ? 0 : 1;
}
