// search.cpp -- exhaustive lazy search: is there an s-state protocol winning all instances with a+b <= N ?
// usage: search s N [maxnodes] [all]
// Reductions (proved in proofs.txt, Lemma R): wlog qA = 0, qB = 1; wlog the rule is "unordered":
// one outcome multiset per unordered pair {p,q}, no swaps.  All 2^s output maps are tried.
// An entry is assigned only when a reachable configuration needs it.
#include "pp.h"
Proto P; Checker C; int N; long long nodesVisited = 0, maxNodes = -1; bool aborted = false;
long long found = 0; bool wantAll = false;
void printProto(FILE* f) {
    fprintf(f, "s=%d qA=%d qB=%d w=", P.s, P.qA, P.qB);
    for (int q = 0; q < P.s; q++) fprintf(f, "%c", P.w[q] ? 'B' : 'A');
    fprintf(f, " d:");
    for (int p = 0; p < P.s; p++) for (int q = 0; q < P.s; q++) { if (P.d[p][q] < 0) fprintf(f, " %d,%d->?", p, q); else if (P.d[p][q] != p * P.s + q) fprintf(f, " %d,%d->%d,%d", p, q, P.d[p][q] / P.s, P.d[p][q] % P.s); }
    fprintf(f, "\n");
}
bool solve() {
    nodesVisited++;
    if (maxNodes >= 0 && nodesVisited > maxNodes) { aborted = true; return false; }
    int r = C.checkUpTo(N);
    if (r == 1) return false;
    if (r == 0) { found++; if (found == 1 || wantAll) { printf("FOUND N=%d: ", N); printProto(stdout); fflush(stdout); } return !wantAll; }
    int p = C.needp, q = C.needq; int s = P.s;
    for (int x = 0; x < s; x++) for (int y = x; y < s; y++) {
        if ((x == p && y == q) || (x == q && y == p)) { P.d[p][q] = p * s + q; P.d[q][p] = q * s + p; }
        else { P.d[p][q] = P.d[q][p] = x * s + y; }
        if (solve()) return true; if (aborted) break;
    }
    P.d[p][q] = P.d[q][p] = -1; return false;
}
int main(int argc, char** argv) {
    int s = atoi(argv[1]); N = atoi(argv[2]); if (argc > 3) maxNodes = atoll(argv[3]); if (argc > 4) wantAll = atoi(argv[4]);
    P.s = s; P.qA = 0; P.qB = 1; C.P = &P; C.partial = true;
    bool ok = false;
    for (int wm = 0; wm < (1 << s) && !ok && !aborted; wm++) {
        for (int q = 0; q < s; q++) P.w[q] = (wm >> q) & 1;
        for (int p = 0; p < s; p++) for (int q = 0; q < s; q++) P.d[p][q] = -1;
        ok = solve();
    }
    printf("s=%d N=%d result=%s found=%lld searchnodes=%lld\n", s, N, aborted ? "ABORTED" : (found ? "EXISTS" : "NONE"), found, nodesVisited);
    return 0;
}
