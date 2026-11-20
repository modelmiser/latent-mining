// Referee's own implementation, written from the problem text.
// States: 0=Q 1=G 2=F 3..s-1 unnamed.  tab[x][k], x != F.
// Modes:
//   brute S NMAX          every rule; prefix histogram, single-n stats, lemma checks
//   lazy  S asc           lazy search, NO symmetry breaking, n = 2,3,... ; weighted histogram
//   lazy  S desc N        lazy search, NO symmetry breaking, n = N,N-1,...,2 ; counts rules syncing all 2..N
//   (lazy modes accept trailing "C c" to run only chunk c of C, split on first two branch values)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <vector>
#include <string>
typedef unsigned long long u64;
typedef unsigned __int128 u128;
static int S;
static int tab[16][3];
enum { SYNC = 1, FAILR = 2, NEED = 3 };
static const int MAXN = 40;

// simulate length n with current table. returns SYNC (time in *T), FAILR, or NEED (entry in *nx,*nk)
static int sim(int n, long long *T, int *nx, int *nk) {
    unsigned char c[MAXN + 2], d[MAXN + 2], saved[MAXN + 2];
    c[1] = 1;
    for (int i = 2; i <= n; i++) c[i] = 0;
    memcpy(saved, c, sizeof saved);
    long long t = 0, power = 1, lam = 0;
    for (;;) {
        int nf = 0;
        for (int i = 1; i <= n; i++) {
            int x = c[i], k = 0;
            if (i > 1 && c[i - 1] != x) k++;
            if (i < n && c[i + 1] != x) k++;
            int y = tab[x][k];
            if (y < 0) { *nx = x; *nk = k; return NEED; }
            d[i] = (unsigned char)y;
            if (y == 2) nf++;
        }
        t++;
        if (nf == n) { *T = t; return SYNC; }
        if (nf > 0) return FAILR;
        memcpy(c + 1, d + 1, n);
        // Brent cycle detection (configurations are F-free here, evolution deterministic)
        if (memcmp(c + 1, saved + 1, n) == 0) return FAILR;
        lam++;
        if (lam == power) { memcpy(saved + 1, c + 1, n); power *= 2; lam = 0; }
    }
}

// ---------------- brute ----------------
static void brute(int NMAX) {
    int ne = 3 * (S - 1) - 1; // entries other than d(Q,0)
    std::vector<int> ex, ek;
    for (int x = 0; x < S; x++) { if (x == 2) continue; for (int k = 0; k < 3; k++) { if (x == 0 && k == 0) continue; ex.push_back(x); ek.push_back(k); } }
    u64 total = 1; for (int i = 0; i < ne; i++) total *= S;
    std::vector<u64> hist(NMAX + 2, 0), cnt(NMAX + 2, 0);
    std::vector<long long> mint(NMAX + 2, -1);
    u64 viol_fact0 = 0, viol_l2 = 0, viol_q1 = 0, viol_2n2 = 0, pairs_l2 = 0, pairs_eq = 0;
    std::vector<int> dig(ne, 0);
    tab[0][0] = 0;
    for (u64 r = 0; r < total; r++) {
        u64 q = r;
        for (int i = 0; i < ne; i++) { tab[ex[i]][ek[i]] = (int)(q % S); q /= S; }
        long long tt[64]; bool ok[64];
        int nx, nk; long long T;
        // n = 2 first: most rules fail it; only do full single-n stats when cheap filter passes or always?
        // always: needed for single-n counts.
        int pref = 1; bool pre = true;
        for (int n = 2; n <= NMAX; n++) {
            int res = sim(n, &T, &nx, &nk);
            ok[n] = (res == SYNC); tt[n] = T;
            if (ok[n]) {
                cnt[n]++;
                if (mint[n] < 0 || T < mint[n]) mint[n] = T;
                if (T < n - 1) viol_fact0++;
                if (T < 2 * n - 2) viol_2n2++;
                if (n >= 3 && (tab[0][1] == 0 || tab[0][1] == 2)) viol_q1++;
                if (pre) pref = n;
            } else pre = false;
        }
        hist[pref]++;
        for (int n = 2; n <= NMAX; n++) if (ok[n]) for (int m = n + 1; m <= NMAX; m++) if (ok[m]) {
            pairs_l2++;
            if (tt[n] >= 2 * n - 1) continue;
            if (tt[n] == tt[m] && m - 1 <= tt[n] && tt[n] <= 2 * n - 2) { pairs_eq++; continue; }
            viol_l2++;
        }
    }
    printf("brute S=%d rules=%llu NMAX=%d\n", S, total, NMAX);
    for (int L = 1; L <= NMAX; L++) printf("  longest prefix 2..%d : %llu rules\n", L, hist[L]);
    for (int n = 2; n <= NMAX; n++) printf("  single n=%d : %llu rules, min time %lld\n", n, cnt[n], mint[n]);
    printf("  violations: t<n-1: %llu ; d(Q,1) in {Q,F} yet syncs n>=3: %llu ; Lemma2: %llu (pairs tested %llu, of which early-equal case %llu)\n",
           viol_fact0, viol_q1, viol_l2, pairs_l2, pairs_eq);
    printf("  (rule,n) with t < 2n-2 (poser's belief violated): %llu\n", viol_2n2);
}

