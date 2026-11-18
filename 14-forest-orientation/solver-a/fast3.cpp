// Threshold (budget) solver, compact trees.  Same game model as fast.cpp "fast" mode.
// usage: fast3 N CAP [ADVMODE] [BSTART]   env ALGP=k: Algorithm restricted to replies whose new tree has maxd<=k (upper bound);
// env TERMJ=j: play the W_j game: terminal payoff psi_j(final tree) (see proofs.txt).
// env ALGC=c: Algorithm restricted to replies of cost <= c per round (upper bound). Last round is unrestricted.
// win(state,b) == true  <=>  Adversary can force total future payment > b from state.
// T(n) = least b with win(start,b) false.
// Safe reductions used (see proofs.txt):
//  (L) Algorithm never touches components not involved in the current merge (laziness lemma)
//  (C) with 2 components left the value is min(maxd(A),maxd(B))
//  (B) replies of cost > b are irrelevant for the question "value <= b"
//  orbit reduction of the vertices u, v named by Adversary (tree automorphisms)
// ADVMODE 3 with env SPLIT=a: Adversary keeps a fixed bipartition (a | N-a) and merges across it only in the last round (lower bound).
// ADVMODE 5: Adversary merges only 1+1, 2+2, or two components whose sizes are multiples of 4 (lower bound).
// ADVMODE 6: only 1+1 or two even-sized components (lower bound).
// ADVMODE: 0 exact; 1 Adversary only merges equal sizes (lower bound); 2 equal sizes or total<=4 (lower bound)
#include <bits/stdc++.h>
using namespace std;
typedef uint32_t u32;
typedef unsigned __int128 u128;
typedef array<uint8_t,2> Arc;
static int CAP = 2, ADV = 0; static size_t OPTLIM = 60000000; static int ALGP = 99, ALGC = 99, TERMJ = 0, SPLIT = 0;
static const u32 SEP = 0xFFFFFFFFu;

struct LT { int m; vector<Arc> arcs; };
static vector<u128> tcode; static vector<uint8_t> tm_, tmaxd;
struct H128 { size_t operator()(const u128& x) const { uint64_t a = (uint64_t)x, b = (uint64_t)(x >> 64); a ^= b * 0x9E3779B97F4A7C15ULL; a ^= a >> 31; a *= 0xBF58476D1CE4E5B9ULL; a ^= a >> 29; return a; } };
static unordered_map<u128, u32, H128> codeId;

