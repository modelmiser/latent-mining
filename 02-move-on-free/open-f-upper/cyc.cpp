// open-f-upper/cyc.cpp : certificate for the ASYMPTOTIC restricted bound.
// The DP of dp4.cpp is a walk in a finite graph: node = (i mod 3, o[i-1], o[i], keep[i-1], keep[i], R), one edge per choice
// o[i+1] in {0,1}.  Edge gain g = (#1s the thinning must keep) + 3*(completed free 3-slots); an edge "removes" (base 1 -> 0)
// or "adds" (base 0 -> 1).  With integer weights  w = 12*g - 4 - A*[remove] - B*[add]  we look for A,B >= 0 with NO positive
// cycle.  Then for every walk:  12*v <= 4*Z + A*r + B*a + PHI, PHI = max finite potential, so with a <= r <= k:
//     v <= Z/3 + (A+B)/12 * k + PHI/12 .
// usage: cyc cls     prints, for every A+B = S (S = 20, 21, 22) and B = 0..S, whether a positive cycle exists and PHI.
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char** argv) { int cls = atoi(argv[1]); const int N = 3 * 16 * 3;
    struct E { int from, to, g, rem, add; }; vector<E> es;
    for (int p = 0; p < 3; p++) for (int bits = 0; bits < 16; bits++) for (int R = 0; R < 3; R++) { int o2 = bits >> 3 & 1, o1 = bits >> 2 & 1, k2 = bits >> 1 & 1, k1 = bits & 1;
        if ((k2 && !o2) || (k1 && !o1)) continue; // a kept cell holds a 1
        int b = (p % 3 != cls); // p = (index of the new cell) mod 3
        for (int o0 = 0; o0 < 2; o0++) { int K2 = k2, K1 = k1, K0 = 0, g = 0, RR = R;
            if ((o2 | o1 | o0) && !(K2 | K1)) { g++; if (o0) K0 = 1; else if (o1) K1 = 1; else K2 = 1; }
            if (K2) RR = 0; else { RR++; if (RR == 3) { g += 3; RR = 0; } }
            int nb = o1 << 3 | o0 << 2 | K1 << 1 | K0; es.push_back({(p * 16 + bits) * 3 + R, (((p + 1) % 3) * 16 + nb) * 3 + RR, g, b && !o0, !b && o0}); } }
    for (int S = 20; S <= 22; S++) for (int B = 0; B <= S; B++) { int A = S - B; vector<long> d(N, 0); bool pos = false;
        for (int it = 0; it <= N + 1; it++) { bool ch = false; for (auto& e : es) { long w = 12 * e.g - 4 - A * e.rem - B * e.add; if (d[e.from] + w > d[e.to]) { d[e.to] = d[e.from] + w; ch = true; } } if (!ch) break; if (it == N + 1) pos = true; }
        if (!pos) printf("cls=%d  A+B=%d: A=%d B=%d  NO positive cycle, PHI=%ld  =>  v <= Z/3 + (%d/12)*k + %ld/12 (+ boundary const)\n", cls, S, A, B, *max_element(d.begin(), d.end()), S, *max_element(d.begin(), d.end())); }
    printf("cls=%d: (any A+B value not listed above has a positive cycle for every split, i.e. that slope is NOT certified)\n", cls); return 0; }
