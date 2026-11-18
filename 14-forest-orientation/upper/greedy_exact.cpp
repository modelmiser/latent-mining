// Exact worst case G(n) of rule R (the rule of Gupta, Kumar and Stein: flip a shortest directed path from the
// endpoint with the smaller h), with ties broken by the Adversary.  Quoted in NOTE.md, Section 5, for n <= 15.
// The Adversary plays the real game on fully known trees; the Algorithm must take a cheapest reply.
// This file is cut down from a larger solver for another game; with the first argument S >= n every tree is
// held exactly and the "pattern" code below is never reached.
// build: g++ -O2 -std=c++17 -DWIDE -DGREEDY -o greedy_exact greedy_exact.cpp
// usage: greedy_exact S D lo hi CAP MEM_GB MINUTES     as run: greedy_exact 20 1 1 16 9 20 600
//        (MEM_GB is a hard address-space limit set by the program; n = 14 took 22 minutes, n = 15 two hours)
#include <bits/stdc++.h>
#include <sys/resource.h>
using namespace std;
#ifdef WIDE
typedef uint32_t u16;   // component code (wide build: for S > 10)
#else
typedef uint16_t u16;
#endif
typedef uint64_t u64; typedef uint8_t u8;
static const int CW = sizeof(u16); static const long IDMAX = (CW == 2) ? 32768 : 1000000000;
static int S, D, LO, HI, CAP = 9; static double MEMGB = 10, MINUTES = 60;
static const int BOT = -1;
static chrono::steady_clock::time_point T0;
static double elapsed() { return chrono::duration<double>(chrono::steady_clock::now() - T0).count(); }

// ---------- patterns ----------
struct PK { int t, a, b; };                      // t=0: C(a)   t=1: N(a,b)
static map<array<int,3>, int> ptab; static vector<PK> prv;
static int mk(int t, int a, int b) {
    auto it = ptab.find({t, a, b}); if (it != ptab.end()) return it->second;
    int i = prv.size(); ptab[{t, a, b}] = i; prv.push_back({t, a, b}); return i;
}
static int Cp(int p) { return mk(0, min(p, CAP), 0); }
static int Np(int a, int b) {
    if (a > b) swap(a, b);
    if (a == BOT && b == BOT) return Cp(0);
    if (a != BOT && a == b && prv[a].t == 0) return Cp(prv[a].a + 1);
    return mk(1, a, b);
}
static pair<int,int> kids(int P) {
    PK k = prv[P];
    if (k.t == 0) { if (k.a == 0) return {BOT, BOT}; int c = Cp(k.a - 1); return {c, c}; }
    return {k.a, k.b};
}
static unordered_map<int,int> mcmemo;
static int mincost(int P) {
    PK k = prv[P]; if (k.t == 0) return k.a;
    if (k.a == BOT || k.b == BOT) return 0;
    auto it = mcmemo.find(P); if (it != mcmemo.end()) return it->second;
    int r = 1 + min(mincost(k.a), mincost(k.b)); mcmemo[P] = r; return r;
}
static map<pair<int,int>, int> trmemo;
static int trunc_(int P, int d) {
    if (P == BOT) return BOT;
    if (d == 0) return Cp(mincost(P));
    auto it = trmemo.find({P, d}); if (it != trmemo.end()) return it->second;
    auto [a, b] = kids(P);
    int ta = trunc_(a, d - 1), tb = trunc_(b, d - 1);
    int r = Np(ta, tb); trmemo[{P, d}] = r; return r;
}
static unordered_map<int, vector<vector<int>>> pathmemo;
static const vector<vector<int>>& paths(int P) {
    auto it = pathmemo.find(P); if (it != pathmemo.end()) return it->second;
    auto [a, b] = kids(P); set<vector<int>> out;
    if (a == BOT) out.insert({b});
    if (b == BOT) out.insert({a});
    pair<int,int> co[2] = {{a, b}, {b, a}};
    for (auto [c, o] : co) if (c != BOT) {
        vector<vector<int>> sub = paths(c);
        for (auto& s : sub) { vector<int> v{o}; v.insert(v.end(), s.begin(), s.end()); out.insert(v); }
    }
    return pathmemo[P] = vector<vector<int>>(out.begin(), out.end());
}