static vector<pair<int,int>> adjv[32];
static string codeRec(int v, int p) {
    vector<string> ch;
    for (auto &e : adjv[v]) if (e.first != p) ch.push_back(string(1, e.second ? 'a' : 'b') + codeRec(e.first, v));
    sort(ch.begin(), ch.end());
    string s = "(";
    for (auto &c : ch) s += c;
    s += ")";
    return s;
}
static int szRec(int v, int p, int m, vector<int>& cent) {
    int s = 1, mx = 0;
    for (auto &e : adjv[v]) if (e.first != p) { int c = szRec(e.first, v, m, cent); s += c; mx = max(mx, c); }
    mx = max(mx, m - s);
    if (2 * mx <= m) cent.push_back(v);
    return s;
}
static void buildAdj(const LT& t) {
    for (int i = 0; i < t.m; i++) adjv[i].clear();
    for (auto &a : t.arcs) { adjv[a[0]].push_back({a[1], 1}); adjv[a[1]].push_back({a[0], 0}); }
}
static string canon(const LT& t) {
    buildAdj(t);
    vector<int> cent; szRec(0, -1, t.m, cent);
    string best;
    for (int c : cent) { string s = codeRec(c, -1); if (best.empty() || s < best) best = s; }
    return best;
}
static u128 pack(const string& s) { u128 x = 1; for (char c : s) { int d = c == '(' ? 0 : c == ')' ? 1 : c == 'a' ? 2 : 3; x = (x << 2) | d; } return x; }
static string unpack(u128 x) { string s; while (x > 1) { int d = (int)(x & 3); s += "()ab"[d]; x >>= 2; } reverse(s.begin(), s.end()); return s; }
static int parseRec(const string& s, size_t& i, LT& t) {
    int node = t.m++;
    i++;
    while (s[i] != ')') {
        char d = s[i++];
        int ch = parseRec(s, i, t);
        if (d == 'a') t.arcs.push_back({(uint8_t)node, (uint8_t)ch}); else t.arcs.push_back({(uint8_t)ch, (uint8_t)node});
    }
    i++;
    return node;
}
static LT decode(u32 id) { LT r; r.m = 0; size_t i = 0; string s = unpack(tcode[id]); parseRec(s, i, r); return r; }
static vector<uint8_t> repsOf(const LT& r) {
    buildAdj(r); vector<uint8_t> reps; set<string> seen;
    for (int v = 0; v < r.m; v++) { string s = codeRec(v, -1); if (seen.insert(s).second) reps.push_back(v); }
    return reps;
}
static u32 getId(const LT& t) {
    string c = canon(t);
    if (c.size() > 62) { fprintf(stderr, "tree too large for packing\n"); exit(1); }
    u128 pc = pack(c);
    auto it = codeId.find(pc);
    if (it != codeId.end()) return it->second;
    LT r; r.m = 0; size_t i = 0; parseRec(c, i, r);
    vector<int> od(r.m, 0); for (auto &a : r.arcs) od[a[0]]++;
    buildAdj(r);
    int maxd = 0;
    for (int u = 0; u < r.m; u++) if (od[u] == CAP) {
        vector<int> dist(r.m, -1); deque<int> q; dist[u] = 0; q.push_back(u); int found = -1;
        while (!q.empty() && found < 0) { int x = q.front(); q.pop_front();
            for (auto &e : adjv[x]) if (e.second && dist[e.first] < 0) { dist[e.first] = dist[x] + 1; if (od[e.first] < CAP) { found = dist[e.first]; break; } q.push_back(e.first); } }
        if (found < 0) { fprintf(stderr, "no unsaturated reachable?!\n"); exit(1); }
        maxd = max(maxd, found);
    }
    u32 id = tcode.size(); tcode.push_back(pc); tm_.push_back(r.m); tmaxd.push_back(maxd); codeId[pc] = id;
    return id;
}

static unordered_map<uint64_t, vector<vector<u32>>> optCache; static size_t optStored = 0;
static vector<u32> levelOpts(u32 A, int u, u32 B, int v, int c) {
    uint64_t key = ((uint64_t)A << 40) | ((uint64_t)u << 35) | ((uint64_t)B << 5) | (uint64_t)v;
    if (optStored > OPTLIM) { optCache.clear(); optStored = 0; }
    {
        auto it = optCache.find(key);
        if (it != optCache.end() && (int)it->second.size() > c) return it->second[c];
    }
    LT TA = decode(A), TB = decode(B);
    int mA = TA.m, m = TA.m + TB.m;
    vector<Arc> old;
    for (auto &a : TA.arcs) old.push_back(a);
    for (auto &a : TB.arcs) old.push_back({(uint8_t)(a[0] + mA), (uint8_t)(a[1] + mA)});
    int E = old.size();
    vector<vector<u32>> add;
    int have; { auto it = optCache.find(key); have = it == optCache.end() ? 0 : it->second.size(); }
    for (int cc = have; cc <= c; cc++) {
        vector<u32> res;
        if (cc <= E) {
            vector<int> idx(cc); for (int i = 0; i < cc; i++) idx[i] = i;
            while (true) {
                LT t; t.m = m; t.arcs = old;
                for (int i : idx) swap(t.arcs[i][0], t.arcs[i][1]);
                int od[32] = {0}; bool ok = true;
                for (auto &a : t.arcs) od[a[0]]++;
                for (int x = 0; x < m; x++) if (od[x] > CAP) ok = false;
                if (ok) {
                    int U = u, W = v + mA;
                    if (od[U] < CAP) { LT t2 = t; t2.arcs.push_back({(uint8_t)U, (uint8_t)W}); res.push_back(getId(t2)); }
                    if (od[W] < CAP) { LT t2 = t; t2.arcs.push_back({(uint8_t)W, (uint8_t)U}); res.push_back(getId(t2)); }
                }
                int i = cc - 1;
                while (i >= 0 && idx[i] == E - cc + i) i--;
                if (i < 0) break;
                idx[i]++; for (int j = i + 1; j < cc; j++) idx[j] = idx[j - 1] + 1;
            }
        }
        sort(res.begin(), res.end()); res.erase(unique(res.begin(), res.end()), res.end());
        optStored += res.size() + 4;
        add.push_back(res);
    }
    auto &lv = optCache[key];
    for (auto &r : add) lv.push_back(r);
    return lv[c];
}

