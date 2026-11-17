// open-f-upper/rt.cpp : fixed-rule managers for game F (also N, O), tested two ways.
//   rt exh G M A V [Hcap]     exhaustive reachability vs EVERY adversary; prints max extent (= cells the rule needs)
//                             with Hcap: stops with FAIL as soon as an extent > Hcap is reached
//   rt att M A V variant par  scripted adversary (see attack()); prints max extent reached
// Layout cell codes: 0 free, 1/2/3 = first cell of a block of that size, 4 = later cell of a block.
// A = allocation rule, V = move rule (used only in game F after a freed 2 or 3).
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 u128;
#ifndef WW
#define WW 64
#endif
const int W = WW; // exh mode needs W<=64 (128-bit key); att mode: build with -DWW=600 (binary rtw)
struct Lay { unsigned char c[W]; };
static int M, A, V; static char G = 'F';
static int ext(const Lay& l) { for (int i = W - 1; i >= 0; i--) if (l.c[i]) return i + 1; return 0; }
static int live(const Lay& l) { int n = 0; for (int i = 0; i < W; i++) if (l.c[i]) n++; return n; }
static bool fits(const Lay& l, int p, int s) { if (p < 0 || p + s > W) return false; for (int j = 0; j < s; j++) if (l.c[p + j]) return false; return true; }
static void put(Lay& l, int p, int s) { l.c[p] = s; for (int j = 1; j < s; j++) l.c[p + j] = 4; }
static void lift(Lay& l, int p) { int s = l.c[p]; for (int j = 0; j < s; j++) l.c[p + j] = 0; }
static u128 enc(const Lay& l) { u128 k = 0; for (int i = W - 1; i >= 0; i--) { int v = l.c[i]; int b = v == 0 ? 0 : v == 1 ? 1 : v == 4 ? 3 : 2; k = (k << 2) | b; } return k; }
static Lay dec(u128 k) { Lay l; int i = 0; while (i < W) { int b = (int)(k >> (2 * i)) & 3; if (b == 0) { l.c[i++] = 0; } else if (b == 1) { l.c[i++] = 1; } else { int s = 1; while (i + s < W && ((int)(k >> (2 * (i + s))) & 3) == 3) s++; l.c[i] = s; for (int j = 1; j < s; j++) l.c[i + j] = 4; i += s; } } return l; }
static string show(const Lay& l) { string s; int e = ext(l); for (int i = 0; i < e; i++) s += ".123="[l.c[i]]; return s; }
static long cost(const Lay& l);
// ---------------- allocation rules ----------------
static int holeLen(const Lay& l, int p, int e) { int j = p; while (j < e && !l.c[j]) j++; return j - p; } // p = hole start
static int place(const Lay& l, int s) {
    int e = ext(l);
    if (A == 0) { for (int p = 0; p + s <= W; p++) if (fits(l, p, s)) return p; return -1; }          // first fit
    if (A == 1) { // 1s first fit; 2s,3s: lowest hole of exactly that size, else first fit
        if (s > 1) for (int p = 0; p < e; p++) if (!l.c[p] && (p == 0 || l.c[p - 1])) if (holeLen(l, p, e) == s) return p;
        for (int p = 0; p + s <= W; p++) if (fits(l, p, s)) return p; return -1; }
    if (A == 2) { // best fit: smallest hole that fits (lowest among equals), else top
        int best = -1, bl = 1000; for (int p = 0; p < e; p++) if (!l.c[p] && (p == 0 || l.c[p - 1])) { int L = holeLen(l, p, e); if (L >= s && L < bl) { bl = L; best = p; } }
        return best >= 0 ? best : e; }
    if (A == 3) { // 1s first fit; 2s,3s: exact hole, else top (never split a larger hole), else first fit if top would exceed nothing (no cap here)
        if (s == 1) { for (int p = 0; p < W; p++) if (!l.c[p]) return p; return -1; }
        for (int p = 0; p < e; p++) if (!l.c[p] && (p == 0 || l.c[p - 1])) if (holeLen(l, p, e) == s) return p;
        if (s == 3) { for (int p = 0; p + s <= W; p++) if (fits(l, p, s)) return p; return -1; }
        // s == 2: holes of size >= 4 or 3? prefer hole with L%3==2, then any, lowest
        for (int p = 0; p < e; p++) if (!l.c[p] && (p == 0 || l.c[p - 1])) { int L = holeLen(l, p, e); if (L >= 2 && L % 3 == 2) return p; }
        for (int p = 0; p + s <= W; p++) if (fits(l, p, s)) return p; return -1; }
    if (A == 4) { // potential-greedy placement: any hole position or the top, minimising cost(); ties -> lowest address
        long bc = LONG_MAX; int bp = -1; for (int p = 0; p <= e && p + s <= W; p++) if (fits(l, p, s)) { Lay d = l; put(d, p, s); long v = cost(d); if (v < bc) { bc = v; bp = p; } } return bp; }
    if (A == 9) { // NEGATIVE CONTROL: "last fit" - highest hole below the extent that fits, else top
        for (int p = e - s; p >= 0; p--) if (fits(l, p, s)) return p; return e; }
    return -1;
}
// ---------------- potential: what an adversary gets by "free 1s cleverly, then allocate 3s" ----------------
// keep[] = greedy minimum set of 1s that leaves no NEW free 3-window; e2 = extent after dropping the other 1s;
// slots = disjoint free 3-windows below e2.  fluid = e2 + max(0, M - livekept - 3*slots)  (cells needed if 3s were divisible)
// integer = e2 + 3*max(0, floor((M-livekept)/3) - slots).   COSTMODE picks how they are combined.
static int COSTMODE = 0;
static long cost(const Lay& l) { int e = ext(l); unsigned char keep[W + 3]; memset(keep, 0, sizeof keep); int lk = 0;
    for (int r = 2; r < e; r++) { if (l.c[r] > 1 || l.c[r - 1] > 1 || l.c[r - 2] > 1) continue; if (!(l.c[r] | l.c[r - 1] | l.c[r - 2])) continue; if (keep[r] | keep[r - 1] | keep[r - 2]) continue; int j = r; while (l.c[j] != 1) j--; keep[j] = 1; lk++; }
    int e2 = 0, slots = 0, run = 0, w = 0; for (int i = 0; i < e; i++) { if (l.c[i] > 1) lk++; if (l.c[i] > 1 || keep[i]) e2 = i + 1; }
    for (int i = 0; i < e2; i++) { if (l.c[i] > 1 || keep[i]) { slots += run / 3; w += run % 3; run = 0; } else run++; }
    int fl = e2 + max(0, M - lk - 3 * slots), in = e2 + 3 * max(0, (M - lk) / 3 - slots); int t = -1; for (int i = e - 1; i >= 0; i--) if (l.c[i] == 1) { t = i; break; }
    if (COSTMODE == 0) return (long)fl * 1000000 + (long)e * 1000 + t;            // fluid, then real extent, then highest 1
    if (COSTMODE == 1) return (long)in * 1000000 + (long)fl * 1000 + e;            // integer, then fluid
    if (COSTMODE == 2) return (long)max(fl, e) * 1000000 + (long)w * 1000 + t;
    // modes 3,4: fluid cost with a bonus for 2-holes bordered by movable 1s (one more move turns them into a 3-hole)
    int n2h = 0; for (int i = 1; i + 2 < e; i++) if (l.c[i - 1] == 1 && !l.c[i] && !l.c[i + 1] && l.c[i + 2] == 1) n2h++;
    if (COSTMODE == 3) return (long)(2 * fl - n2h) * 1000000 + (long)e * 1000 + t;
    return (long)(3 * fl - n2h) * 1000000 + (long)e * 1000 + t; }