// ---------- exact trees ----------
typedef vector<vector<int>> Out;
typedef vector<vector<pair<int,int>>> Adj;
static Adj nbrs(const Out& out) {
    Adj ad(out.size());
    for (int v = 0; v < (int)out.size(); v++) for (int w : out[v]) { ad[v].push_back({w, 1}); ad[w].push_back({v, 0}); }
    return ad;
}
static string enc(const Adj& ad, int v, int par) {
    vector<string> cs;
    for (auto [w, d] : ad[v]) if (w != par) cs.push_back(string(1, char('0' + d)) + enc(ad, w, v));
    sort(cs.begin(), cs.end());
    string r = "("; for (auto& c : cs) r += c; r += ")"; return r;
}
static void decrec(const string& s, size_t& pos, int v, Out& out) {
    pos++;                                              // '('
    while (s[pos] != ')') {
        int d = s[pos] - '0'; pos++;
        int w = out.size(); out.push_back({});
        if (d == 1) out[v].push_back(w); else out[w].push_back(v);
        decrec(s, pos, w, out);
    }
    pos++;
}
static Out decode(const string& code) {
    Out out(1); size_t pos = 0; decrec(code, pos, 0, out);
    for (auto& o : out) sort(o.begin(), o.end());
    return out;
}
static unordered_map<string,int> etab; static vector<const string*> ecode;   // exact trees kept as canonical strings only
static vector<int> orbits(const Out& out) {
    Adj ad = nbrs(out); map<string,int> seen;
    for (int v = 0; v < (int)out.size(); v++) seen.insert({enc(ad, v, -1), v});
    vector<int> r; for (auto& kv : seen) r.push_back(kv.second);
    sort(r.begin(), r.end()); return r;
}
static int canon(const Out& out) {
    Adj ad = nbrs(out); string best;
    for (int v = 0; v < (int)out.size(); v++) { string c = enc(ad, v, -1); if (v == 0 || c < best) best = c; }
    auto it = etab.find(best); if (it != etab.end()) return it->second;
    int i = ecode.size(); auto ins = etab.emplace(best, i); ecode.push_back(&ins.first->first);
    return i;
}
static int hrec(const Out& out, int ev, int ep, int v) {
    int n = 0, m = INT_MAX;
    for (int w : out[v]) { n++; m = min(m, hrec(out, ev, ep, w)); }
    if (v == ev) { n++; m = min(m, mincost(ep)); }
    return n < 2 ? 0 : 1 + m;
}
static int oprec(const Out& out, int ev, int ep, int v, int d) {
    if (d == 0) return Cp(hrec(out, ev, ep, v));
    vector<int> sl;
    for (int w : out[v]) sl.push_back(oprec(out, ev, ep, w, d - 1));
    if (v == ev) sl.push_back(trunc_(ep, d - 1));
    while (sl.size() < 2) sl.push_back(BOT);
    if (sl.size() != 2) { fprintf(stderr, "three slots\n"); exit(3); }
    return Np(sl[0], sl[1]);
}
static int outpat(const Out& out, int ev, int ep, int r, int d) { return oprec(out, ev, ep, r, d); }
static void walk(const Out& out, vector<int>& path, vector<pair<int,Out>>& res) {
    for (int w : out[path.back()]) {
        path.push_back(w);
        if (out[w].size() < 2) {
            Out o = out;
            for (size_t i = 0; i + 1 < path.size(); i++) {
                int a = path[i], b = path[i + 1];
                o[a].erase(find(o[a].begin(), o[a].end(), b)); o[b].push_back(a);
            }
            for (auto& t : o) sort(t.begin(), t.end());
            res.push_back({(int)path.size() - 1, o});
        } else walk(out, path, res);
        path.pop_back();
    }
}
static vector<pair<int,Out>> exact_replies(const Out& out, int x) {
    if (out[x].size() < 2) return {{0, out}};
    vector<pair<int,Out>> res; vector<int> path{x}; walk(out, path, res); return res;
}
static Out join(const Out& o1, int x, const Out& o2, int y) {
    int n1 = o1.size(); Out o = o1;
    for (auto& t : o2) { vector<int> u; for (int w : t) u.push_back(w + n1); o.push_back(u); }
    o[x].push_back(y + n1);
    for (auto& t : o) sort(t.begin(), t.end());
    return o;
}

