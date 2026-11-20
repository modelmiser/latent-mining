// g2: faster exhaustive search for "is there a winning list of length m ?" (see proofs.txt for the prunings)
// usage: g2 H m MODE [R]     MODE=1: connectivity (cluster) rule, positions unbounded (abort if array edge is hit)
//                            MODE=0: all lists with a_1=0 and every a_i in [-R,R]   (no connectivity rule)
// optional env NOHB=1 disables the hitting prune (for cross-checks), SECOND=a restricts a_2 = a (for splitting work)
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
#ifndef WWDEF
#define WWDEF 176
#endif
static const int WW = WWDEF, OFF = WWDEF / 2;
int LLO = 0, LHI = WW - 1; // loop bounds: all chips lie in [LLO,LHI] (checked)
struct Cfg { unsigned char v[WW]; bool operator<(const Cfg& o) const { return memcmp(v, o.v, WW) < 0; } bool operator==(const Cfg& o) const { return !memcmp(v, o.v, WW); } };
int H, M, MODE, RWIN, GMAX, NOHB = 0;
long long nodes = 0, hbprunes = 0, memohits = 0;
int cnt[WW]; int total = 0;
vector<int> path, best;
struct Key { u64 a, b; bool operator==(const Key& o) const { return a == o.a && b == o.b; } };
struct KH { size_t operator()(const Key& k) const { return k.a; } };
// fail memo: open addressing, 16 bytes/entry, key = 122-bit hash of (canonical S, add counts); value = max r known to fail.
// A hash collision could only turn a node into a false "fail" (bias: towards "no winning list"); probability negligible.
struct Ent { u64 a, b; };
Ent* tab = 0; u64 tabmask = 0; long long memosize = 0, memoevict = 0;
static int memoGet(const Key& k) { u64 i = k.a & tabmask, kb = k.b & ~63ULL; for (int p = 0; p < 8; p++) { Ent& e = tab[(i + p) & tabmask]; if (e.a == k.a && (e.b & ~63ULL) == kb) return (int)(e.b & 63); if (!e.a) return 0; } return 0; }
static void memoPut(const Key& k, int r) { u64 i = k.a & tabmask, kb = k.b & ~63ULL; for (int p = 0; p < 8; p++) { Ent& e = tab[(i + p) & tabmask]; if (e.a == k.a && (e.b & ~63ULL) == kb) { if ((int)(e.b & 63) < r) e.b = kb | r; return; } if (!e.a) { e.a = k.a; e.b = kb | r; memosize++; return; } } Ent& e = tab[(i + (k.b >> 8) % 8) & tabmask]; e.a = k.a; e.b = kb | r; memoevict++; }
int A2 = -1000, A3START = -1000; double TLIM = 1e18; bool stopReq = false; clock_t t0;
static inline u64 mix(u64 x) { x ^= x >> 33; x *= 0xff51afd7ed558ccdULL; x ^= x >> 33; x *= 0xc4ceb9fe1a85ec53ULL; x ^= x >> 33; return x; }
static int rho(int F) { if (F <= 0) return 0; int k = 0; while ((k + 2) * (k + 3) / 2 <= F) k++; return k + 1; }
static int gmaxOf(int m) { int g = 0; for (int a = 1; a < m; a++) g = max(g, rho(a - 2) + rho(m - a - 2)); return max(g, 1); }

