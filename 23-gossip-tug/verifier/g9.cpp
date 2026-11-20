// Referee's own program for the Delayer/Hastener gossip game. Written from the problem text.
// modes:
//   exact n [raw] [V m r]        full minimax, memo on canonical (or raw) positions; checks static bounds on every position
//   thr n tlo thi logTT [V m r]  threshold search "Delayer forces >= t" for t = tlo..thi ; env NOLEM=1 disables lemma cut-offs
//   certD n claim file fmt       fmt A: "K_0..K_{n-1} a"   fmt B: "t a K_0..K_{n-1} ..."
//   certH n claim file           "K_0..K_{n-1} a:b a:b ..."
//   table n file                 "value K_1..K_n" lines, compared against my exact values
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 u128;
typedef uint64_t u64;
static int n;
static unsigned FULLM;
struct S { uint16_t k[11]; };
static inline int pc(unsigned x) { return __builtin_popcount(x); }
static inline bool legal(const S &s, int a, int b) { return a != b && (!((s.k[a] >> b) & 1) || !((s.k[b] >> a) & 1)); }
static inline bool terminal(const S &s) { for (int i = 0; i < n; i++) if (s.k[i] != FULLM) return false; return true; }
static inline void docall(S &s, int a, int b) { uint16_t u = s.k[a] | s.k[b]; s.k[a] = u; s.k[b] = u; }
static inline u64 mix(u64 x) { x ^= x >> 33; x *= 0xff51afd7ed558ccdULL; x ^= x >> 33; x *= 0xc4ceb9fe1a85ec53ULL; x ^= x >> 33; return x; }
static bool RAW = false;
static long CAP = 1L << 60;
static const long FACT[12] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800};