// ---------- components: u16 code, exact e -> 2e, pattern p -> 2p+1 ----------
static u16 EC(int e) { if (e >= IDMAX) { fprintf(stderr, "too many exact trees\n"); exit(3); } return 2 * e; }
static u16 PC(int p) { if (p >= IDMAX) { fprintf(stderr, "too many patterns\n"); exit(3); } return 2 * p + 1; }
struct Opt { int cost; vector<u16> cands; bool operator<(const Opt& o) const { return cost != o.cost ? cost < o.cost : cands < o.cands; } };
typedef vector<Opt> Named;
static unordered_map<u64, u64> mvmemo; static vector<uint32_t> pool;   // pool: [count] then per named pair [cost, k, k candidates]
static u64 moves(u16 A, u16 B) {
    u64 key = (u64)A << 32 | B;
    auto it = mvmemo.find(key); if (it != mvmemo.end()) return it->second;
    vector<int> na = (A & 1) ? vector<int>{-1} : orbits(decode(*ecode[A >> 1]));
    vector<int> nb = (B & 1) ? vector<int>{-1} : orbits(decode(*ecode[B >> 1]));
    set<Named> result;
    for (int x : na) for (int y : nb) {
        set<Opt> opts;
        for (int dir = 0; dir < 2; dir++) {
            u16 X = dir ? B : A, Y = dir ? A : B; int vx = dir ? y : x, vy = dir ? x : y;
            if (!(X & 1)) {
                Out ox = decode(*ecode[X >> 1]);
                for (auto& [cost, o2] : exact_replies(ox, vx)) {
                    set<u16> cands;
                    if (!(Y & 1)) {
                        Out oy = decode(*ecode[Y >> 1]); Out z = join(o2, vx, oy, vy);
                        if ((int)z.size() <= S) cands.insert(EC(canon(z)));
                        else for (int r = 0; r < (int)z.size(); r++) cands.insert(PC(outpat(z, -1, 0, r, D)));
                    } else {
                        for (int r = 0; r < (int)o2.size(); r++) cands.insert(PC(outpat(o2, vx, Y >> 1, r, D)));
                        cands.insert(Y);
                    }
                    opts.insert({cost, vector<u16>(cands.begin(), cands.end())});
                }
            } else {
                int Q; set<u16> extra;
                if (!(Y & 1)) {
                    Out oy = decode(*ecode[Y >> 1]); Q = outpat(oy, -1, 0, vy, D + 8);
                    for (int r = 0; r < (int)oy.size(); r++) extra.insert(PC(outpat(oy, -1, 0, r, D)));
                } else { Q = Y >> 1; extra.insert(Y); }
                vector<vector<int>> ps = paths(X >> 1);
                for (auto& s : ps) {
                    int w = Np(s[0], Q); set<u16> cands = extra; cands.insert(PC(trunc_(w, D)));
                    for (size_t i = 1; i < s.size(); i++) { w = Np(s[i], w); cands.insert(PC(trunc_(w, D))); }
                    opts.insert({(int)s.size() - 1, vector<u16>(cands.begin(), cands.end())});
                }
            }
        }
        Named keep;
        for (auto& o : opts) {
            bool dom = false;
#ifndef GREEDY
            for (auto& k : keep) if (k.cost <= o.cost && includes(o.cands.begin(), o.cands.end(), k.cands.begin(), k.cands.end())) { dom = true; break; }
#endif
            if (!dom) keep.push_back(o);
        }
        result.insert(keep);
    }
    set<pair<int, vector<u16>>> flat;                 // greedy: cheapest replies only, ties to the Adversary
    for (auto& nm : result) {
        int c0 = nm[0].cost; set<u16> cs;
        for (auto& o : nm) if (o.cost == c0) cs.insert(o.cands.begin(), o.cands.end());
        flat.insert({c0, vector<u16>(cs.begin(), cs.end())});
    }
    u64 off = pool.size(); pool.push_back(flat.size());
    for (auto& f : flat) { pool.push_back(f.first); pool.push_back(f.second.size()); for (u16 w : f.second) pool.push_back(w); }
    return mvmemo[key] = off;
}

