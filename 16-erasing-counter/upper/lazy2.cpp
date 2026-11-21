// Trajectory search over ALL trees of depth <= r (d = 2^r - 1 indices), written from the definition in 3-question.txt.
// The orbit of a start state is followed; a tree node is decided only when the orbit first reaches it.
// Indices are named 0,1,2,.. in order of first use; the start value of an index is chosen when it is named.
// The start state is required to be a minimum-weight state of its cycle (every cycle has one), so every cycle of
// every tree is produced once per choice of such a start state, together with the part of the tree it uses.
// At every closed cycle it checks:
//   E : number of steps using a leaf with R = empty is <= 1 when the length is >= 2;
//   P1: length <= L(T'), T' = the decided part of the tree with every undecided node made a leaf
//       (L(leaf)=1, L(node)=L(1-child)*(L(0-child)+1)); T' has the smallest L of any tree containing this orbit.
// usage: lazy2 r [mutate]   mutate=1: L(node)=L(1)*L(0) (wrong on purpose); mutate=2: a leaf writing index 0 counts as empty
#include <bits/stdc++.h>
using namespace std;
int r, d, k = 0, MUT = 0; unsigned s0 = 0;
int kind[64], val[64];
vector<unsigned char> vis; vector<unsigned> trail;
long long nodes = 0, cycles = 0, violE = 0, violP1 = 0, tight = 0; int best = 0, maxE = 0; int shown = 0;
long long Lof(int n) {
    if (kind[n] != 1) return 1;
    return Lof(2 * n + 1) * (Lof(2 * n) + (MUT == 1 ? 0 : 1));
}
string show(int n) {
    if (kind[n] == 1) return "(" + to_string(val[n] + 1) + " " + show(2 * n) + " " + show(2 * n + 1) + ")";
    if (kind[n] == 2 && val[n] >= 0) return "[" + to_string(val[n] + 1) + "]";
    if (kind[n] == 2) return "[]";
    return "?";
}
void report(const char *w, int len, long long x) {
    if (shown++ < 20) { printf("WITNESS %s: len=%d value=%lld tree %s start ", w, len, x, show(1).c_str()); for (int i = 0; i < k; i++) printf("%d", (s0 >> i) & 1); printf("\n"); }
}
void go(unsigned s, int len, int ne) {
    size_t mark = trail.size();
    for (;;) {
        int n = 1, pl = 0, path[8]; unsigned P = 0;
        while (kind[n] == 1) { int i = val[n]; path[pl++] = i; P |= 1u << i; n = 2 * n + ((s >> i) & 1); }
        if (kind[n] == 0) {
            nodes++;
            kind[n] = 2; val[n] = -1; go(s, len, ne);
            for (int q = 0; q < pl; q++) { val[n] = path[q]; go(s, len, ne); }
            if (pl < r) {
                kind[n] = 1;
                for (int i = 0; i < k; i++) { if (P >> i & 1) continue; val[n] = i; go(s, len, ne); }
                if (k < d) {
                    val[n] = k; k++; go(s, len, ne);
                    unsigned b = 1u << (k - 1); s0 |= b; go(s | b, len, ne); s0 &= ~b; k--;
                }
            }
            kind[n] = 0; break;
        }
        unsigned t = s & ~P; if (val[n] >= 0) t |= 1u << val[n];
        bool e = val[n] < 0; if (MUT == 2 && val[n] == 0) e = true;
        if (e) ne++;
        len++;
        unsigned df = t ^ s0;
        if (df == 0) {
            cycles++;
            if (len > best) best = len;
            if (len >= 2) { if (ne > maxE) maxE = ne; if (ne >= 2) { violE++; report("E", len, ne); } }
            long long L = Lof(1);
            if (len > L) { violP1++; report("P1", len, L); }
            if (len == L) tight++;
            break;
        }
        if (vis[df]) break;
        if (__builtin_popcount(t) < __builtin_popcount(s0)) break;
        vis[df] = 1; trail.push_back(df); s = t;
    }
    while (trail.size() > mark) { vis[trail.back()] = 0; trail.pop_back(); }
}
int main(int argc, char **argv) {
    r = atoi(argv[1]); d = (1 << r) - 1; if (argc > 2) MUT = atoi(argv[2]);
    vis.assign(1u << d, 0); vis[0] = 1;
    go(0, 0, 0);
    printf("r=%d MUT=%d COMPLETE decision nodes=%lld closed cycles=%lld longest=%d\n", r, MUT, nodes, cycles, best);
    printf("E: max empty-leaf steps on a cycle of length>=2: %d ; cycles with >=2: %lld\n", maxE, violE);
    printf("P1: cycles longer than L(T'): %lld ; cycles with length == L(T'): %lld\n", violP1, tight);
}
