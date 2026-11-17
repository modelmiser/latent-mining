// open-f-upper/dp4.cpp : exact optimum of the restricted problem of opt4.cpp for LARGE M, by dynamic programming.
// Same position P(M,cls), same adversary script (free every second 2 [parity par], greedy-minimum thinning of 1s, 3s),
// same prescient manager (any net relocation of <= k ones), EXCEPT destinations are restricted to cells of [0,M)
// (surplus 1s are "dumped" for free: in the real game they would sit in freed 2-holes, where the thinning frees them).
// So dp4 >= the true restricted optimum only through that restriction; it is cross-checked against opt4 exact for M<=48.
// Quantity maximised: v = (#1s the adversary must keep) + 3*(free 3-slots);  final extent = e2 + 3*max(0, floor((C - v)/3)).
// usage: dp4 M cls par
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char** argv) { int M = atoi(argv[1]), cls = atoi(argv[2]), par = atoi(argv[3]);
    int n1 = 0; vector<int> base(M); for (int i = 0; i < M; i++) { base[i] = (i % 3 != cls); n1 += base[i]; }
    int n2 = (M - n1) / 2, k = 0; for (int i = 0; i < n2; i++) if (i % 2 == par) k++;
    int Z = M + (par == 0 ? 2 : 0); // if the 2 next to the zone is freed, its two cells join the zone as fixed free cells
    int e2 = M + 2 * n2 - (((n2 - 1) % 2 == par) ? 2 : 0); int C = M - 2 * (n2 - k);
    // state index: r (0..k), a (0..k), o1,o0,k1,k0 (4 bits), R (0..2)
    auto idx = [&](int r, int a, int bits, int R) { return ((r * (k + 1) + a) * 16 + bits) * 3 + R; };
    size_t N = (size_t)(k + 1) * (k + 1) * 48; const short NEG = -30000; vector<short> cur(N, NEG), nxt(N, NEG);
    cur[idx(0, 0, 0, 0)] = 0; // before cell 0: pretend two kept-free... handled by i<2 guards
    for (int i = 0; i < Z; i++) { fill(nxt.begin(), nxt.end(), NEG);
        for (int r = 0; r <= k; r++) for (int a = 0; a <= k; a++) for (int bits = 0; bits < 16; bits++) for (int R = 0; R < 3; R++) { short v = cur[idx(r, a, bits, R)]; if (v == NEG) continue;
            int o2 = bits >> 3 & 1, o1 = bits >> 2 & 1, k2 = bits >> 1 & 1, k1 = bits & 1; // cells i-2, i-1
            int b = i < M ? base[i] : 0;
            for (int o0 = 0; o0 < 2; o0++) { int r2 = r, a2 = a; if (i >= M && o0) continue; if (b && !o0) r2++; if (!b && o0) a2++; if (r2 > k || a2 > k) continue;
                int K2 = k2, K1 = k1, K0 = 0, vv = v, RR = R;
                if (i >= 2 && (o2 | o1 | o0) && !(K2 | K1)) { vv++; if (o0) K0 = 1; else if (o1) K1 = 1; else K2 = 1; }
                if (i >= 2) { if (K2) RR = 0; else { RR++; if (RR == 3) { vv += 3; RR = 0; } } }
                int nb = o1 << 3 | o0 << 2 | K1 << 1 | K0; short& t = nxt[idx(r2, a2, nb, RR)]; if (vv > t) t = vv; } }
        swap(cur, nxt); }
    int best = -1, br = 0, ba = 0;
    for (int r = 0; r <= k; r++) for (int a = 0; a <= r; a++) for (int bits = 0; bits < 16; bits++) for (int R = 0; R < 3; R++) { int v = cur[idx(r, a, bits, R)]; if (v == NEG) continue;
        int K2 = bits >> 1 & 1, K1 = bits & 1, RR = R; // finalise the last two cells
        if (Z >= 2) { if (K2) RR = 0; else { RR++; if (RR == 3) { v += 3; RR = 0; } } } if (K1) RR = 0; else { RR++; if (RR == 3) { v += 3; RR = 0; } }
        if (v > best) { best = v; br = r; ba = a; } }
    int fin = max(M + 2 * n2, e2 + 3 * max(0, (C - best) / 3)); int v0 = (Z - (par == 0 ? 2 : 0)) / 3; // rough baseline
    printf("M=%d cls=%d par=%d k=%d  max v=%d (removed %d, re-placed %d)  final extent=%d  minus 5M/3 = %+.1f  7M/4=%.1f  fluid=e2+C-v=%d (%+.1f)\n", M, cls, par, k, best, br, ba, fin, fin - 5.0 * M / 3, 7.0 * M / 4, e2 + C - best, e2 + C - best - 5.0 * M / 3); (void)v0;
    return 0; }