// ---------- memo: open addressing over an arena of states ----------
static const int CHB = 26; static vector<u8*> chunks; static size_t cpos = (size_t)1 << CHB;
static vector<u64> tab; static size_t tmask, tcnt = 0;
static u64 hashst(const u16* st, int L) {
    u64 h = 0x9E3779B97F4A7C15ull ^ L;
    for (int i = 0; i < L; i++) { h ^= st[i]; h *= 0xff51afd7ed558ccdull; h ^= h >> 29; }
    h ^= h >> 32; h *= 0xc4ceb9fe1a85ec53ull; h ^= h >> 31; return h;
}
static inline u8* refp(u64 ref) { return chunks[ref >> CHB] + (ref & (((u64)1 << CHB) - 1)); }
static double membytes() { return (double)chunks.size() * ((size_t)1 << CHB) + tab.size() * 8.0; }
static void bail(const char* why) { printf("# stopped: %s (%.0f s, %zu states, %.1f GB)\n", why, elapsed(), tcnt, membytes() / 1e9); fflush(stdout); exit(2); }
static void grow() {
    if ((membytes() + tab.size() * 16.0) / 1e9 > MEMGB) (void)0;
    vector<u64> old; old.swap(tab); tab.assign(old.size() * 2, 0); tmask = tab.size() - 1;
    for (u64 e : old) if (e) {
        u8* p = refp((e >> 16) - 1); int L = p[0]; u16 buf[256]; memcpy(buf, p + 1, CW * L);
        size_t i = hashst(buf, L) & tmask; while (tab[i]) i = (i + 1) & tmask; tab[i] = e;
    }
}
static int lookup(const u16* st, int L, u64 h) {
    size_t i = h & tmask; u64 tag = (h >> 56) & 0xff;
    while (u64 e = tab[i]) {
        if (((e >> 8) & 0xff) == tag) { u8* p = refp((e >> 16) - 1); if (p[0] == L && !memcmp(p + 1, st, CW * L)) return e & 0xff; }
        i = (i + 1) & tmask;
    }
    return -1;
}
static void insert(const u16* st, int L, u64 h, int val) {
    if ((tcnt + 1) * 10 > tab.size() * 7) grow();
    size_t need = 1 + CW * L;
    if (cpos + need > ((size_t)1 << CHB)) {
        if ((membytes() + (1 << CHB)) / 1e9 > MEMGB) (void)0;
        chunks.push_back((u8*)malloc((size_t)1 << CHB)); cpos = 0;
    }
    u64 ref = ((u64)(chunks.size() - 1) << CHB) | cpos; u8* p = chunks.back() + cpos; cpos += need;
    p[0] = L; memcpy(p + 1, st, CW * L);
    size_t i = h & tmask; while (tab[i]) i = (i + 1) & tmask;
    tab[i] = ((ref + 1) << 16) | (((h >> 56) & 0xff) << 8) | (u64)val; tcnt++;
    if ((tcnt & 0xfffff) == 0 && elapsed() > MINUTES * 60) bail("time cap");
}
// ---------- threshold search (build with -DNULLWIN): W(st,t) <=> value(st) >= t; memo keeps bounds lo <= value <= hi ----------
static u64* slot(const u16* st, int L, u64 h) {
    size_t i = h & tmask;
    while (u64 e = tab[i]) {
        u8* p = refp((e >> 16) - 1); if (p[0] == L && !memcmp(p + 1, st, CW * L)) return &tab[i];
        i = (i + 1) & tmask;
    }
    return nullptr;
}
static void setb(const u16* st, int L, u64 h, int lo, int hi) {      // tighten the bounds of a state (insert if new)
    u64* e = slot(st, L, h);
    if (!e) {
        if ((tcnt + 1) * 10 > tab.size() * 7) grow();
        size_t need = 1 + CW * L;
        if (cpos + need > ((size_t)1 << CHB)) {
            if ((membytes() + (1 << CHB)) / 1e9 > MEMGB) (void)0;
            chunks.push_back((u8*)malloc((size_t)1 << CHB)); cpos = 0;
        }
        u64 ref = ((u64)(chunks.size() - 1) << CHB) | cpos; u8* p = chunks.back() + cpos; cpos += need;
        p[0] = L; memcpy(p + 1, st, CW * L);
        size_t i = h & tmask; while (tab[i]) i = (i + 1) & tmask;
        tab[i] = ((ref + 1) << 16) | ((u64)hi << 8) | (u64)lo; tcnt++;
        if ((tcnt & 0xfffff) == 0 && elapsed() > MINUTES * 60) bail("time cap");
        return;
    }
    int l0 = *e & 0xff, h0 = (*e >> 8) & 0xff;
    lo = max(lo, l0); hi = min(hi, h0);
    if (lo > hi) { fprintf(stderr, "bounds crossed\n"); exit(3); }
    *e = (*e & ~(u64)0xffff) | ((u64)hi << 8) | (u64)lo;
}
static int V(const u16* st, int L) {
    if (L <= 1) return 0;
    u64 h = hashst(st, L); int got = lookup(st, L, h); if (got >= 0) return got;
    int best = 0; u16 ch[256];
    for (int a = 0; a < L; a++) {
        if (a > 0 && st[a] == st[a - 1]) continue;
        for (int b = a + 1; b < L; b++) {
            if (b > a + 1 && st[b] == st[b - 1]) continue;
            u64 off = moves(st[a], st[b]); int cnt = pool[off++];
            for (int q = 0; q < cnt; q++) {
                int c0 = pool[off], kk = pool[off + 1]; uint32_t cs[512];
                if (kk > 512) { fprintf(stderr, "too many candidates\n"); exit(3); }
                memcpy(cs, &pool[off + 2], 4 * kk); off += 2 + kk;      // copy: the pool may move during recursion
                int worst = 0;
                for (int j = 0; j < kk; j++) {
                    u16 w = cs[j]; int k = 0; bool put = false;
                    for (int i = 0; i < L; i++) { if (i == a || i == b) continue; if (!put && w < st[i]) { ch[k++] = w; put = true; } ch[k++] = st[i]; }
                    if (!put) ch[k++] = w;
                    int v = c0 + V(ch, k); if (v > worst) worst = v;
                }
                if (worst > best) best = worst;
            }
        }
    }
    if (best > 250) { fprintf(stderr, "value overflow\n"); exit(3); }
    insert(st, L, h, best); return best;
}
int main(int argc, char** argv) {
    if (argc < 5) { fprintf(stderr, "usage: greedy_exact S D lo hi [CAP] [MEM_GB] [MINUTES]\n"); return 1; }
    S = atoi(argv[1]); D = atoi(argv[2]); LO = atoi(argv[3]); HI = atoi(argv[4]);
    if (argc > 5) CAP = atoi(argv[5]);
    if (argc > 6) MEMGB = atof(argv[6]);
    if (argc > 7) MINUTES = atof(argv[7]);
    if (HI > 250) return 1;
#ifdef GREEDY
    if (S < HI) { fprintf(stderr, "S must be at least hi, or trees are not held exactly\n"); return 1; }
#endif
    { struct rlimit rl; rl.rlim_cur = rl.rlim_max = (rlim_t)(MEMGB * 1e9); if (setrlimit(RLIMIT_AS, &rl)) { perror("setrlimit"); return 1; } }   // hard limit on ALL memory
    set_new_handler([] { printf("# stopped: hard memory limit\n"); fflush(stdout); _exit(2); });
    T0 = chrono::steady_clock::now(); tab.assign(1 << 20, 0); tmask = tab.size() - 1;
    int one = canon(Out(1)); u16 st[256];
    for (int n = LO; n <= HI; n++) {
        for (int i = 0; i < n; i++) st[i] = EC(one);
        int v = V(st, n), B = max(0, n / 4 - 1);
        printf("%d %d B=%d %s states %zu exact %zu patterns %zu  %.0fs %.1fGB\n", n, v, B, v > B ? ">B" : "", tcnt, ecode.size(), prv.size(), elapsed(), membytes() / 1e9);
        fflush(stdout);
    }
    return 0;
}
