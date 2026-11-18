// Exact solver for the forest-orientation game.
// usage: fast MODE N CAP [ADVMODE] [DUMPFILE]
//   MODE = full : Algorithm may re-orient the whole forest, every valid orientation is tried,
//                 no pruning, no closed forms (reference; states = oriented forests up to isomorphism)
//   MODE = fast : uses (1) laziness in untouched components, (2) cost-ordered branch and bound,
//                 (3) alpha cut on the Adversary side, (4) closed form for 2 components.
//   ADVMODE (fast only): 0 = unrestricted Adversary (exact value)
//                        1 = Adversary may only merge two components of EQUAL size (lower bound)
//                        2 = Adversary may only merge components whose sizes are both <=4 or equal (lower bound)
#include <bits/stdc++.h>
using namespace std;
typedef uint32_t u32;
typedef array<uint8_t,2> Arc; // tail, head

static int CAP = 2;
static int ADV = 0;

struct LT { int m; vector<Arc> arcs; };
struct Tree { int m; vector<Arc> arcs; vector<uint8_t> od; vector<uint8_t> reps; int maxd; string code; };
static vector<Tree> trees;
static unordered_map<string,u32> codeId;

static vector<pair<int,int>> adjv[32]; // (nbr, dir) dir=1 if v->nbr
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
static int parseRec(const string& s, size_t& i, LT& t) {
    int node = t.m++;
    i++; // '('
    while (s[i] != ')') {
        char d = s[i++];
        int ch = parseRec(s, i, t);
        if (d == 'a') t.arcs.push_back({(uint8_t)node, (uint8_t)ch}); else t.arcs.push_back({(uint8_t)ch, (uint8_t)node});
    }
    i++;
    return node;
}
static u32 getId(const LT& t) {
    string c = canon(t);
    auto it = codeId.find(c);
    if (it != codeId.end()) return it->second;
    Tree T; T.code = c;
    LT r; r.m = 0; size_t i = 0; parseRec(c, i, r);
    T.m = r.m; T.arcs = r.arcs; T.od.assign(T.m, 0);
    for (auto &a : T.arcs) T.od[a[0]]++;
    // orbit representatives: rooted code at each vertex
    buildAdj(r);
    { set<string> seen; for (int v = 0; v < T.m; v++) { string s = codeRec(v, -1); if (seen.insert(s).second) T.reps.push_back(v); } }
    // maxd: max over saturated u of directed distance to nearest unsaturated vertex
    T.maxd = 0;
    for (int u = 0; u < T.m; u++) if (T.od[u] == CAP) {
        vector<int> dist(T.m, -1); deque<int> q; dist[u] = 0; q.push_back(u); int found = -1;
        while (!q.empty() && found < 0) { int x = q.front(); q.pop_front();
            for (auto &e : adjv[x]) if (e.second && dist[e.first] < 0) { dist[e.first] = dist[x] + 1; if (T.od[e.first] < CAP) { found = dist[e.first]; break; } q.push_back(e.first); } }
        if (found < 0) { fprintf(stderr, "no unsaturated reachable?!\n"); exit(1); }
        T.maxd = max(T.maxd, found);
    }
    u32 id = trees.size(); trees.push_back(T); codeId[c] = id;
    return id;
}