// ---------------- move rules ----------------
static int g3(const Lay& l) { int e = ext(l), g = 0; for (int p = 0; p < e;) { if (l.c[p]) { p++; continue; } int L = holeLen(l, p, e); g += L / 3; p += L; } return g; }
static int top1(const Lay& l) { for (int i = W - 1; i >= 0; i--) if (l.c[i] == 1) return i; return -1; }
static bool compact1(Lay& l) { int t = top1(l); if (t < 0) return false; for (int p = 0; p < t; p++) if (!l.c[p]) { l.c[t] = 0; l.c[p] = 1; return true; } return false; }
static bool compact2(Lay& l) { for (int t = W - 2; t >= 0; t--) if (l.c[t] == 2) { Lay d = l; lift(d, t); for (int p = 0; p < t; p++) if (fits(d, p, 2)) { put(d, p, 2); l = d; return true; } return false; } return false; }
static bool greedy1(Lay& l, int mode) { // best single 1-move by a potential (smaller better); only if it strictly improves
    int e = ext(l); auto pot = [&](const Lay& x) -> long { int t = top1(x);
        if (mode == 0) return -(long)g3(x) * 100000 + (long)ext(x) * 100 + t;       // 3-holes first, then extent, then top 1
        return (long)ext(x) * 100000 - (long)g3(x) * 100 + t; };                    // extent first
    long bp = pot(l); Lay b = l; bool ch = false;
    for (int i = 0; i < e; i++) if (l.c[i] == 1) { Lay d = l; d.c[i] = 0; for (int q = 0; q < e; q++) if (q != i && !d.c[q]) { d.c[q] = 1; long v = pot(d); if (v < bp) { bp = v; b = d; ch = true; } d.c[q] = 0; } }
    l = b; return ch; }
