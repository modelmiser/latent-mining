// Referee: literal game search. No memo, no abstraction, all 2^n subsets of U as questions.
// Alive set recomputed from the whole history: L_x = {s : answer s false for x} (forced by (i)),
// (ii) checked against stored ell_s (each computed from C_{s-1} when question s was asked).
// usage: litgame n k qmax [u1]    prints, for q=0..qmax, whether the questioner wins within q.
#include <cstdio>
#include <cstdlib>
int n, k, U1 = 0;
int Sh[32], Ah[32], ellh[32];
long long nodes = 0;
static int aliveMask(int t) {
    int C = 0;
    for (int x = 0; x < n; x++) {
        int L[32], m = 0;
        for (int s = 1; s <= t; s++) { int in = (Sh[s] >> x) & 1; if (in != Ah[s]) L[m++] = s; }
        bool ok = true;
        if (U1) ok = (m <= 1);
        else for (int i = 0; i < m && ok; i++) for (int j = i + 1; j < m; j++)
            if (L[j] <= L[i] + k * ellh[L[i]]) { ok = false; break; }
        if (ok) C |= 1 << x;
    }
    return C;
}
static bool win(int t, int q, int C) { // t questions asked, alive set C, questioner must win by time q
    nodes++;
    if (__builtin_popcount(C) == 1) return true;
    if (t == q) return false;
    for (int S = 0; S < (1 << n); S++) {
        int a = __builtin_popcount(S & C), b = __builtin_popcount(C & ~S);
        Sh[t + 1] = S; ellh[t + 1] = a < b ? a : b;
        bool good = true;
        for (int ans = 0; ans < 2 && good; ans++) {
            Ah[t + 1] = ans;
            int C2 = aliveMask(t + 1);
            if (C2 == 0) continue;            // illegal answer
            if (!win(t + 1, q, C2)) good = false;
        }
        if (good) return true;
    }
    return false;
}
int main(int argc, char** argv) {
    n = atoi(argv[1]); k = atoi(argv[2]); int qmax = atoi(argv[3]); if (argc > 4) U1 = 1;
    for (int q = 0; q <= qmax; q++) {
        nodes = 0; bool w = win(0, q, (1 << n) - 1);
        printf("%s n=%d k=%d q=%d win=%d nodes=%lld\n", U1 ? "U1" : "Q", n, k, q, (int)w, nodes); fflush(stdout);
        if (w) break;
    }
}