// relabel: perm[old] = new. out = relabelled position. Exact canonical form if the class-permutation count <= CAP.
static void canon(const S &s, S &out, int *perm) {
    if (RAW) { out = s; for (int i = 0; i < n; i++) perm[i] = i; return; }
    uint16_t col[11];
    for (int j = 0; j < n; j++) { unsigned c = 0; for (int i = 0; i < n; i++) if ((s.k[i] >> j) & 1) c |= 1u << i; col[j] = c; }
    u64 h[11], h2[11];
    for (int i = 0; i < n; i++) h[i] = pc(s.k[i]) * 16 + pc(col[i]);
    for (int r = 0; r < 3; r++) {
        u64 m1[11], m2[11];
        for (int i = 0; i < n; i++) { m1[i] = mix(h[i] * 3 + 1); m2[i] = m1[i] * 0x9e3779b97f4a7c15ULL + 0x1234567; m2[i] ^= m2[i] >> 29; }
        for (int i = 0; i < n; i++) {
            u64 a = 0, b = 0;
            for (unsigned m = s.k[i] & ~(1u << i); m; m &= m - 1) a += m1[__builtin_ctz(m)];
            for (unsigned m = col[i] & ~(1u << i); m; m &= m - 1) b += m2[__builtin_ctz(m)];
            h2[i] = mix(h[i] * 1000003ULL + a * 31 + (b ^ (b >> 17)) * 0xc2b2ae3d27d4eb4fULL);
        }
        memcpy(h, h2, sizeof(u64) * n);
    }
    int ord[11];
    for (int i = 0; i < n; i++) ord[i] = i;
    sort(ord, ord + n, [&](int x, int y) { return h[x] != h[y] ? h[x] < h[y] : x < y; });
    // segs[]/sege[]: colour classes that still need their orderings enumerated. A class whose members are pairwise
    // interchangeable (each transposition is an automorphism of the position) gives the same relabelled position in
    // every internal order, so it is left as sorted.
    int segs[12], sege[12], ns = 0; long prod = 1;
    for (int i = 0; i < n;) {
        int j = i; while (j < n && h[ord[j]] == h[ord[i]]) j++;
        if (j - i > 1) {
            bool tw = true; int x = ord[i];
            for (int q = i + 1; q < j && tw; q++) {
                int y = ord[q]; unsigned mm = (1u << x) | (1u << y);
                if ((s.k[x] & ~mm) != (s.k[y] & ~mm) || ((s.k[x] >> y) & 1) != ((s.k[y] >> x) & 1)) { tw = false; break; }
                for (int z = 0; z < n; z++) if (z != x && z != y && ((s.k[z] >> x) & 1) != ((s.k[z] >> y) & 1)) { tw = false; break; }
            }
            if (!tw) { segs[ns] = i; sege[ns] = j; ns++; if (prod < (1L << 40)) prod *= FACT[j - i]; }
        }
        i = j;
    }
    auto enc = [&](const int *o, S &e, int *p) {
        for (int i = 0; i < n; i++) p[o[i]] = i;
        for (int i = 0; i < n; i++) { unsigned v = 0; for (unsigned m = s.k[o[i]]; m; m &= m - 1) v |= 1u << p[__builtin_ctz(m)]; e.k[i] = v; }
    };
    enc(ord, out, perm);
    if (prod == 1 || prod > CAP) return;
    S e; int p[11];
    while (true) {
        int sg = ns - 1;
        for (; sg >= 0; sg--) if (next_permutation(ord + segs[sg], ord + sege[sg])) break;
        if (sg < 0) break;
        enc(ord, e, p);
        bool less = false;
        for (int i = 0; i < n; i++) if (e.k[i] != out.k[i]) { less = e.k[i] < out.k[i]; break; }
        if (less) { out = e; memcpy(perm, p, sizeof(int) * n); }
    }
}
static inline u128 keyOf(const S &s) {
    u128 k = 0; int sh = 0;
    for (int i = 0; i < n; i++) {
        unsigned r = s.k[i]; unsigned lowm = (1u << i) - 1;
        unsigned v = (r & lowm) | ((r >> (i + 1)) << i);
        k |= (u128)v << sh; sh += n - 1;
    }
    return k;
}
// static bounds
static bool NOLEM = false;
static inline int nonfull(const S &s) { int u = 0; for (int i = 0; i < n; i++) u += s.k[i] != FULLM; return u; }
static inline int livepairs(const S &s) { int c = 0; for (int a = 0; a < n; a++) for (int b = a + 1; b < n; b++) c += legal(s, a, b); return c; }
static void sbounds(const S &s, int &lo, int &hi) {
    int u = nonfull(s);
    lo = (u + 1) / 2;
    hi = livepairs(s);
    if (NOLEM) return;
    int mink = n; unsigned fullset = 0;
    for (int j = 0; j < n; j++) { int c = 0; for (int i = 0; i < n; i++) c += (s.k[i] >> j) & 1; mink = min(mink, c); }
    lo = max(lo, n - mink);
    for (int i = 0; i < n; i++) if (s.k[i] == FULLM) fullset |= 1u << i;
    if (fullset) { bool ok = true; for (int i = 0; i < n; i++) if (s.k[i] != FULLM && !(fullset & ~s.k[i])) { ok = false; break; } if (ok) hi = min(hi, u); }
}
struct H128 { size_t operator()(const u128 &x) const { return mix((u64)x) ^ mix((u64)(x >> 64) + 0x9e3779b97f4a7c15ULL); } };
static unordered_map<u128, uint8_t, H128> memo;
static long viol = 0;
static int exactVal(const S &s) { // s canonical
    if (terminal(s)) return 0;
    u128 key = keyOf(s);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;
    int best = 0;
    for (int a = 0; a < n; a++) {
        if (getenv("NONFULL") && s.k[a] == FULLM) continue; // neighbouring reading: Delayer may not name a fully informed agent
        int worst = 1000;
        for (int b = 0; b < n; b++) if (legal(s, a, b)) {
            S c = s; docall(c, a, b); S cc; int p[11]; canon(c, cc, p);
            worst = min(worst, 1 + exactVal(cc));
        }
        if (worst < 1000) best = max(best, worst);
    }
    bool sv = NOLEM; NOLEM = false; int lo, hi; sbounds(s, lo, hi); NOLEM = sv;
    if (best < lo || best > hi) { viol++; if (viol < 5) { printf("BOUND VIOLATION val=%d lo=%d hi=%d :", best, lo, hi); for (int i = 0; i < n; i++) printf(" %x", s.k[i]); puts(""); } }
    memo[key] = best;
    return best;
}
static S startPos(int argc, char **argv, int from) {
    S s; memset(&s, 0, sizeof s);
    for (int i = 0; i < n; i++) s.k[i] = 1u << i;
    for (int i = from; from > 0 && i < argc; i++) if (!strcmp(argv[i], "V")) {
        int m = atoi(argv[i + 1]), r = atoi(argv[i + 2]);
        if (m + r != n) { fprintf(stderr, "m+r != n\n"); exit(2); }
        for (int j = 0; j < m; j++) s.k[j] = (1u << m) - 1;
    }
    return s;
}
// ---------------- threshold search ----------------
struct E { u64 a, b; };
static E *tt; static u64 ttmask; static u64 ttcount = 0, nodes = 0;
static const u128 OCC = (u128)1 << 110;
static inline E *ttfind(u128 key, bool create) {
    u64 ka = (u64)key, kb = (u64)(key >> 64) | (1ULL << 46); // occupied flag at bit 110
    u64 h = (mix(ka) ^ mix(kb * 0x9e3779b97f4a7c15ULL)) & ttmask;
    for (int pr = 0; pr < 24; pr++) {
        E *e = &tt[(h + pr) & ttmask];
        if (e->a == ka && (e->b & 0x0000ffffffffffffULL) == kb) return e;
        if (e->a == 0 && e->b == 0) { if (!create) return nullptr; e->a = ka; e->b = kb | (0xffULL << 56); ttcount++; return e; }
    }
    if (!create) return nullptr;
    E *e = &tt[h]; e->a = ka; e->b = kb | (0xffULL << 56); return e; // replace (lo=0, hi=255)
}
static inline int elo(const E *e) { return (e->b >> 48) & 0xff; }
static inline int ehi(const E *e) { return (e->b >> 56) & 0xff; }
static inline void setlo(E *e, int v) { e->b = (e->b & ~(0xffULL << 48)) | ((u64)v << 48); }
static inline void sethi(E *e, int v) { e->b = (e->b & ~(0xffULL << 56)) | ((u64)v << 56); }
static inline bool twin(const S &s, int a, int b) {
    // swapping labels a,b leaves the position unchanged
    unsigned ma = 1u << a, mb = 1u << b;
    for (int i = 0; i < n; i++) if (i != a && i != b) { if (((s.k[i] >> a) & 1) != ((s.k[i] >> b) & 1)) return false; }
    unsigned ra = s.k[a] & ~(ma | mb), rb = s.k[b] & ~(ma | mb);
    if (ra != rb) return false;
    return ((s.k[a] >> b) & 1) == ((s.k[b] >> a) & 1);
}
static int BORD = 2; static bool PROG = true; static u64 calls = 0;
static inline bool twinq(const S &s, int a, int b) {
    unsigned mm = (1u << a) | (1u << b);
    if ((s.k[a] & ~mm) != (s.k[b] & ~mm)) return false;
    if (((s.k[a] >> b) & 1) != ((s.k[b] >> a) & 1)) return false;
    for (int i = 0; i < n; i++) if (i != a && i != b) { if (((s.k[i] >> a) & 1) != ((s.k[i] >> b) & 1)) return false; }
    return true;
}
static bool dwin(const S &s, int t) { // s: any labelling. true iff Delayer can force >= t further calls
    if (t <= 0) return true;
    if (PROG && (++calls & ((1ULL << 28) - 1)) == 0) { fprintf(stderr, "progress: calls %llu nodes %llu tt %llu %.0fs\n", (unsigned long long)calls, (unsigned long long)nodes, (unsigned long long)ttcount, (double)clock() / CLOCKS_PER_SEC); }
    // static cut-offs, same bounds as sbounds() but evaluated only when they can decide
    int u = 0; unsigned fullset = 0;
    for (int i = 0; i < n; i++) if (s.k[i] != FULLM) u++; else fullset |= 1u << i;
    if ((u + 1) / 2 >= t) return true;
    int live = 0;
    for (int a = 0; a < n; a++) for (int b = a + 1; b < n; b++) live += !(((s.k[a] >> b) & 1) && ((s.k[b] >> a) & 1));
    if (live < t) return false;
    if (!NOLEM) {
        if (t <= n - 1) { for (int j = 0; j < n; j++) { int c = 0; for (int i = 0; i < n; i++) c += (s.k[i] >> j) & 1; if (n - c >= t) return true; } }
        if (fullset && u < t) { bool ok = true; for (int i = 0; i < n; i++) if (s.k[i] != FULLM && !(fullset & ~s.k[i])) { ok = false; break; } if (ok) return false; }
    }
    S cs; int cp[11]; canon(s, cs, cp);
    u128 key = keyOf(cs);
    E *e = ttfind(key, false);
    if (e) { if (elo(e) >= t) return true; if (ehi(e) < t) return false; }
    nodes++;
    // twin classes (transpositions that are automorphisms generate an equivalence relation)
    int cls[11];
    for (int a = 0; a < n; a++) { cls[a] = a; for (int a2 = 0; a2 < a; a2++) if (cls[a2] == a2 && twinq(s, a2, a)) { cls[a] = a2; break; } }
    int as[11], na = 0, cnt[11];
    for (int a = 0; a < n; a++) {
        if (cls[a] != a) continue;
        int c = 0; for (int b = 0; b < n; b++) c += legal(s, a, b);
        if (!c) continue;
        cnt[a] = c; as[na++] = a;
    }
    sort(as, as + na, [&](int x, int y) { return cnt[x] != cnt[y] ? cnt[x] < cnt[y] : x < y; });
    bool res = false;
    for (int ai = 0; ai < na && !res; ai++) {
        int a = as[ai];
        int sc[11], bs[11], nb = 0; unsigned seen = 0;
        for (int b = 0; b < n; b++) if (legal(s, a, b)) {
            if ((seen >> cls[b]) & 1) continue; // an earlier legal reply b' != a is a twin of b
            seen |= 1u << cls[b];
            int un = pc(s.k[a] | s.k[b]), ka = pc(s.k[a]), kb = pc(s.k[b]);
            if (BORD == 2) sc[nb] = -(((un - ka) + (un - kb)) * 64 + un);
            else if (BORD == 3) sc[nb] = -((un == n) * 4096 + (un - ka) + (un - kb));
            else sc[nb] = -(un * 16 + kb);
            bs[nb] = b; nb++;
        }
        int o[11]; for (int i = 0; i < nb; i++) o[i] = i;
        sort(o, o + nb, [&](int x, int y) { return sc[x] != sc[y] ? sc[x] < sc[y] : x < y; });
        bool all = true;
        for (int i = 0; i < nb; i++) { S c = s; docall(c, a, bs[o[i]]); if (!dwin(c, t - 1)) { all = false; break; } }
        if (all) res = true;
    }
    e = ttfind(key, true);
    if (res) { if (elo(e) < t) setlo(e, t); } else { if (ehi(e) > t - 1) sethi(e, t - 1); }
    return res;
}
// ---------------- witness checkers ----------------
struct Line { S f; int a; int perm[11]; vector<pair<int, int>> rep; int t; };
static unordered_map<u128, Line, H128> lines;
static long missing = 0, dupl = 0; static bool bad = false; static string why;
static unordered_map<u128, int, H128> gm;
static void fail(const string &w) { if (!bad) { bad = true; why = w; } }
static int gD(const S &c) { // guaranteed number of further calls for Delayer following the file from canonical c
    if (terminal(c)) return 0;
    u128 key = keyOf(c);
    auto it = gm.find(key); if (it != gm.end()) return it->second;
    auto li = lines.find(key);
    int r;
    if (li == lines.end()) { missing++; r = (nonfull(c) + 1) / 2; }
    else {
        const Line &L = li->second;
        if (L.a < 0 || L.a >= n) { fail("agent out of range"); return 0; }
        int a = L.perm[L.a]; int worst = 1000;
        for (int b = 0; b < n; b++) if (legal(c, a, b)) { S d = c; docall(d, a, b); S dc; int p[11]; canon(d, dc, p); worst = min(worst, 1 + gD(dc)); }
        if (worst == 1000) { fail("Delayer names an inactive agent"); return 0; }
        r = worst;
    }
    gm[key] = r; return r;
}
static int gH(const S &c) {
    if (terminal(c)) return 0;
    u128 key = keyOf(c);
    auto it = gm.find(key); if (it != gm.end()) return it->second;
    auto li = lines.find(key);
    if (li == lines.end()) { fail("position missing from Hastener file"); return 1000; }
    const Line &L = li->second; int rep[11]; for (int i = 0; i < n; i++) rep[i] = -1;
    for (auto &pr : L.rep) { if (pr.first < 0 || pr.first >= n || pr.second < 0 || pr.second >= n) { fail("agent out of range"); return 1000; } rep[L.perm[pr.first]] = L.perm[pr.second]; }
    int best = 0;
    for (int a = 0; a < n; a++) {
        bool act = false; for (int b = 0; b < n; b++) act |= legal(c, a, b);
        if (!act) continue;
        if (rep[a] < 0) { fail("no reply for an active agent"); return 1000; }
        if (!legal(c, a, rep[a])) { fail("illegal reply"); return 1000; }
        S d = c; docall(d, a, rep[a]); S dc; int p[11]; canon(d, dc, p);
        best = max(best, 1 + gH(dc));
    }
    gm[key] = best; return best;
}
int main(int argc, char **argv) {
    if (argc < 3) return 2;
    string mode = argv[1]; n = atoi(argv[2]); FULLM = (1u << n) - 1;
    NOLEM = getenv("NOLEM") != nullptr; if (getenv("BORD")) BORD = atoi(getenv("BORD"));
    if (mode == "exact") {
        for (int i = 3; i < argc; i++) if (!strcmp(argv[i], "raw")) RAW = true;
        S s = startPos(argc, argv, 3), c; int p[11]; canon(s, c, p);
        int v = exactVal(c);
        printf("exact n=%d %s value=%d positions=%zu boundviolations=%ld\n", n, RAW ? "raw" : "canon", v, memo.size(), viol);
    } else if (mode == "thr") {
        int tlo = atoi(argv[3]), thi = atoi(argv[4]), lg = atoi(argv[5]);
        CAP = 720;
        ttmask = (1ULL << lg) - 1; tt = (E *)calloc(1ULL << lg, sizeof(E));
        if (!tt) { puts("alloc failed"); return 3; }
        S s = startPos(argc, argv, 6), c; int p[11]; canon(s, c, p);
        clock_t t0 = clock();
        for (int t = tlo; t <= thi; t++) {
            bool r = dwin(c, t);
            printf("n=%d %s>=%d ? %s  (tt %llu nodes %llu, %.1fs)\n", n, NOLEM ? "[nolem] " : "", t, r ? "YES" : "NO", (unsigned long long)ttcount, (unsigned long long)nodes, (double)(clock() - t0) / CLOCKS_PER_SEC);
            fflush(stdout);
            if (!r) break;
        }
    } else if (mode == "job") {
        // job n t logTT a1 b1 ... ak bk [a] : play the listed calls (each must be legal) from the initial position, labels as given
        // (no relabelling). Without the trailing a: list Delayer's options at that position. With it: search for a reply b to a
        // such that Delayer cannot force >= t-1 from the child (so naming a does not give >= t).
        int t = atoi(argv[3]), lg = atoi(argv[4]); CAP = 720;
        S s = startPos(0, argv, 0);
        int i = 5;
        for (; i + 1 < argc; i += 2) { int a = atoi(argv[i]), b = atoi(argv[i + 1]); if (!legal(s, a, b)) { puts("ILLEGAL call in path"); return 2; } docall(s, a, b); }
        printf("position:"); for (int j = 0; j < n; j++) printf(" %x", s.k[j]); printf("  remaining threshold t=%d\n", t);
        if (i >= argc) {
            for (int a = 0; a < n; a++) {
                int c = 0; for (int b = 0; b < n; b++) c += legal(s, a, b);
                if (!c) { printf("agent %d inactive\n", a); continue; }
                int tw = -1; for (int a2 = 0; a2 < a; a2++) if (twin(s, a2, a)) { tw = a2; break; }
                printf("agent %d active, replies:", a); for (int b = 0; b < n; b++) if (legal(s, a, b)) printf(" %d", b);
                if (tw >= 0) printf("   [twin of %d]", tw); puts("");
            }
            return 0;
        }
        int a = atoi(argv[i]);
        ttmask = (1ULL << lg) - 1; tt = (E *)calloc(1ULL << lg, sizeof(E)); if (!tt) { puts("alloc failed"); return 3; }
        clock_t t0 = clock(); bool found = false;
        // order replies by static upper bound
        vector<pair<int,int>> bs;
        for (int b = 0; b < n; b++) if (legal(s, a, b)) { S c = s; docall(c, a, b); int l2, h2; sbounds(c, l2, h2); bs.push_back({h2, b}); }
        if (bs.empty()) { puts("agent inactive"); return 2; }
        sort(bs.begin(), bs.end());
        const char *only = getenv("REPLY");
        for (auto &pb : bs) {
            int b = pb.second; if (only && atoi(only) != b) continue;
            S c = s; docall(c, a, b); S cc; int p[11]; canon(c, cc, p);
            bool r = dwin(cc, t - 1);
            printf("agent %d reply %d: Delayer forces >=%d from child ? %s (tt %llu nodes %llu, %.1fs)\n", a, b, t - 1, r ? "YES" : "NO", (unsigned long long)ttcount, (unsigned long long)nodes, (double)(clock() - t0) / CLOCKS_PER_SEC);
            fflush(stdout);
            if (!r) { found = true; break; }
        }
        printf("RESULT agent %d at this position: %s\n", a, found ? "REFUTED (does not give >= t)" : "NOT refuted by the replies tried");
    } else if (mode == "certD" || mode == "certH") {
        int claim = atoi(argv[3]); ifstream f(argv[4]); char fmt = mode == "certD" ? argv[5][0] : 'H';
        string ln; long nl = 0;
        while (getline(f, ln)) {
            if (ln.empty() || ln[0] == '#') continue;
            istringstream is(ln); Line L; memset(&L.f, 0, sizeof L.f); L.a = -1; L.t = 0; string tok; bool okl = true;
            if (fmt == 'B') { if (!(is >> L.t >> L.a)) okl = false; }
            for (int i = 0; i < n && okl; i++) { if (!(is >> tok)) { okl = false; break; } L.f.k[i] = strtoul(tok.c_str(), nullptr, 16); if (L.f.k[i] > FULLM || !((L.f.k[i] >> i) & 1)) okl = false; }
            if (fmt == 'A') { if (!(is >> L.a)) okl = false; }
            if (fmt == 'H') { while (is >> tok) { int a, b; if (sscanf(tok.c_str(), "%d:%d", &a, &b) != 2) { okl = false; break; } L.rep.push_back({a, b}); } }
            if (!okl) { fail("malformed line"); break; }
            S c; canon(L.f, c, L.perm);
            u128 key = keyOf(c);
            if (lines.count(key)) { dupl++; if (L.t > lines[key].t) lines[key] = L; } else lines[key] = L;
            nl++;
        }
        S s = startPos(argc, argv, 99), c; int p[11]; canon(s, c, p);
        int g = mode == "certD" ? gD(c) : gH(c);
        bool acc = !bad && (mode == "certD" ? g >= claim : g <= claim);
        printf("%s %s n=%d file=%s lines=%ld dupl=%ld visited=%zu missing(fallback)=%ld guaranteed%s%d claim=%d %s\n", acc ? "ACCEPT" : "REJECT", mode.c_str(), n, argv[4], nl, dupl, gm.size(), missing, mode == "certD" ? ">=" : "<=", g, claim, bad ? why.c_str() : "");
        return acc ? 0 : 1;
    } else if (mode == "table") {
        S s = startPos(argc, argv, 99), c; int p[11]; canon(s, c, p);
        int v = exactVal(c);
        ifstream f(argv[3]); string ln; long nl = 0, mism = 0, unreach = 0; unordered_set<u128, H128> seen;
        while (getline(f, ln)) {
            if (ln.empty() || ln[0] == '#') continue;
            istringstream is(ln); int val; is >> val; S t; memset(&t, 0, sizeof t); string tok;
            for (int i = 0; i < n; i++) { is >> tok; t.k[i] = strtoul(tok.c_str(), nullptr, 16); }
            S tc; canon(t, tc, p); u128 key = keyOf(tc); nl++;
            if (!seen.insert(key).second) { mism++; continue; }
            auto it = memo.find(key);
            if (it == memo.end()) { unreach++; if (exactVal(tc) != val) mism++; }
            else if (it->second != val) mism++;
        }
        printf("table n=%d my A(n)=%d my reachable nonterminal classes=%zu file lines=%ld mismatches(or duplicates)=%ld not-in-my-reachable-set=%ld\n", n, v, memo.size() - unreach, nl, mism, unreach);
    }
    return 0;
}