static void domoves(Lay& l, int budget, int freedPos) {
    if (V == 0 || budget <= 0) return;
    if (V == 1) { while (budget-- > 0) if (!compact1(l)) break; return; }                       // compaction of 1s
    if (V == 2) { if (budget == 2 && compact2(l)) return; while (budget-- > 0) if (!compact1(l)) break; return; } // + 2s
    if (V == 3) { while (budget-- > 0) if (!greedy1(l, 0)) { if (!compact1(l)) break; } return; }
    if (V == 4) { while (budget-- > 0) if (!greedy1(l, 1)) { if (!compact1(l)) break; } return; }
    if (V == 5) { // top-local compaction: highest 1 into the highest free cell below it
        while (budget-- > 0) { int t = top1(l); if (t < 0) break; int p = t - 1; while (p >= 0 && l.c[p]) p--; if (p < 0) break; l.c[t] = 0; l.c[p] = 1; } return; }
    if (V == 6 || V == 7 || V == 10) { // V=10: only 1-moves, destination below the highest 1 (1s never leave [0,M)); compaction when nothing improves // potential-greedy: best single move (1-move, or a 2-move when the budget is 2) by cost(); repeat while budget lasts
        while (budget > 0) { int e = ext(l); long bc = cost(l); Lay b = l; int used = 0;
            for (int i = 0; i < e; i++) { int sz = l.c[i]; if (sz != 1 && sz != 2) continue; if (sz > budget) continue; if (sz == 2 && V >= 7) continue; int t1 = top1(l); Lay d = l; lift(d, i);
                for (int q = 0; q <= e && q + sz <= W; q++) if (q != i && fits(d, q, sz)) { if (V == 10 && q > t1) break; put(d, q, sz); long v = cost(d); if (v < bc) { bc = v; b = d; used = sz; } lift(d, q); } }
            if (!used) { if (V == 10 && compact1(l)) { budget--; continue; } break; } l = b; budget -= used; } return; }
    if (V == 8) { // NEGATIVE CONTROL: move the lowest 1 into the hole just freed
        int p = 0; while (p < W && l.c[p] != 1) p++; if (p < W && p != freedPos && !l.c[freedPos]) { l.c[p] = 0; l.c[freedPos] = 1; } return; }
}
// ---------------- exhaustive reachability ----------------
struct Tab { vector<u128> k; size_t mask, n = 0; Tab(size_t cap) : k(cap, ~(u128)0), mask(cap - 1) {}
    bool ins(u128 x) { size_t i = (size_t)((uint64_t)(x ^ (x >> 61)) * 0x9E3779B97F4A7C15ULL >> 20) & mask; while (k[i] != ~(u128)0) { if (k[i] == x) return false; i = (i + 1) & mask; } k[i] = x; n++; return true; } };