struct VH { size_t operator()(const vector<u32>& v) const { uint64_t h = 1469598103934665603ULL; for (u32 x : v) { h ^= x; h *= 1099511628211ULL; h ^= h >> 29; } return h; } };
static unordered_map<vector<u32>, pair<int8_t,int8_t>, VH> memo; // lo <= V <= hi
static bool advAllowed(int a, int b) {
    if (ADV == 0) return true;
    if (ADV == 1) return a == b;
    if (ADV == 2) return a == b || (a + b <= 4);
    if (ADV == 5) return (a == 1 && b == 1) || (a == 2 && b == 2) || (a % 4 == 0 && b % 4 == 0);
    if (ADV == 6) return (a == 1 && b == 1) || (a % 2 == 0 && b % 2 == 0);
    return true;
}
static unsigned long long calls = 0; static int NTOT = 0;
// terminal payoff psi_J(tree) = min over valid reorientations D'' of  dist(D,D'') + min(J, maxd(D''))
static unordered_map<u32,int> psiMemo;
static int psi(u32 id) {
    auto it = psiMemo.find(id); if (it != psiMemo.end()) return it->second;
    int best = min(TERMJ, (int)tmaxd[id]);
    LT T = decode(id); int E = T.arcs.size();
    for (int cc = 1; cc < best && cc <= E; cc++) {
        vector<int> idx(cc); for (int i = 0; i < cc; i++) idx[i] = i;
        while (true) {
            LT t = T; for (int i : idx) swap(t.arcs[i][0], t.arcs[i][1]);
            int od[32] = {0}; bool ok = true; for (auto &a : t.arcs) od[a[0]]++;
            for (int x = 0; x < t.m; x++) if (od[x] > CAP) ok = false;
            if (ok) { u32 id2 = getId(t); best = min(best, cc + min(TERMJ, (int)tmaxd[id2])); }
            int i = cc - 1; while (i >= 0 && idx[i] == E - cc + i) i--;
            if (i < 0) break;
            idx[i]++; for (int j = i + 1; j < cc; j++) idx[j] = idx[j - 1] + 1;
        }
    }
    return psiMemo[id] = best;
}
static bool win(const vector<u32>& st, int b) {
    int k = st.size();
    if (k == 1) return TERMJ ? psi(st[0]) > b : false;
    if (k == 2 && !TERMJ && advAllowed(tm_[st[0]], tm_[st[1]])) return min(tmaxd[st[0]], tmaxd[st[1]]) > b;
    int sp = -1;
    if (ADV == 3) {
        for (int x = 0; x < k; x++) if (st[x] == SEP) sp = x;
        if (k == 3) return min(tmaxd[st[0]], tmaxd[st[2]]) > b;   // one tree on each side: closed form
    }
    {
        auto it = memo.find(st);
        if (it != memo.end()) { if (it->second.first > b) return true; if (it->second.second <= b) return false; }
    }
    calls++;
    set<pair<u32,u32>> seen; bool any = false;
    for (int i = 0; i < k; i++) for (int j = i + 1; j < k; j++) {
        u32 A = st[i], B = st[j];
        if (ADV == 3 && (i == sp || j == sp || ((i < sp) != (j < sp)))) continue;   // only merges inside one side
        if (!seen.insert({A, (ADV == 3 && i > sp) ? (B | 0x80000000u) : B}).second) continue;
        if (!advAllowed(tm_[A], tm_[B])) continue;
        any = true;
        vector<u32> rest; for (int x = 0; x < k; x++) if (x != i && x != j) rest.push_back(st[x]);
        vector<uint8_t> ra = repsOf(decode(A)), rb = repsOf(decode(B)); int E = tm_[A] + tm_[B] - 2;
        for (int u : ra) for (int v : rb) {
            bool replied = false;
            for (int c = 0; c <= E && c <= b && c <= ALGC && !replied; c++) {
                vector<u32> opts = levelOpts(A, u, B, v, c);
                for (u32 id : opts) {
                    if (tmaxd[id] > ALGP) continue;
                    vector<u32> ns = rest;
                    if (ADV == 3) {
                        int q = 0; while (ns[q] != SEP) q++;
                        if (i < sp) ns.insert(lower_bound(ns.begin(), ns.begin() + q, id), id);
                        else ns.insert(lower_bound(ns.begin() + q + 1, ns.end(), id), id);
                        if (2 * SPLIT == NTOT) {   // equal halves: sides are interchangeable
                            int q2 = 0; while (ns[q2] != SEP) q2++;
                            vector<u32> a(ns.begin(), ns.begin() + q2), bb(ns.begin() + q2 + 1, ns.end());
                            if (bb < a) { ns = bb; ns.push_back(SEP); ns.insert(ns.end(), a.begin(), a.end()); }
                        }
                    } else ns.insert(lower_bound(ns.begin(), ns.end(), id), id);
                    if (!win(ns, b - c)) { replied = true; break; }
                }
            }
            if (!replied) {
                auto &e = memo.emplace(st, make_pair((int8_t)0, (int8_t)100)).first->second;
                if (e.first < b + 1) e.first = b + 1;
                return true;
            }
        }
    }
    if (!any) { fprintf(stderr, "restricted Adversary has no move (bad N for ADVMODE)\n"); exit(1); }
    auto &e = memo.emplace(st, make_pair((int8_t)0, (int8_t)100)).first->second;
    if (e.second > b) e.second = b;
    return false;
}