// ---------- merge options by cost level (fast mode) ----------
static unordered_map<uint64_t, vector<vector<u32>>> optCache;
static const vector<u32>& levelOpts(u32 A, int u, u32 B, int v, int c) {
    uint64_t key = ((uint64_t)A << 40) | ((uint64_t)u << 35) | ((uint64_t)B << 5) | (uint64_t)v;
    auto &lv = optCache[key];
    while ((int)lv.size() <= c) {
        int cc = lv.size();
        const Tree TA = trees[A], TB = trees[B]; // copies: getId may reallocate trees
        int mA = TA.m, m = TA.m + TB.m;
        vector<Arc> old;
        for (auto &a : TA.arcs) old.push_back(a);
        for (auto &a : TB.arcs) old.push_back({(uint8_t)(a[0] + mA), (uint8_t)(a[1] + mA)});
        int E = old.size();
        vector<u32> res;
        if (cc <= E) {
            vector<int> idx(cc); for (int i = 0; i < cc; i++) idx[i] = i;
            while (true) {
                LT t; t.m = m; t.arcs = old;
                for (int i : idx) swap(t.arcs[i][0], t.arcs[i][1]);
                int od[32] = {0}; bool ok = true;
                for (auto &a : t.arcs) od[a[0]]++;
                for (int x = 0; x < m; x++) if (od[x] > CAP) { if (!(od[x] == CAP + 0 )) { if (od[x] > CAP) ok = false; } }
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
        optCache[key].push_back(res);
    }
    return optCache[key][c];
}

struct VH { size_t operator()(const vector<u32>& v) const { uint64_t h = 1469598103934665603ULL; for (u32 x : v) { h ^= x; h *= 1099511628211ULL; h ^= h >> 29; } return h; } };
static unordered_map<vector<u32>, int8_t, VH> memo;
static bool advAllowed(int a, int b) {
    if (ADV == 0) return true;
    if (ADV == 1) return a == b;
    if (ADV == 2) return a == b || (a + b <= 4);
    return true;
}
static int solveFast(const vector<u32>& st) {
    int k = st.size();
    if (k == 1) return 0;
    if (k == 2) return min(trees[st[0]].maxd, trees[st[1]].maxd);
    auto it = memo.find(st);
    if (it != memo.end()) return it->second;
    int best = 0; bool any = false;
    set<pair<u32,u32>> seen;
    for (int i = 0; i < k; i++) for (int j = i + 1; j < k; j++) {
        u32 A = st[i], B = st[j];
        if (!seen.insert({A, B}).second) continue;
        if (!advAllowed(trees[A].m, trees[B].m)) continue;
        any = true;
        vector<u32> rest; for (int x = 0; x < k; x++) if (x != i && x != j) rest.push_back(st[x]);
        vector<uint8_t> ra = trees[A].reps, rb = trees[B].reps; int E = trees[A].m + trees[B].m - 2;
        for (int u : ra) for (int v : rb) {
            int val = 100;
            for (int c = 0; c <= E && c < val && val > best; c++) {
                vector<u32> opts = levelOpts(A, u, B, v, c);
                for (u32 id : opts) {
                    vector<u32> ns = rest; ns.insert(lower_bound(ns.begin(), ns.end(), id), id);
                    int t = c + solveFast(ns);
                    if (t < val) { val = t; if (val <= best || val <= c) break; }
                }
            }
            if (val == 100) { fprintf(stderr, "no option?!\n"); exit(1); }
            if (val > best) best = val;
        }
    }
    if (!any) { fprintf(stderr, "restricted Adversary has no move (bad N for ADVMODE)\n"); exit(1); }
    memo[st] = best;
    return best;
}

// ---------- full reference mode ----------
static unordered_map<u32, vector<pair<int,u32>>> reorCache; // all valid reorientations of a tree: (cost, id)
static const vector<pair<int,u32>>& allReor(u32 A) {
    auto it = reorCache.find(A);
    if (it != reorCache.end()) return it->second;
    const Tree T = trees[A]; int E = T.arcs.size();
    map<u32,int> bestc;
    for (u32 mask = 0; mask < (1u << E); mask++) {
        LT t; t.m = T.m; t.arcs = T.arcs; int od[32] = {0};
        for (int i = 0; i < E; i++) { if (mask >> i & 1) swap(t.arcs[i][0], t.arcs[i][1]); od[t.arcs[i][0]]++; }
        bool ok = true; for (int x = 0; x < T.m; x++) if (od[x] > CAP) ok = false;
        if (!ok) continue;
        u32 id = getId(t); int c = __builtin_popcount(mask);
        auto f = bestc.find(id); if (f == bestc.end() || f->second > c) bestc[id] = c;
    }
    vector<pair<int,u32>> r; for (auto &p : bestc) r.push_back({p.second, p.first});
    return reorCache[A] = r;
}
static unordered_map<vector<u32>, int8_t, VH> memoF;
static int solveFull(const vector<u32>& st);
static void prodRec(const vector<u32>& rest, size_t i, int cost, vector<u32>& cur, int& val) {
    if (i == rest.size()) { vector<u32> ns = cur; sort(ns.begin(), ns.end()); int t = cost + solveFull(ns); if (t < val) val = t; return; }
    vector<pair<int,u32>> opts = allReor(rest[i]);
    for (auto &o : opts) { cur.push_back(o.second); prodRec(rest, i + 1, cost + o.first, cur, val); cur.pop_back(); }
}
static int solveFull(const vector<u32>& st) {
    int k = st.size();
    if (k == 1) return 0;
    auto it = memoF.find(st);
    if (it != memoF.end()) return it->second;
    int best = 0;
    for (int i = 0; i < k; i++) for (int j = i + 1; j < k; j++) {
        u32 A = st[i], B = st[j];
        vector<u32> rest; for (int x = 0; x < k; x++) if (x != i && x != j) rest.push_back(st[x]);
        const Tree TA = trees[A], TB = trees[B]; int mA = TA.m, m = TA.m + TB.m;
        for (int u = 0; u < TA.m; u++) for (int v = 0; v < TB.m; v++) {   // all vertices, no orbit reduction
            vector<Arc> all;
            for (auto &a : TA.arcs) all.push_back(a);
            for (auto &a : TB.arcs) all.push_back({(uint8_t)(a[0] + mA), (uint8_t)(a[1] + mA)});
            int E = all.size();
            all.push_back({(uint8_t)u, (uint8_t)(v + mA)});
            int val = 100;
            for (u32 mask = 0; mask < (1u << (E + 1)); mask++) {
                LT t; t.m = m; t.arcs = all; int od[32] = {0};
                for (int e = 0; e <= E; e++) { if (mask >> e & 1) swap(t.arcs[e][0], t.arcs[e][1]); od[t.arcs[e][0]]++; }
                bool ok = true; for (int x = 0; x < m; x++) if (od[x] > CAP) ok = false;
                if (!ok) continue;
                int cost = __builtin_popcount(mask & ((1u << E) - 1));
                vector<u32> cur; cur.push_back(getId(t));
                prodRec(rest, 0, cost, cur, val);
            }
            if (val > best) best = val;
        }
    }
    memoF[st] = best;
    return best;
}

int main(int argc, char** argv) {
    if (argc < 4) { fprintf(stderr, "usage\n"); return 1; }
    string mode = argv[1]; int N = atoi(argv[2]); CAP = atoi(argv[3]);
    if (argc > 4) ADV = atoi(argv[4]);
    LT s; s.m = 1; u32 single = getId(s);
    vector<u32> st(N, single);
    int r = (mode == "full") ? solveFull(st) : solveFast(st);
    size_t ns = (mode == "full") ? memoF.size() : memo.size();
    printf("mode=%s adv=%d cap=%d n=%d T=%d states=%zu trees=%zu\n", mode.c_str(), ADV, CAP, N, r, ns, trees.size());
    if (argc > 5) {
        FILE* f = fopen(argv[5], "w");
        fprintf(f, "# cap=%d n=%d mode=%s adv=%d ; each line: value | canonical codes of the components\n", CAP, N, mode.c_str(), ADV);
        fprintf(f, "# code: '(' children ')' ; before each child 'a' = arc parent->child, 'b' = arc child->parent\n");
        auto &M = (mode == "full") ? memoF : memo;
        vector<string> lines;
        for (auto &p : M) { string l = to_string((int)p.second) + " |"; for (u32 id : p.first) l += " " + trees[id].code; lines.push_back(l); }
        sort(lines.begin(), lines.end());
        for (auto &l : lines) fprintf(f, "%s\n", l.c_str());
        fclose(f);
    }
    return 0;
}