static void stepSet(const vector<Cfg>& S, int a, vector<Cfg>& out) {
    out.clear();
    for (const Cfg& c0 : S) {
        Cfg c = c0;
        if (++c.v[a] >= H) continue;
        out.push_back(c);
        for (int x = LLO; x <= LHI; x++) if (c.v[x] >= 2) {
            if (c.v[x-1] + 1 >= H || c.v[x+1] + 1 >= H) continue;
            if (x <= LLO || x >= LHI) { fprintf(stderr, "LOOP BOUND HIT - result invalid\n"); exit(3); }
            Cfg e = c; e.v[x] -= 2; e.v[x-1]++; e.v[x+1]++;
            out.push_back(e);
        }
    }
    sort(out.begin(), out.end());
    out.erase(unique(out.begin(), out.end()), out.end());
}
// hitting prune: can <= budget further adds make EVERY c in S (Firer passing forever) reach H ?  only used when budget < H
int extra[WW];
static bool canHit(const vector<Cfg>& S, size_t from, int budget) {
    size_t i = from;
    for (; i < S.size(); i++) {
        bool hit = false;
        for (int x = LLO; x <= LHI && !hit; x++) if (S[i].v[x] + extra[x] >= H) hit = true;
        if (!hit) break;
    }
    if (i == S.size()) return true;
    if (budget == 0) return false;
    for (int x = LLO; x <= LHI; x++) {
        int need = H - S[i].v[x] - extra[x];
        if (need <= budget) {
            extra[x] += need;
            bool ok = canHit(S, i + 1, budget - need);
            extra[x] -= need;
            if (ok) return true;
        }
    }
    return false;
}
int RHO[64];
static int fillersNeeded(int rleft, int xlo, int xhi) { // connectivity rule, necessary condition for extension to a connected list of length M
    int f = 0, prev = -1, mL = 0;                           // all add positions lie in [xlo,xhi]
    for (int x = xlo; x <= xhi; x++) if (cnt[x]) {
        if (prev >= 0) {
            int g = x - prev, mR = total - mL;
            if (g > min(GMAX, RHO[mL + rleft] + RHO[mR + rleft])) f += max(1, (g + GMAX - 1) / GMAX - 1);   // RHO[i] = rho(i-2)
        }
        mL += cnt[x]; prev = x;
    }
    return f;
}
// ---- passive rule (Lemma P in proofs.txt).  After the current prefix (counts cnt[], rleft adds still to come, final length M):
// a position x with cnt[x]+rleft <= H-1 stays "light".  If no firing-count function f >= 0 with
//   sum f <= M-cnt[x]-2,  f(z-1)+f(z+1)-2f(z) >= -(c(z)+e(z)) for all z  (c = cnt without x, e >= 0 extra adds, sum e <= rleft),
//   and f(x-1)+f(x+1) >= 1
// exists, then no completion is a shortest winning list.  Sparse DP over positions, state (f(z-1), f(z), e used) -> min sum f.
int NOPASSIVE = 1; long long passprunes = 0, passcalls = 0;
unordered_map<Key, unsigned char, KH> passcache;
static bool reachable(const int* c, int lo, int hi, int y, int F, int rleft) {
    // exists (f,e) with f(y) >= 1 ?  f lives on a run containing y of length <= F
    int zlo = max(lo - 1, y - F) - 1, zhi = min(hi + 1, y + F) + 1; (void)zlo; (void)zhi;
    zlo = y - F - 1; zhi = y + F + 1;
    static int cur[16][16][8], nxt[16][16][8];
    const int INF = 1000;
    for (int p = 0; p <= F; p++) for (int q = 0; q <= F; q++) for (int e = 0; e <= rleft; e++) cur[p][q][e] = INF;
    cur[0][0][0] = 0; // f(zlo-1)=f(zlo)=0
    for (int z = zlo; z <= zhi; z++) { // choose f(z+1), constraint at z
        for (int p = 0; p <= F; p++) for (int q = 0; q <= F; q++) for (int e = 0; e <= rleft; e++) nxt[p][q][e] = INF;
        int cz = (z >= lo && z <= hi) ? c[z] : 0;
        for (int p = 0; p <= F; p++) for (int q = 0; q <= F; q++) for (int e = 0; e <= rleft; e++) {
            int cost = cur[p][q][e]; if (cost > F) continue;
            for (int ez = 0; e + ez <= rleft; ez++) {
                int nlo = max(0, 2 * q - p - cz - ez);
                if (z + 1 == y) nlo = max(nlo, 1);
                for (int n = nlo; cost + n <= F; n++) { int& t = nxt[q][n][e + ez]; if (cost + n < t) t = cost + n; }
            }
        }
        memcpy(cur, nxt, sizeof(cur));
    }
    for (int e = 0; e <= rleft; e++) if (cur[0][0][e] <= F) return true;
    return false;
}
static bool passiveHopeless(int rleft) {
    if (NOPASSIVE || rleft >= H - 1 || rleft > 6) return false;
    int pl = WW, ph = -1; for (int x = 0; x < WW; x++) if (cnt[x]) { pl = min(pl, x); ph = max(ph, x); }
    Key k{17, 23};
    for (int x = pl; x <= ph; x++) { k.a = mix(k.a ^ (cnt[x] + 5)); k.b = mix(k.b * 77 + cnt[x] + 2); }
    k.a = mix(k.a + rleft);
    auto it = passcache.find(k); if (it != passcache.end()) return it->second;
    passcalls++;
    bool hopeless = false;
    static int c[WW];
    for (int x = pl; x <= ph && !hopeless; x++) if (cnt[x] && cnt[x] + rleft <= H - 1) {
        int F = M - cnt[x] - 2; if (F > 15) { fprintf(stderr, "F too big\n"); exit(3); }
        memcpy(c, cnt, sizeof(c)); c[x] = 0;
        if (F < 1 || !(reachable(c, pl, ph, x - 1, F, rleft) || reachable(c, pl, ph, x + 1, F, rleft))) hopeless = true;
    }
    passcache[k] = hopeless;
    return hopeless;
}
static Key keyOf(const vector<Cfg>& S, int r) {
    int lo = WW, hi = -1;
    if (MODE) {
        for (const Cfg& c : S) { for (int x = LLO; x < lo; x++) if (c.v[x]) { lo = x; break; } for (int x = LHI; x > hi; x--) if (c.v[x]) { hi = x; break; } }
        for (int x = LLO; x <= LHI; x++) if (cnt[x]) { if (x < lo) lo = x; if (x > hi) hi = x; }
    } else { lo = 0; hi = WW - 1; }
    // per-config polynomial hashes read left-to-right (f) and right-to-left (b); set hash = sum of mixed config hashes (order independent)
    const u64 P1 = 0x9E3779B97F4A7C15ULL, P2 = 0xC2B2AE3D27D4EB4FULL;
    u64 A0 = 0, B0 = 0, A1 = 0, B1 = 0;
    for (const Cfg& c : S) {
        u64 f1 = 1, f2 = 2, b1 = 1, b2 = 2;
        for (int i = lo, j = hi; i <= hi; i++, j--) { f1 = f1 * P1 + c.v[i] + 1; f2 = f2 * P2 + c.v[i] + 1; b1 = b1 * P1 + c.v[j] + 1; b2 = b2 * P2 + c.v[j] + 1; }
        A0 += mix(f1); B0 += mix(f2 ^ 0x55); A1 += mix(b1); B1 += mix(b2 ^ 0x55);
    }
    u64 p1 = 17, p2 = 99, q1 = 17, q2 = 99;
    if (MODE) for (int i = lo, j = hi; i <= hi; i++, j--) { p1 = p1 * P1 + cnt[i] + 3; p2 = p2 * P2 + cnt[i] + 3; q1 = q1 * P1 + cnt[j] + 3; q2 = q2 * P2 + cnt[j] + 3; }
    Key k0{mix(A0 ^ mix(p1)), mix(B0 + mix(p2))}, k1{mix(A1 ^ mix(q1)), mix(B1 + mix(q2))};
    if (!MODE) return k0;
    return (k0.a < k1.a || (k0.a == k1.a && k0.b < k1.b)) ? k0 : k1;
}
bool dfs(const vector<Cfg>& S, int r) {
    nodes++;
    if (S.empty()) { best = path; return true; }
    if (r == 0) return false;
    static unsigned char okpos[64][WW]; unsigned char* okp = okpos[r]; bool anyDef = false; // candidate filter from "deficient" configs
    for (const Cfg& c : S) {
        int mx = 0; for (int x = LLO; x <= LHI; x++) mx = max(mx, (int)c.v[x]);
        if (mx + r < H) return false;
        if (mx + r == H) { // Firer passing forever from c: all r remaining adds must go on one vertex that already has mx chips
            if (!anyDef) { anyDef = true; for (int x = LLO; x <= LHI; x++) okp[x] = (c.v[x] == mx); }
            else for (int x = LLO; x <= LHI; x++) if (c.v[x] != mx) okp[x] = 0;
        }
    }
    if (anyDef) { bool any = false; for (int x = LLO; x <= LHI; x++) if (okp[x]) any = true; if (!any) return false; }
    if (!NOHB && r < H) { if (!canHit(S, 0, r)) { hbprunes++; return false; } }
    Key key = keyOf(S, r);
    if (!key.a) key.a = 1;
    if (memoGet(key) >= r) { memohits++; return false; }
    int depth = M - r;
    int lo, hi, pl = WW, ph = -1, freshHit = -1;
    if (MODE) {
        for (int x = LLO; x <= LHI; x++) if (cnt[x]) { pl = min(pl, x); ph = max(ph, x); }
        lo = pl - GMAX * r; hi = ph + GMAX * r;
        // every list obeying the connectivity rule has all positions within GMAX*(M-1) of a_1
        lo = max(lo, OFF - GMAX * (M - 1)); hi = min(hi, OFF + GMAX * (M - 1));
        if (lo < 10 || hi > WW - 11) { fprintf(stderr, "POSITION WINDOW EDGE HIT - result invalid\n"); exit(3); }
    } else { lo = OFF - RWIN; hi = OFF + RWIN; }
    int saveLLO = LLO, saveLHI = LHI;
    vector<Cfg> T;
    if (depth == 1 && lo < OFF) lo = OFF;                 // reflection symmetry: a_2 >= 0
    if (depth == 1 && A2 > -1000) { lo = hi = OFF + A2; }
    if (depth == 2 && A3START > -1000) lo = max(lo, OFF + A3START);
    for (int a = lo; a <= hi; a++) {
        if (depth == 2 && A2 > -1000) {
            if ((double)(clock() - t0) / CLOCKS_PER_SEC > TLIM) { printf("NEXT a3=%d\n", a - OFF); fflush(stdout); stopReq = true; return false; }
        }
        if (anyDef && !NOHB && (a < saveLLO || a > saveLHI || !okp[a])) continue;
        cnt[a]++; total++;
        bool ok = true;
        if (MODE) {
            if (fillersNeeded(r - 1, min(a, pl), max(a, ph)) > r - 1) ok = false;
            else if (passiveHopeless(r - 1)) { ok = false; passprunes++; }
        }
        if (ok && !NOHB && r < H) {   // hitting pre-check for the lines "Firer passes forever from c in S"
            if (a < saveLLO || a > saveLHI) { if (freshHit < 0) freshHit = canHit(S, 0, r - 1) ? 1 : 0; ok = freshHit; }
            else { extra[a] = 1; ok = canHit(S, 0, r - 1); extra[a] = 0; }
        }
        if (ok) {
            LLO = min(saveLLO, a - 6); LHI = max(saveLHI, a + 6);
            stepSet(S, a, T);
            path.push_back(a - OFF);
            bool w = dfs(T, r - 1);
            path.pop_back();
            LLO = saveLLO; LHI = saveLHI;
            if (w) { cnt[a]--; total--; return true; }
            if (stopReq) { cnt[a]--; total--; return false; }
            if (depth == 2 && A2 > -1000) { printf("done a2=%d a3=%d nodes=%lld t=%.0f\n", A2, a - OFF, nodes, (double)(clock() - t0) / CLOCKS_PER_SEC); fflush(stdout); }
        }
        cnt[a]--; total--;
    }
    if (stopReq) return false;
    if (depth == 1 && A2 > -1000) { printf("A2DONE %d\n", A2); }
    memoPut(key, r);
    return false;
}
int main(int argc, char** argv) {
    H = atoi(argv[1]); M = atoi(argv[2]); MODE = atoi(argv[3]); RWIN = argc > 4 ? atoi(argv[4]) : 0;
    if (getenv("NOHB")) NOHB = 1;
    if (getenv("PASSIVE")) NOPASSIVE = 0;
    GMAX = gmaxOf(M); t0 = clock();
    for (int i = 0; i < 64; i++) RHO[i] = rho(i - 2);
    int memlog = getenv("MEMLOG") ? atoi(getenv("MEMLOG")) : 24;
    tab = (Ent*)calloc((size_t)1 << memlog, sizeof(Ent)); tabmask = ((u64)1 << memlog) - 1;
    if (!tab) { fprintf(stderr, "no memory\n"); return 3; }
    if (getenv("A2")) A2 = atoi(getenv("A2"));
    if (getenv("A3START")) A3START = atoi(getenv("A3START"));
    if (getenv("TLIM")) TLIM = atof(getenv("TLIM"));
    Cfg z; memset(z.v, 0, WW);
    vector<Cfg> S0(1, z), S1, S2;
    cnt[OFF] = 1; total = 1; LLO = OFF - 6 - (MODE ? 0 : RWIN); LHI = OFF + 6 + (MODE ? 0 : RWIN);
    stepSet(S0, OFF, S1);
    path.push_back(0);
    bool w;
    if (getenv("SECOND")) {
        int a = OFF + atoi(getenv("SECOND"));
        cnt[a]++; total++;
        if (MODE && fillersNeeded(M - 2, 0, WW - 1) > M - 2) w = false;
        else { LLO = min(LLO, a - 6); LHI = max(LHI, a + 6); stepSet(S1, a, S2); path.push_back(a - OFF); w = dfs(S2, M - 2); }
    } else w = dfs(S1, M - 1);
    printf("h=%d m=%d mode=%d R=%d gmax=%d%s%s%s%s : %s  nodes=%lld hbprunes=%lld memohits=%lld memo=%zu passprunes=%lld passDP=%lld\n", H, M, MODE, RWIN, GMAX, NOHB ? " NOHB" : "", NOPASSIVE ? " NOPASSIVE" : "",
           getenv("SECOND") ? " a2=" : "", getenv("SECOND") ? getenv("SECOND") : "", w ? "WIN" : (stopReq ? "STOPPED (time limit, incomplete)" : "no winning list"), nodes, hbprunes, memohits, (size_t)memosize, passprunes, passcalls);
    printf("memo evictions=%lld time=%.0fs\n", memoevict, (double)(clock() - t0) / CLOCKS_PER_SEC);
    if (w) { printf("list:"); for (int a : best) printf(" %d", a); printf("\n"); }
    return 0;
}
