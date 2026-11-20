// Referee's search, written from the problem statement (no solver code copied).
// Board cells 0..41 (3 bits each) in an unsigned __int128; cell c = vertex c-21.
// usage:
//   v Z h M            zone-restricted Firer (T1=2,T2=4), all lists of length M with a_1=0 whose prefixes can
//                      still be completed to an overlap-connected interval family; reports first Z-winning list
//   v W h M R          unrestricted Firer, all lists of length M with entries in [-R,R] (a_1 free); first winning list
//   v ZW h M R         zone-restricted Firer, entries in [-R,R], a_1=0, NO connectivity pruning, no memo:
//                      enumerates ALL Z-winning lists of length exactly M (no proper prefix winning) and
//                      reports how many have a disconnected final interval family
//   v WW h M R         like ZW but unrestricted Firer: prints all winning lists (a_1 = 0) of length M
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 U;
typedef unsigned long long u64;
static int H, M; static bool ZONE = false, CONN = false, WINDOW = false; static int R = 0;
static const int NC = 42, BITS = 3;
static int cnt[NC]; static int lst[64]; static int depth = 0;
static long long nodes = 0, memohits = 0;
static const int OFF = 21;
static inline int cell(U s, int c) { return (int)((s >> (BITS * c)) & 7); }
static inline U one(int c) { return ((U)1) << (BITS * c); }
static void die(const char* m) { fprintf(stderr, "ABORT: %s\n", m); exit(9); }
static inline int radOf(int c) { return c < 2 ? 0 : (c < 4 ? 1 : 2); }
static bool zn[NC];
static void computeZone() { memset(zn, 0, sizeof zn); for (int x = 2; x < NC - 2; x++) if (cnt[x]) { int r = radOf(cnt[x]); for (int d = -r; d <= r; d++) zn[x + d] = true; } }
// one round: add at cell a, then pass or fire.  cnt[] must already include this add.
static void stepRound(const vector<U>& S, int a, vector<U>& out, bool lastRound) {
    out.clear();
    if (a < 3 || a > NC - 4) die("add out of board");
    if (ZONE) computeZone();
    for (U s : S) {
        if (cell(s, a) + 1 >= H) continue;           // height h after step (1)
        U t = s + one(a);
        out.push_back(t);                            // pass
        if (lastRound) continue;                     // firing in the last round cannot help survival beyond pass
        for (int x = 1; x <= NC - 2; x++) {
            if (cell(t, x) < 2) continue;
            if (x <= 1 || x >= NC - 2) die("chip at board edge");
            if (ZONE && !(zn[x - 1] && zn[x + 1])) continue;
            if (cell(t, x - 1) + 1 >= H || cell(t, x + 1) + 1 >= H) continue;   // height h after step (2)
            out.push_back(t - 2 * one(x) + one(x - 1) + one(x + 1));
        }
    }
    sort(out.begin(), out.end()); out.erase(unique(out.begin(), out.end()), out.end());
}
static int gammaNeed() {   // ceil(Gamma/2): lower bound on further adds needed for overlap-connectivity
    int Rmax = -100, g = 0; bool first = true;
    // intervals sorted by left end: collect
    int L[NC], Rr[NC], n = 0;
    for (int x = 0; x < NC; x++) if (cnt[x]) { int r = radOf(cnt[x]); L[n] = x - r; Rr[n] = x + r; n++; }
    for (int i = 1; i < n; i++) for (int j = i; j > 0 && L[j - 1] > L[j]; j--) { swap(L[j], L[j - 1]); swap(Rr[j], Rr[j - 1]); }
    for (int i = 0; i < n; i++) { if (!first && L[i] > Rmax) g += L[i] - Rmax; first = false; Rmax = max(Rmax, Rr[i]); }
    return (g + 1) / 2;
}
static inline u64 mix(u64 x) { x ^= x >> 33; x *= 0xff51afd7ed558ccdULL; x ^= x >> 33; x *= 0xc4ceb9fe1a85ec53ULL; x ^= x >> 33; return x; }
struct Key { u64 a, b; bool operator==(const Key& o) const { return a == o.a && b == o.b; } };
struct KH { size_t operator()(const Key& k) const { return k.a; } };
static unordered_map<Key, unsigned char, KH> memo;
static U reflectU(U s, int lo, int hi) { U r = 0; for (int c = lo; c <= hi; c++) { int v = cell(s, c); if (v) r += ((U)v) << (BITS * (hi - c)); } return r; }
static Key makeKey(const vector<U>& S) {
    int pl = NC, ph = -1;
    if (WINDOW) { pl = OFF - R; ph = OFF + R; } else for (int x = 0; x < NC; x++) if (cnt[x]) { pl = min(pl, x); ph = max(ph, x); }
    int lo = WINDOW ? 0 : pl - 2, hi = WINDOW ? NC : ph + 2;     // window mode: reflect about cell OFF (cells 1..NC-1)
    Key f{0x1234567ULL, 0x89abcdefULL}, b{0x1234567ULL, 0x89abcdefULL};
    if (WINDOW) {
        for (U s : S) { u64 x0 = (u64)s, x1 = (u64)(s >> 64); f.a += mix(x0 * 0x9e3779b97f4a7c15ULL + mix(x1 + 1)); f.b += mix(mix(x0 + 7) ^ (x1 * 0xc2b2ae3d27d4eb4fULL + 3));
            U r = 0; for (int c = 1; c <= NC - 1; c++) { int v = cell(s, c); if (v) r += ((U)v) << (BITS * (NC - c)); }
            x0 = (u64)r; x1 = (u64)(r >> 64); b.a += mix(x0 * 0x9e3779b97f4a7c15ULL + mix(x1 + 1)); b.b += mix(mix(x0 + 7) ^ (x1 * 0xc2b2ae3d27d4eb4fULL + 3)); }
        if (ZONE) { for (int c = OFF - R; c <= OFF + R; c++) { f.a = mix(f.a + min(cnt[c], 4) + 11); f.b = mix(f.b ^ (min(cnt[c], 4) + 5)); int d = 2 * OFF - c; b.a = mix(b.a + min(cnt[d], 4) + 11); b.b = mix(b.b ^ (min(cnt[d], 4) + 5)); } }
    } else {
        for (U s : S) {
            if (lo > 0 && (s & ((((U)1) << (BITS * lo)) - 1)) != 0) die("chip left of zone");
            U t = s >> (BITS * lo); u64 x0 = (u64)t, x1 = (u64)(t >> 64);
            f.a += mix(x0 * 0x9e3779b97f4a7c15ULL + mix(x1 + 1)); f.b += mix(mix(x0 + 7) ^ (x1 * 0xc2b2ae3d27d4eb4fULL + 3));
            U r = reflectU(s, lo, hi); x0 = (u64)r; x1 = (u64)(r >> 64);
            if ((s >> (BITS * (hi + 1))) != 0) die("chip right of zone");
            b.a += mix(x0 * 0x9e3779b97f4a7c15ULL + mix(x1 + 1)); b.b += mix(mix(x0 + 7) ^ (x1 * 0xc2b2ae3d27d4eb4fULL + 3));
        }
        for (int c = pl, d = ph; c <= ph; c++, d--) { f.a = mix(f.a + min(cnt[c], 4) + 11); f.b = mix(f.b ^ (min(cnt[c], 4) + 5)); b.a = mix(b.a + min(cnt[d], 4) + 11); b.b = mix(b.b ^ (min(cnt[d], 4) + 5)); }
    }
    if (f.a < b.a || (f.a == b.a && f.b <= b.b)) return f; return b;
}
static bool passSurvives(const vector<U>& S, int r) {   // some state survives r more adds by passing forever
    for (U s : S) { int mx = 0; for (int c = 1; c <= NC - 2; c++) mx = max(mx, cell(s, c)); if (mx + r < H) return true; }
    return false;
}
static bool NOMEMO = false, NOPASS = false;
// returns true iff some completion with <= r more adds wins (set becomes empty)
static bool dfs(const vector<U>& S, int r) {
    nodes++;
    if (r == 0) return false;
    if (!NOPASS && passSurvives(S, r)) return false;
    Key k{0, 0};
    if (!NOMEMO) { k = makeKey(S); auto it = memo.find(k); if (it != memo.end() && it->second >= r) { memohits++; return false; } }
    int lo, hi;
    if (WINDOW) { lo = OFF - R; hi = OFF + R; }
    else { int pl = NC, ph = -1; for (int x = 0; x < NC; x++) if (cnt[x]) { pl = min(pl, x); ph = max(ph, x); } lo = pl - 2 * r - 1; hi = ph + 2 * r + 1;
           /* connected final family => (max add position - min add position) <= sum of 2*rad <= M */ if (CONN) { lo = max(lo, ph - M); hi = min(hi, pl + M); } }
    vector<U> T;
    for (int a = lo; a <= hi; a++) {
        if (a < 3 || a > NC - 4) { if (CONN) { int aa = max(0, min(NC - 1, a)); cnt[aa]++; int need = gammaNeed(); cnt[aa]--; if (need > r - 1) continue; /* farther positions only have larger gaps */ } die("candidate add outside board"); }
        cnt[a]++; lst[depth++] = a - OFF;
        bool ok = true;
        if (CONN && gammaNeed() > r - 1) ok = false;
        bool win = false;
        if (ok) {
            stepRound(S, a, T, false);
            if (T.empty()) win = true; else win = dfs(T, r - 1);
        }
        if (win && T.empty()) { printf("  WINNING LIST (h=%d):", H); for (int i = 0; i < depth; i++) printf(" %d", lst[i]); printf("\n"); }
        cnt[a]--; depth--;
        if (win) return true;
    }
    if (!NOMEMO) { auto& v = memo[k]; if (v < r) v = r; }
    return false;
}
// enumeration of all winning lists of length exactly M, a_1 fixed, no pruning except pass rule
static long long nwin = 0, ndisc = 0; static bool PRINTALL = false;
static void enumAll(const vector<U>& S, int r) {
    nodes++;
    if (r == 0) return;
    if (passSurvives(S, r)) return;
    vector<U> T;
    for (int a = OFF - R; a <= OFF + R; a++) {
        cnt[a]++; lst[depth++] = a - OFF;
        stepRound(S, a, T, false);
        if (T.empty()) {
            if (r == 1) { nwin++; bool disc = gammaNeed() > 0; if (disc) ndisc++;
                if (PRINTALL || disc) { printf("  %s", disc ? "DISCONNECTED " : ""); for (int i = 0; i < depth; i++) printf(" %d", lst[i]); printf("\n"); } }
            else { printf("  SHORTER winning list, length %d:", depth); for (int i = 0; i < depth; i++) printf(" %d", lst[i]); printf("\n"); }
        } else enumAll(T, r - 1);
        cnt[a]--; depth--;
    }
}
int main(int argc, char** argv) {
    string mode = argv[1]; H = atoi(argv[2]); M = atoi(argv[3]); if (argc > 4) R = atoi(argv[4]);
    if (getenv("NOMEMO")) NOMEMO = true; if (getenv("NOPASS")) NOPASS = true;
    if (H > 8) die("h too large for 3-bit cells");
    clock_t t0 = clock();
    vector<U> S0, S1; S0.push_back(0);
    if (mode == "Z" || mode == "ZW" || mode == "WW") {
        ZONE = (mode != "WW"); CONN = (mode == "Z"); WINDOW = (mode != "Z");
        cnt[OFF] = 1; lst[depth++] = 0;
        stepRound(S0, OFF, S1, false);
        if (mode == "Z") {
            bool w = S1.empty() ? true : dfs(S1, M - 1);
            printf("Z h=%d M=%d : %s  nodes=%lld memo=%zu memohits=%lld time=%.0fs\n", H, M, w ? "Z-WINNING LIST FOUND" : "no Z-winning list", nodes, memo.size(), memohits, (double)(clock() - t0) / CLOCKS_PER_SEC);
        } else {
            PRINTALL = (getenv("PRINTALL") != 0);
            if (S1.empty()) { if (M == 1) nwin = 1; } else enumAll(S1, M - 1);
            printf("%s h=%d M=%d R=%d : %lld winning lists of length M with a_1=0, %lld with disconnected interval family  nodes=%lld time=%.0fs\n", mode.c_str(), H, M, R, nwin, ndisc, nodes, (double)(clock() - t0) / CLOCKS_PER_SEC);
        }
    } else if (mode == "L" || mode == "LZ") {   // v L h 0 a1 a2 ... : does this list win (LZ: against Z-lines only)
        ZONE = (mode == "LZ"); vector<U> A = S0, B;
        for (int i = 4; i < argc; i++) { int a = OFF + atoi(argv[i]); cnt[a]++; stepRound(A, a, B, false); A.swap(B); if (A.empty()) { printf("%s after %d rounds\n", ZONE ? "Z-WIN" : "WIN", i - 3); return 0; } }
        printf("%s (%zu surviving positions)\n", ZONE ? "Z-LOSE" : "LOSE", A.size());
    } else if (mode == "W") {
        WINDOW = true;
        bool w = dfs(S0, M);
        printf("W h=%d M=%d R=%d : %s  nodes=%lld memo=%zu time=%.0fs\n", H, M, R, w ? "WINNING LIST FOUND" : "no winning list", nodes, memo.size(), (double)(clock() - t0) / CLOCKS_PER_SEC);
    }
    return 0;
}