int main(int argc, char** argv) {
    if (argc < 3) { fprintf(stderr, "usage\n"); return 1; }
    int N = atoi(argv[1]); CAP = atoi(argv[2]);
    if (argc > 3) ADV = atoi(argv[3]);
    int b0 = argc > 4 ? atoi(argv[4]) : 0; if (getenv("OPTLIM")) OPTLIM = atoll(getenv("OPTLIM")); if (getenv("ALGP")) ALGP = atoi(getenv("ALGP")); if (getenv("ALGC")) ALGC = atoi(getenv("ALGC")); if (getenv("TERMJ")) TERMJ = atoi(getenv("TERMJ"));
    LT s; s.m = 1; u32 single = getId(s);
    vector<u32> st(N, single); NTOT = N;
    if (getenv("SPLIT")) SPLIT = atoi(getenv("SPLIT"));
    if (ADV == 3) { st.assign(SPLIT, single); st.push_back(SEP); for (int i = 0; i < N - SPLIT; i++) st.push_back(single); }
    if (TERMJ) printf("terminal payoff psi_J with J=%d (W_J game)\n", TERMJ);
    for (int b = b0;; b++) {
        bool w = win(st, b);
        printf("adv=%d cap=%d n=%d : value > %d ? %s   (states=%zu trees=%zu calls=%llu)\n", ADV, CAP, N, b, w ? "YES" : "NO", memo.size(), tcode.size(), calls);
        fflush(stdout);
        if (getenv("DUMPLO")) {   // certificate of the lower bounds proved so far, then stop
            FILE* f = fopen(getenv("DUMPLO"), "w");
            fprintf(f, "# cap=%d n=%d adv=%d termj=%d query: value > %d : %s\n# each line: LB | components   (claim: value(state) >= LB; in adv=3 files 'SEP' separates the two sides)\n", CAP, N, ADV, TERMJ, b, w ? "YES" : "NO");
            vector<string> lines;
            for (auto &p : memo) if (p.second.first > 0) { string l = to_string((int)p.second.first) + " |"; for (u32 id : p.first) l += id == SEP ? string(" SEP") : " " + unpack(tcode[id]); lines.push_back(l); }
            sort(lines.begin(), lines.end());
            for (auto &l : lines) fprintf(f, "%s\n", l.c_str());
            fclose(f); return 0;
        }
        if (!w) { printf("RESULT adv=%d cap=%d n=%d T%s=%d%s\n", ADV, CAP, N, ADV ? "_restricted" : "", b, b == b0 && b0 > 0 ? " (upper bound only: started at BSTART)" : ""); break; }
    }
    return 0;
}