static int exhaustive(int Hcap, int logcap) {
    Tab seen((size_t)1 << logcap); vector<u128> q; q.push_back(0); seen.ins(0); int mx = 0; Lay worst{}; size_t lim = ((size_t)1 << logcap) * 7 / 10;
    for (size_t h = 0; h < q.size(); h++) { Lay l = dec(q[h]); int lv = live(l);
        for (int s = 1; s <= 3; s++) if (lv + s <= M) { int p = place(l, s); if (p < 0 || !fits(l, p, s)) { printf("BADPLACE\n"); return 2; } Lay d = l; put(d, p, s); int e = ext(d);
            if (e > mx) { mx = e; worst = d; if (Hcap && e > Hcap) { printf("G=%c M=%d A=%d V=%d FAIL cap=%d reached extent %d: %s  (after %zu states)\n", G, M, A, V, Hcap, e, show(d).c_str(), q.size()); return 1; } }
            if (seen.ins(enc(d))) q.push_back(enc(d)); }
        for (int p = 0; p < W; p++) { int s = l.c[p]; if (s < 1 || s > 3) continue; if (G == 'O' && s != 1) continue; Lay d = l; lift(d, p); if (G == 'F') domoves(d, s - 1, p);
            if (live(d) != lv - s) { printf("BADMOVE\n"); return 2; }
            if (seen.ins(enc(d))) q.push_back(enc(d)); }
        if (seen.n > lim) { printf("G=%c M=%d A=%d V=%d INCOMPLETE table full at %zu states, max extent so far %d\n", G, M, A, V, seen.n, mx); return 3; } }
    printf("G=%c M=%d A=%d V=%d reachable=%zu H_needed=%d ceil(5M/3)=%d floor(7M/4)=%d worst=%s%s\n", G, M, A, V, q.size(), mx, (5 * M + 2) / 3, 7 * M / 4, show(worst).c_str(), Hcap ? "  PASS(cap)" : "");
    return 0; }
// ---------------- scripted adversary ----------------
static Lay cur; static int mxe; static bool verbose;
static void say(const char* t) { if (verbose) printf("  %-28s live=%2d ext=%2d %s\n", t, live(cur), ext(cur), show(cur).c_str()); }
static bool al(int s) { if (live(cur) + s > M) return false; int p = place(cur, s); put(cur, p, s); mxe = max(mxe, ext(cur)); return true; }
static void fr(int p) { int s = cur.c[p]; lift(cur, p); domoves(cur, s - 1, p); }
static void thin() { // keep a minimum set of 1s such that no NEW free 3-window appears (greedy interval point cover)
    int e = ext(cur); vector<char> keep(W, 0);
    for (int r = 2; r < e; r++) { bool ok = true, has = false, hit = false; for (int j = r - 2; j <= r; j++) { if (cur.c[j] > 1) ok = false; if (cur.c[j] == 1) has = true; if (keep[j]) hit = true; }
        if (!ok || !has || hit) continue; for (int j = r; j >= r - 2; j--) if (cur.c[j] == 1) { keep[j] = 1; break; } }
    for (int p = 0; p < e; p++) if (cur.c[p] == 1 && !keep[p]) fr(p); }
static int attack(int variant, int par) {
    memset(&cur, 0, sizeof cur); mxe = 0;
    for (int i = 0; i < M; i++) al(1); say("M ones");
    if (variant == 0) { thin(); say("thin (O attack)"); while (al(3)); say("3s"); while (al(2)); while (al(1)); return mxe; }
    // variant 1: keep two residue classes ("11.11."), 2s, free every second upper 2, thin, 3s.  par bit0: which parity is freed; par>>1: residue freed
    int cls = (par >> 1) % 3; for (int p = 0; p < W; p++) if (cur.c[p] == 1 && p % 3 == cls) fr(p); say("drop one class");
    while (al(2)); say("2s");
    int t1 = top1(cur); vector<int> twos; for (int p = t1 + 1; p < W; p++) if (cur.c[p] == 2) twos.push_back(p);
    for (size_t i = (par & 1); i < twos.size(); i += 2) fr(twos[i]); say("free alternate 2s (+moves)");
    thin(); say("thin");
    while (al(3)); say("3s"); while (al(2)); while (al(1)); say("fill");
    return mxe; }
