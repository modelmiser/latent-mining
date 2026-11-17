// slow.cpp -- literal implementation of the definition, no memo, no state abstraction.
// A node is the full history (S_1..S_t as bitmasks over U, answers). V_t is recomputed from scratch:
// for every x, L = {s : answer_s != [x in S_s]} (forced by (i)); alive iff (ii) holds, where
// ell_s is computed from C_{s-1}, itself recomputed from scratch.
// usage: slow K N Q   -> prints whether questioner wins within Q questions
//        slow U1 N Q  -> same for the at-most-one-lie game
#include <bits/stdc++.h>
using namespace std;
int K, n; bool U1mode = false;
int S[32], A[32];
long long nodes = 0;

// alive set after t questions, by the definition
int aliveSet(int t) {
    if (U1mode) {
        int C = 0;
        for (int x = 0; x < n; x++) {
            int lies = 0;
            for (int s = 1; s <= t; s++) if (((S[s] >> x) & 1) != A[s]) lies++;
            if (lies <= 1) C |= 1 << x;
        }
        return C;
    }
    int ell[32];
    int Cprev = (1 << n) - 1;   // C_0
    int C = Cprev;
    for (int u = 1; u <= t; u++) {
        ell[u] = min(__builtin_popcount(S[u] & Cprev), __builtin_popcount(Cprev & ~S[u]));
        // C_u: pairs valid w.r.t. questions 1..u
        C = 0;
        for (int x = 0; x < n; x++) {
            bool ok = true;
            for (int s = 1; s <= u && ok; s++) {
                if (((S[s] >> x) & 1) == A[s]) continue; // not a lie
                for (int w = s + 1; w <= u && w <= s + K * ell[s]; w++)
                    if (((S[w] >> x) & 1) != A[w]) { ok = false; break; }
            }
            if (ok) C |= 1 << x;
        }
        Cprev = C;
    }
    return C;
}
bool win(int t, int q) { // t questions asked, q remaining
    nodes++;
    int C = aliveSet(t);
    if (__builtin_popcount(C) == 1) return true;
    if (q == 0) return false;
    for (int s = 0; s < (1 << n); s++) {
        S[t + 1] = s;
        bool ok = true;
        for (int a = 0; a < 2 && ok; a++) {
            A[t + 1] = a;
            int C2 = aliveSet(t + 1);
            if (C2 == 0) continue;      // responder may not empty C
            if (!win(t + 1, q - 1)) ok = false;
        }
        if (ok) return true;
    }
    return false;
}
int main(int argc, char** argv) {
    if (string(argv[1]) == "U1") U1mode = true; else K = atoi(argv[1]);
    n = atoi(argv[2]); int q = atoi(argv[3]);
    bool w = win(0, q);
    printf("%s n=%d q=%d win=%d nodes=%lld\n", argv[1], n, q, (int)w, nodes);
}
