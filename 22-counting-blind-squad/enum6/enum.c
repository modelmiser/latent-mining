// Full enumeration of counting-blind firing squad rules. Written from chain/3-question.txt only.
// Build: gcc -O2 -DS=6 -o enum6 enum.c
// States: 0=Q, 1=G, 2=F, 3..S-1 unnamed.
// Rule entries in order (Q,1),(Q,2),(G,0),(G,1),(G,2),(U1,0),...  [(Q,0)=Q fixed]; ND = 3(S-1)-1 digits,
// digit 0 is the most significant digit of the rule index (index in [0, S^ND)).
// Usage: enum CHUNKDIGITS FIRSTCHUNK LASTCHUNK(exclusive) THRESH
//   a chunk fixes the top CHUNKDIGITS digits; one output line set per completed chunk.
// Modes: enum sim d0 d1 ... d(ND-1)   -> print firing time for n=2..NMAX for one rule
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifndef S
#error define S
#endif
#define ND (3*(S-1)-1)
#define NMAX 20           /* 3 bits per cell packed in 64 bits */
#define F 2

static uint8_t d[S*3];     /* d[x*3+k]; row F unused */

/* Returns firing time t>=1 if the rule synchronises length n, 0 if it fails
   (partial fire, or a configuration repeats => never fires).
   Never-fire detection: Brent's cycle detection on the exact packed configuration.
   The run is deterministic and has at most (S-1)^n F-free configurations, so an F-free
   run must repeat a configuration; Brent's method reports only on exact equality of two
   configurations at different times (=> periodic from there, F-free for ever), and
   is guaranteed to hit once the run is in its cycle and the window power >= cycle length. */
static long simulate(int n)
{
    uint8_t c[NMAX], nc[NMAX];
    c[0] = 1; for (int i = 1; i < n; i++) c[i] = 0;
    uint64_t tort = 0; for (int i = 0; i < n; i++) tort |= (uint64_t)c[i] << (3*i);
    uint64_t power = 1, lam = 0;
    for (long t = 1;; t++) {
        int nf = 0; uint64_t pk = 0;
        for (int i = 0; i < n; i++) {
            int x = c[i], k = 0;
            if (i > 0 && c[i-1] != x) k++;
            if (i < n-1 && c[i+1] != x) k++;
            int y = d[x*3+k];
            nc[i] = y; nf += (y == F); pk |= (uint64_t)y << (3*i);
        }
        if (nf) return nf == n ? t : 0;
        if (pk == tort) return 0;
        lam++;
        if (lam == power) { tort = pk; power <<= 1; lam = 0; }
        memcpy(c, nc, n);
    }
}

/* N = largest N such that lengths 2..N all synchronise; 1 if length 2 fails. */
static int scoreN(void)
{
    int n;
    for (n = 2; n <= NMAX; n++) if (!simulate(n)) break;
    return n - 1;
}

static void setrule(const int *dig)
{
    d[0] = 0;                        /* d(Q,0)=Q */
    for (int j = 0; j < ND; j++) {
        int e = j + 1;               /* entry number in Q0,Q1,Q2,G0,... over non-F states */
        int row = e / 3, k = e % 3;  /* row 0=Q,1=G,2=U1,... */
        int x = row < 2 ? row : row + 1;
        d[x*3+k] = dig[j];
    }
}

int main(int argc, char **argv)
{
    if (argc >= 2 && !strcmp(argv[1], "sim")) {
        int dig[ND];
        if (argc != 2 + ND) { fprintf(stderr, "need %d digits\n", ND); return 2; }
        for (int j = 0; j < ND; j++) dig[j] = atoi(argv[2+j]);
        setrule(dig);
        for (int n = 2; n <= NMAX; n++) { long t = simulate(n); printf("n=%d t=%ld\n", n, t); if (!t) break; }
        printf("N=%d\n", scoreN());
        return 0;
    }
    if (argc != 5) { fprintf(stderr, "usage\n"); return 2; }
    int cd = atoi(argv[1]); long c0 = atol(argv[2]), c1 = atol(argv[3]); int thresh = atoi(argv[4]);
    int dig[ND];
    for (long ch = c0; ch < c1; ch++) {
        long v = ch;
        for (int j = cd - 1; j >= 0; j--) { dig[j] = v % S; v /= S; }
        if (v) { fprintf(stderr, "chunk out of range\n"); return 2; }
        for (int j = cd; j < ND; j++) dig[j] = 0;
        uint64_t hist[NMAX+1]; memset(hist, 0, sizeof hist);
        uint64_t visited = 0;
        static char buf[1<<22]; size_t bl = 0;
        for (;;) {
            setrule(dig);
            int N = scoreN();
            hist[N]++; visited++;
            if (N >= thresh) {
                bl += sprintf(buf+bl, "RULE S=%d N=%d d=", S, N);
                for (int j = 0; j < ND; j++) buf[bl++] = '0' + dig[j];
                buf[bl++] = '\n';
                if (bl > sizeof buf - 200) { fwrite(buf, 1, bl, stdout); bl = 0; }
            }
            int j = ND - 1;                       /* odometer over the low ND-cd digits */
            while (j >= cd && ++dig[j] == S) { dig[j] = 0; j--; }
            if (j < cd) break;
        }
        fwrite(buf, 1, bl, stdout);
        printf("CHUNK S=%d cd=%d id=%ld visited=%llu hist", S, cd, ch, (unsigned long long)visited);
        for (int i = 1; i <= NMAX; i++) printf(" %llu", (unsigned long long)hist[i]);
        printf("\n");
        fflush(stdout);
    }
    return 0;
}