// ---------------- lazy ----------------
static int unassigned;
static u128 wh[64];     // weighted histogram by longest prefix (asc) / [0]=fail,[1]=pass (desc)
static u64 leaves = 0;
static int chunkC = 1, chunkc = 0, depthb = 0, firstv = 0;
static int bestL = 0; static int besttab[16][3]; static long long besttimes[64]; static long long curtimes[64];
static bool DESC = false; static int NTOP = 0; static const int ASC_CAP = 36;
static u128 pw(int e) { u128 r = 1; while (e-- > 0) r *= S; return r; }

static void leaf_asc(int L) {
    wh[L] += pw(unassigned); leaves++;
    if (L > bestL) { bestL = L; memcpy(besttab, tab, sizeof tab); memcpy(besttimes, curtimes, sizeof curtimes); }
}
static void go(int n) {
    for (;;) {
        if (!DESC && n > ASC_CAP) { printf("CAP REACHED n=%d\n", n); exit(3); }
        if (DESC && n < 2) { wh[1] += pw(unassigned); leaves++; if (!bestL) { bestL = 1; memcpy(besttab, tab, sizeof tab); memcpy(besttimes, curtimes, sizeof curtimes);} return; }
        int nx, nk; long long T;
        int res = sim(n, &T, &nx, &nk);
        if (res == SYNC) { curtimes[n] = T; n += DESC ? -1 : 1; continue; }
        if (res == FAILR) { if (DESC) { wh[0] += pw(unassigned); leaves++; } else leaf_asc(n - 1); return; }
        // NEED: branch on every value, no symmetry breaking
        unassigned--; depthb++;
        for (int v = 0; v < S; v++) {
            if (chunkC > 1) {
                if (depthb == 1) firstv = v;
                if (depthb == 2 && ((firstv * S + v) % chunkC) != chunkc) continue;
            }
            tab[nx][nk] = v;
            go(n);
        }
        tab[nx][nk] = -1;
        unassigned++; depthb--;
        return;
    }
}
static void pr128(u128 v) { char b[64]; int i = 63; b[i] = 0; if (v == 0) b[--i] = '0'; while (v) { b[--i] = '0' + (int)(v % 10); v /= 10; } printf("%s", b + i); }

int main(int argc, char **argv) {
    if (argc < 3) return 1;
    std::string mode = argv[1]; S = atoi(argv[2]);
    if (mode == "brute") { brute(atoi(argv[3])); return 0; }
    int a = 3;
    if (std::string(argv[a]) == "desc") { DESC = true; NTOP = atoi(argv[a + 1]); a += 2; } else a += 1;
    if (a + 1 < argc) { chunkC = atoi(argv[a]); chunkc = atoi(argv[a + 1]); }
    for (int x = 0; x < 16; x++) for (int k = 0; k < 3; k++) tab[x][k] = -1;
    tab[0][0] = 0; unassigned = 3 * (S - 1) - 1;
    go(DESC ? NTOP : 2);
    printf("lazy S=%d %s chunk %d/%d leaves=%llu\n", S, DESC ? "desc" : "asc", chunkc, chunkC, leaves);
    u128 tot = 0;
    if (DESC) {
        printf("  rules syncing all 2..%d : ", NTOP); pr128(wh[1]); printf("\n  rules not: "); pr128(wh[0]); printf("\n"); tot = wh[0] + wh[1];
    } else {
        for (int L = 1; L < 64; L++) if (wh[L]) { printf("  longest prefix 2..%d : ", L); pr128(wh[L]); printf(" rules\n"); tot += wh[L]; }
    }
    printf("  weight total: "); pr128(tot); printf("  (S^(3S-4) = "); pr128(pw(3 * S - 4)); printf(")\n");
    if (bestL) {
        printf("  a best rule (unassigned shown as -1), order d(x,0..2) for x=Q,G,U1..:");
        for (int x = 0; x < S; x++) { if (x == 2) continue; for (int k = 0; k < 3; k++) printf(" %d", besttab[x][k]); }
        printf("\n  times:");
        if (DESC) for (int n = 2; n <= NTOP; n++) printf(" %lld", besttimes[n]); else for (int n = 2; n <= bestL; n++) printf(" %lld", besttimes[n]);
        printf("\n");
    }
    return 0;
}