// ---------------- random structured adversary (for M too large for exhaustive play) ----------------
// A script is a list of steps; each step is one of: fill with s-blocks (all or a fraction), free 1s by residue mask,
// free 2s / 3s by rank pattern, thin().  Random scripts + mutation of the best one.  Gives a LOWER bound on what a rule needs.
struct Step { int type, a, b; }; // 0 fill s=a (b = percent of possible) ; 1 free 1s: period a, mask b (bit set = free) ; 2 free s-blocks (s=a>>8) by rank: period a&255, mask b ; 3 thin
static int runScript(const vector<Step>& sc) { memset(&cur, 0, sizeof cur); mxe = 0;
    for (const Step& st : sc) { if (mxe > W - 8) return mxe;
        if (st.type == 0) { int can = (M - live(cur)) / st.a; int n = can * st.b / 100; if (st.b >= 100) n = can; for (int i = 0; i < n; i++) { if (ext(cur) > W - 8) return mxe; if (!al(st.a)) break; } }
        else if (st.type == 1) { for (int p = 0; p < W; p++) if (cur.c[p] == 1 && (st.b >> (p % st.a) & 1)) fr(p); }
        else if (st.type == 2) { int sz = st.a >> 8, per = st.a & 255; vector<int> pos; for (int p = 0; p < W; p++) if (cur.c[p] == sz) pos.push_back(p); // ranks fixed before freeing; blocks of this size that get MOVED meanwhile are skipped
            for (size_t i = 0; i < pos.size(); i++) if ((st.b >> (i % per) & 1) && cur.c[pos[i]] == sz) fr(pos[i]); }
        else thin(); }
    return mxe; }
static string stepStr(const Step& s) { char b[64]; if (s.type == 0) sprintf(b, "fill%d(%d%%)", s.a, s.b); else if (s.type == 1) sprintf(b, "free1(mod%d,mask%d)", s.a, s.b); else if (s.type == 2) sprintf(b, "free%d(rank mod%d,mask%d)", s.a >> 8, s.a & 255, s.b); else sprintf(b, "thin"); return b; }
static Step rndStep(mt19937& g) { Step s; int t = g() % 10; if (t < 4) { s.type = 0; s.a = 1 + g() % 3; s.b = (g() % 3) ? 100 : 10 + g() % 90; } else if (t < 6) { s.type = 1; s.a = 2 + g() % 5; s.b = 1 + g() % ((1 << s.a) - 1); } else if (t < 9) { int sz = 2 + g() % 2, per = 2 + g() % 4; s.type = 2; s.a = sz << 8 | per; s.b = 1 + g() % ((1 << per) - 1); } else { s.type = 3; s.a = s.b = 0; } return s; }
static int randomAdv(int trials, int seed) { mt19937 g(seed); int best = 0; vector<Step> bs;
    for (int t = 0; t < trials; t++) { vector<Step> sc; if (bs.empty() || g() % 3 == 0) { int n = 3 + g() % 8; sc.push_back({0, 1, 100}); for (int i = 0; i < n; i++) sc.push_back(rndStep(g)); sc.push_back({3, 0, 0}); sc.push_back({0, 3, 100}); }
        else { sc = bs; int m = 1 + g() % 2; while (m--) { int k = g() % 3; size_t i = g() % sc.size(); if (k == 0) sc[i] = rndStep(g); else if (k == 1) sc.insert(sc.begin() + i, rndStep(g)); else if (sc.size() > 2) sc.erase(sc.begin() + i); } }
        int e = runScript(sc); if (e > best) { best = e; bs = sc; } }
    printf("M=%d A=%d V=%d cm=%d random-adversary best extent=%d ratio=%.4f (5M/3=%.1f 7M/4=%.1f) script:", M, A, V, COSTMODE, best, (double)best / M, 5.0 * M / 3, 7.0 * M / 4); for (auto& s : bs) printf(" %s", stepStr(s).c_str()); printf("\n"); return best; }
int main(int argc, char** argv) { if (getenv("COSTMODE")) COSTMODE = atoi(getenv("COSTMODE"));
    string mode = argv[1];
    if (mode == "exh") { G = argv[2][0]; M = atoi(argv[3]); A = atoi(argv[4]); V = atoi(argv[5]); int cap = argc > 6 ? atoi(argv[6]) : 0; int lc = argc > 7 ? atoi(argv[7]) : 26; return exhaustive(cap, lc); }
    if (mode == "att") { M = atoi(argv[2]); A = atoi(argv[3]); V = atoi(argv[4]); int var = atoi(argv[5]); int par = argc > 6 ? atoi(argv[6]) : 0; verbose = argc > 7;
        int e = attack(var, par); printf("M=%d A=%d V=%d variant=%d par=%d extent=%d  5M/3=%.1f 7M/4=%.1f\n", M, A, V, var, par, e, 5.0 * M / 3, 7.0 * M / 4); return 0; }
    if (mode == "rnd") { M = atoi(argv[2]); A = atoi(argv[3]); V = atoi(argv[4]); randomAdv(atoi(argv[5]), argc > 6 ? atoi(argv[6]) : 1); return 0; }
    return 9; }
