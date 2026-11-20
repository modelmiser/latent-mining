// sat.cpp: SAT encoding of "a top-scheme on n cells survives t operations" + a small CDCL solver.
// usage: sat n t [rootempty=0] [sym=1] [dimacs_out_file|-] [unused] [prune=0]
//
// ENCODING (exact, see proofs.txt section 2):
//  type T = (s, d): d = operations done, s = last min(len, t-d+1) bits of the stack (deeper bits
//  can never be seen within the remaining t-d operations).
//  variables  L[T][c]      "some history of type T may sit in configuration c"
//             e[c][op][c'] "E(c,op) may be c'"  (c' superset of c)
//             qv[c][k]     "Q(c) = k", k in {0,1,EMPTY}
//  clauses    L[(empty,0)][0^n]
//             -L[T][c] v -e[c][op][c'] v L[T.op][c']         (d < t, op legal for s)
//             -L[T][c] v OR_{c'} e[c][op][c']                (d < t, op legal for s)
//             -L[T][c] v qv[c][top(s)]                       (d >= 1; also d = 0 if rootempty)
//             at most one of qv[c][0..2]
//  sym=1 adds symmetry breaking under cell permutations: E(0^n,PUSH0) is a prefix set 1^k0^(n-k),
//  and E(0^n,PUSH1) is (a prefix of those k cells) union (a prefix of the other n-k cells).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

// ---------------- minimal CDCL ----------------
struct Clause { int size; bool learnt; float act; int lits[1]; };
struct Watch { Clause *c; int blocker; };
struct Solver {
    int nv = 0;
    vector<signed char> val;       // per literal: 1 true, -1 false, 0 undef
    vector<int> level, trail, trail_lim;
    vector<Clause *> reason;
    vector<vector<Watch>> watches;
    vector<Clause *> clauses, learnts;
    vector<double> activity; double var_inc = 1.0; float cla_inc = 1.0;
    vector<int> heap, hpos;         // max-heap on activity
    vector<signed char> phase;
    vector<char> seenv;
    size_t qhead = 0;
    vector<char> dbgmodel;
    bool ok = true;
    long long conflicts = 0, decisions = 0, propagations = 0;
    unsigned long long rng = 88172645463325252ULL;

    int newVar() {
        int v = nv++;
        val.push_back(0); val.push_back(0); level.push_back(0); reason.push_back(nullptr);
        watches.emplace_back(); watches.emplace_back(); activity.push_back(0); hpos.push_back(-1);
        phase.push_back(0); seenv.push_back(0); heapInsert(v);
        return v;
    }
    bool hless(int a, int b) { return activity[a] > activity[b]; }
    void heapUp(int i) { int x = heap[i]; while (i > 0) { int p = (i - 1) / 2; if (!hless(x, heap[p])) break; heap[i] = heap[p]; hpos[heap[i]] = i; i = p; } heap[i] = x; hpos[x] = i; }
    void heapDown(int i) { int x = heap[i]; int n = heap.size(); for (;;) { int c = 2 * i + 1; if (c >= n) break; if (c + 1 < n && hless(heap[c + 1], heap[c])) c++; if (!hless(heap[c], x)) break; heap[i] = heap[c]; hpos[heap[i]] = i; i = c; } heap[i] = x; hpos[x] = i; }
    void heapInsert(int v) { if (hpos[v] >= 0) return; heap.push_back(v); hpos[v] = heap.size() - 1; heapUp(hpos[v]); }
    int heapPop() { int x = heap[0]; hpos[x] = -1; int last = heap.back(); heap.pop_back(); if (!heap.empty()) { heap[0] = last; hpos[last] = 0; heapDown(0); } return x; }
    void bumpVar(int v) { if ((activity[v] += var_inc) > 1e100) { for (auto &a : activity) a *= 1e-100; var_inc *= 1e-100; } if (hpos[v] >= 0) heapUp(hpos[v]); }

    int dl() { return trail_lim.size(); }
    void enqueue(int lit, Clause *r) { val[lit] = 1; val[lit ^ 1] = -1; level[lit >> 1] = dl(); reason[lit >> 1] = r; trail.push_back(lit); }
    void attach(Clause *c) { watches[c->lits[0] ^ 1].push_back({c, c->lits[1]}); watches[c->lits[1] ^ 1].push_back({c, c->lits[0]}); }
    Clause *mk(const vector<int> &ls, bool learnt) {
        Clause *c = (Clause *)malloc(sizeof(Clause) + sizeof(int) * (ls.size() - 1));
        c->size = ls.size(); c->learnt = learnt; c->act = 0;
        for (size_t i = 0; i < ls.size(); i++) c->lits[i] = ls[i];
        return c;
    }
    // add an original clause (at level 0)
    bool addClause(vector<int> ls) {
        if (!ok) return false;
        sort(ls.begin(), ls.end());
        vector<int> out;
        for (size_t i = 0; i < ls.size(); i++) {
            if (i && ls[i] == ls[i - 1]) continue;
            if (i && ls[i] == (ls[i - 1] ^ 1)) return true; // tautology
            if (val[ls[i]] == 1) return true;
            if (val[ls[i]] == -1) continue;
            out.push_back(ls[i]);
        }
        if (out.empty()) return ok = false;
        if (out.size() == 1) { enqueue(out[0], nullptr); return ok = (propagate() == nullptr); }
        Clause *c = mk(out, false); clauses.push_back(c); attach(c);
        return true;
    }
    Clause *propagate() {
        Clause *confl = nullptr;
        while (qhead < trail.size()) {
            int p = trail[qhead++]; // p is true; visit clauses watching ~p ... stored under index p
            propagations++;
            vector<Watch> &ws = watches[p];
            size_t i = 0, j = 0;
            while (i < ws.size()) {
                Watch w = ws[i++];
                if (val[w.blocker] == 1) { ws[j++] = w; continue; }
                Clause *c = w.c;
                int falselit = p ^ 1;
                if (c->lits[0] == falselit) { c->lits[0] = c->lits[1]; c->lits[1] = falselit; }
                int first = c->lits[0];
                if (val[first] == 1) { ws[j++] = {c, first}; continue; }
                bool found = false;
                for (int k = 2; k < c->size; k++) if (val[c->lits[k]] != -1) {
                    c->lits[1] = c->lits[k]; c->lits[k] = falselit;
                    watches[c->lits[1] ^ 1].push_back({c, first}); found = true; break;
                }
                if (found) continue;
                ws[j++] = {c, first};
                if (val[first] == -1) { confl = c; qhead = trail.size(); while (i < ws.size()) ws[j++] = ws[i++]; }
                else enqueue(first, c);
            }
            ws.resize(j);
        }
        return confl;
    }
    void cancelUntil(int lvl) {
        if (dl() <= lvl) return;
        for (int i = (int)trail.size() - 1; i >= trail_lim[lvl]; i--) {
            int l = trail[i]; int v = l >> 1; val[l] = val[l ^ 1] = 0; phase[v] = !(l & 1); reason[v] = nullptr; heapInsert(v);
        }
        trail.resize(trail_lim[lvl]); trail_lim.resize(lvl); qhead = trail.size();
    }
    void analyze(Clause *confl, vector<int> &out, int &btlevel) {
        out.clear(); out.push_back(-1);
        int pathC = 0, p = -1; int idx = trail.size() - 1;
        vector<int> toclear;
        do {
            if (confl->learnt) { if ((confl->act += cla_inc) > 1e20f) { for (auto c : learnts) c->act *= 1e-20f; cla_inc *= 1e-20f; } }
            for (int k = (p == -1 ? 0 : 1); k < confl->size; k++) {
                int q = confl->lits[k]; int v = q >> 1;
                if (val[q] != -1) { fprintf(stderr, "NONFALSE lit in %s clause size=%d k=%d val=%d\n", p == -1 ? "conflict" : "reason", confl->size, k, val[q]); exit(5); }
                if (!seenv[v] && level[v] > 0) {
                    seenv[v] = 1; toclear.push_back(v); bumpVar(v);
                    if (level[v] >= dl()) pathC++; else out.push_back(q);
                }
            }
            while (!seenv[trail[idx] >> 1]) idx--;
            p = trail[idx--]; confl = reason[p >> 1]; seenv[p >> 1] = 0; pathC--;
            if (confl && confl->lits[0] != p) { fprintf(stderr, "REASON ORDER BROKEN size=%d learnt=%d pos1=%d\n", confl->size, confl->learnt, confl->lits[1] == p); exit(7); }
            if (!confl && pathC > 0) { fprintf(stderr, "DECISION with pathC>0\n"); exit(6); }
        } while (pathC > 0);
        out[0] = p ^ 1;
        // simple minimisation: drop a literal whose reason's other literals are all already in the clause
        for (int v : toclear) seenv[v] = 0;
        vector<int> marked;
        for (size_t i = 1; i < out.size(); i++) { seenv[out[i] >> 1] = 1; marked.push_back(out[i] >> 1); }
        size_t j = 1;
        for (size_t i = 1; i < out.size(); i++) {
            Clause *r = reason[out[i] >> 1]; bool keep = true;
            if (r) { keep = false; for (int k = 0; k < r->size; k++) { int q = r->lits[k]; if ((q >> 1) == (out[i] >> 1)) continue; if (!seenv[q >> 1] && level[q >> 1] > 0) { keep = true; break; } } }
            if (keep) out[j++] = out[i];
        }
        for (int v : marked) seenv[v] = 0; // (bug fixed: out[] is compacted in place above, so clear from a copy)
        out.resize(j);
        btlevel = 0;
        if (out.size() > 1) { size_t m = 1; for (size_t i = 2; i < out.size(); i++) if (level[out[i] >> 1] > level[out[m] >> 1]) m = i; swap(out[1], out[m]); btlevel = level[out[1] >> 1]; }
    }
    void reduceDB() {
        // called at decision level 0
        sort(learnts.begin(), learnts.end(), [](Clause *a, Clause *b) { return a->act < b->act; });
        size_t half = learnts.size() / 2, j = 0;
        vector<Clause *> dead;
        for (size_t i = 0; i < learnts.size(); i++) {
            Clause *c = learnts[i];
            if (i < half && c->size > 2) dead.push_back(c); else learnts[j++] = c;
        }
        learnts.resize(j);
        for (auto &ws : watches) ws.clear();
        for (auto c : clauses) attach(c);
        for (auto c : learnts) attach(c);
        for (auto c : dead) free(c);
    }
    static double luby(double y, int x) { int size, seq; for (size = 1, seq = 0; size < x + 1; seq++, size = 2 * size + 1); while (size - 1 != x) { size = (size - 1) >> 1; seq--; x = x % size; } double r = 1; while (seq-- > 0) r *= y; return r; }
    // returns 1 SAT, 0 UNSAT
    int solve() {
        if (!ok) return 0;
        if (propagate()) return 0;
        int restarts = 0; long long nextReduce = 20000; vector<int> learnt;
        for (;;) {
            long long budget = (long long)(luby(2, restarts++) * 200), c0 = conflicts;
            for (;;) {
                Clause *confl = propagate();
                if (confl) {
                    conflicts++;
                    if (dl() == 0) return 0;
                    int bt; analyze(confl, learnt, bt);
                    if (!dbgmodel.empty()) { bool sat = false; for (int l : learnt) if (dbgmodel[l >> 1] == !(l & 1)) sat = true;
                        if (!sat) { fprintf(stderr, "BAD LEARNT at conflict %lld size %zu dl %d bt %d\n", conflicts, learnt.size(), dl(), bt);
                            bool cs = false; for (int k = 0; k < confl->size; k++) if (dbgmodel[confl->lits[k] >> 1] == !(confl->lits[k] & 1)) cs = true;
                            fprintf(stderr, "conflict clause learnt=%d size=%d satisfied_by_model=%d\n", confl->learnt, confl->size, cs); exit(9); } }
                    cancelUntil(bt);
                    if (learnt.size() == 1) enqueue(learnt[0], nullptr);
                    else { Clause *c = mk(learnt, true); c->act = cla_inc; learnts.push_back(c); attach(c); enqueue(learnt[0], c); }
                    var_inc /= 0.95; cla_inc /= 0.999f;
                    if ((conflicts & 0xfffff) == 0) { fprintf(stderr, "  conflicts=%lld learnts=%zu\n", conflicts, learnts.size()); }
                } else {
                    if (conflicts - c0 >= budget) { cancelUntil(0); break; }
                    int next = -1;
                    while (!heap.empty()) { int v = heapPop(); if (val[2 * v] == 0) { next = v; break; } }
                    if (next < 0) return 1;
                    decisions++;
                    trail_lim.push_back(trail.size());
                    enqueue(2 * next + (phase[next] ? 0 : 1), nullptr);
                }
            }
            if (conflicts >= nextReduce) { // level 0 here; all reasons at level 0 are never dereferenced by analyze
                for (int l : trail) reason[l >> 1] = nullptr;
                reduceDB(); nextReduce = conflicts + 20000 + 500 * restarts;
            }
        }
    }
};

// ---------------- encoding ----------------
int n, t;
static inline int trunc_(int code, int k) { int len = 31 - __builtin_clz(code); if (len <= k) return code; return (1 << k) | (code & ((1 << k) - 1)); }
int main(int argc, char **argv) {
    n = atoi(argv[1]); t = atoi(argv[2]);
    int rootempty = argc > 3 ? atoi(argv[3]) : 0;
    int sym = argc > 4 ? atoi(argv[4]) : 1;
    const char *dim = argc > 5 ? argv[5] : "-";
    int N = 1 << n;
    Solver S;
    vector<vector<int>> cnf; // copy for DIMACS
    bool wantDimacs = strcmp(dim, "-") != 0;
    auto add = [&](const vector<int> &c) { if (wantDimacs) cnf.push_back(c); S.addClause(c); };
    // types
    map<pair<int, int>, int> tid; vector<pair<int, int>> types;
    types.push_back({1, 0}); tid[{1, 0}] = 0;
    for (size_t i = 0; i < types.size(); i++) {
        int s = types[i].first, d = types[i].second;
        if (d == t) continue;
        for (int op = 0; op < 3; op++) {
            if (op == 2 && s == 1) continue;
            int s2 = op == 2 ? s >> 1 : (s << 1) | op;
            s2 = trunc_(s2, t - (d + 1) + 1);
            if (!tid.count({s2, d + 1})) { tid[{s2, d + 1}] = types.size(); types.push_back({s2, d + 1}); }
        }
    }
    int NT = types.size();
    // variables
    vector<int> Lv(NT * N); for (auto &v : Lv) v = S.newVar();
    vector<int> qv(N * 3); for (auto &v : qv) v = S.newVar();
    vector<map<int, int>> ev(N * 3);
    for (int c = 0; c < N; c++) for (int op = 0; op < 3; op++) {
        int fr = (N - 1) & ~c;
        for (int a = fr;; a = (a - 1) & fr) { ev[c * 3 + op][c | a] = S.newVar(); if (a == 0) break; }
    }
    auto P = [](int v) { return 2 * v; }; auto Ng = [](int v) { return 2 * v + 1; };
    add({P(Lv[0 * N + 0])});
    int prune = argc > 7 ? atoi(argv[7]) : 0;
    if (prune) {
        // Redundant unit clauses (proofs.txt sections 1 and 4): a history of type (s,d), d >= 1, in config c needs
        //  (i) n - |c| >= t - d   (alternating pushes set one new cell per remaining step), and
        //  (ii) |c| >= lo(s,d) = least number of forced configuration changes on any way to reach the type.
        vector<int> lo(NT, 1000); lo[0] = 0;
        for (int T = 0; T < NT; T++) { // types are in BFS (depth) order
            int s = types[T].first, d = types[T].second; if (d == t) continue;
            int r = t - d, len = 31 - __builtin_clz(s);
            for (int op = 0; op < 3; op++) {
                if (op == 2 && s == 1) continue;
                int s2 = op == 2 ? s >> 1 : (s << 1) | op; s2 = trunc_(s2, t - (d + 1) + 1);
                bool stay;
                if (d == 0) stay = true; // root: unchecked, be conservative
                else if (op < 2) { stay = len >= r; for (int k = 0; stay && k < r; k++) if (((s >> k) & 1) != op) stay = false; }
                else { stay = len >= r + 1; for (int k = 1; stay && k <= r; k++) if (((s >> k) & 1) != (s & 1)) stay = false; }
                int T2 = tid[{s2, d + 1}];
                lo[T2] = min(lo[T2], lo[T] + (stay ? 0 : 1));
            }
        }
        for (int T = 1; T < NT; T++) { int d = types[T].second;
            for (int c = 0; c < N; c++) { int w = __builtin_popcount(c); if (n - w < t - d || w < lo[T]) add({Ng(Lv[T * N + c])}); } }
    }
    for (int c = 0; c < N; c++) { add({Ng(qv[c * 3]), Ng(qv[c * 3 + 1])}); add({Ng(qv[c * 3]), Ng(qv[c * 3 + 2])}); add({Ng(qv[c * 3 + 1]), Ng(qv[c * 3 + 2])}); }
    for (int T = 0; T < NT; T++) {
        int s = types[T].first, d = types[T].second;
        int top = s == 1 ? 2 : (s & 1);
        if (d >= 1 || rootempty) for (int c = 0; c < N; c++) add({Ng(Lv[T * N + c]), P(qv[c * 3 + top])});
        if (d == t) continue;
        for (int op = 0; op < 3; op++) {
            if (op == 2 && s == 1) continue;
            int s2 = op == 2 ? s >> 1 : (s << 1) | op;
            s2 = trunc_(s2, t - (d + 1) + 1);
            int T2 = tid[{s2, d + 1}];
            for (int c = 0; c < N; c++) {
                vector<int> tot = {Ng(Lv[T * N + c])};
                for (auto &kv : ev[c * 3 + op]) {
                    add({Ng(Lv[T * N + c]), Ng(kv.second), P(Lv[T2 * N + kv.first])});
                    tot.push_back(P(kv.second));
                }
                add(tot);
            }
        }
    }
    if (sym) {
        for (auto &kv : ev[0 * 3 + 0]) { int c2 = kv.first; if (c2 & (c2 + 1)) add({Ng(kv.second)}); }
        for (int k = 0; k <= n; k++) {
            int Pm = (1 << k) - 1;
            for (auto &kv : ev[0 * 3 + 1]) {
                int c2 = kv.first; int a = c2 & Pm, b = (c2 & ~Pm) >> k;
                if ((a & (a + 1)) || (b & (b + 1))) add({Ng(ev[0][Pm]), Ng(kv.second)});
            }
        }
    }
    if (wantDimacs) {
        FILE *f = fopen(dim, "w");
        fprintf(f, "c top-scheme n=%d t=%d rootempty=%d sym=%d ; encoding described in sat.cpp header\n", n, t, rootempty, sym);
        fprintf(f, "p cnf %d %zu\n", S.nv, cnf.size());
        for (auto &c : cnf) { for (int l : c) fprintf(f, "%d ", (l & 1) ? -((l >> 1) + 1) : ((l >> 1) + 1)); fprintf(f, "0\n"); }
        fclose(f);
    }
    if (getenv("CHECKMODEL")) { FILE *f = fopen(getenv("CHECKMODEL"), "r"); S.dbgmodel.assign(S.nv, 0); int v; while (fscanf(f, "%d", &v) == 1) S.dbgmodel[v] = 1; fclose(f);
        for (auto c : S.clauses) { bool sat = false; for (int k = 0; k < c->size; k++) if (S.dbgmodel[c->lits[k] >> 1] == !(c->lits[k] & 1)) sat = true; if (!sat) { fprintf(stderr, "model violates original clause\n"); exit(8); } } }
    int r = S.solve();
    if (r && getenv("WRITEMODEL")) { FILE *f = fopen(getenv("WRITEMODEL"), "w"); for (int v = 0; v < S.nv; v++) if (S.val[2 * v] == 1) fprintf(f, "%d\n", v); fclose(f); }
    printf("n=%d t=%d rootempty=%d sym=%d prune=%d types=%d vars=%d clauses=%zu : %s  conflicts=%lld decisions=%lld\n", n, t, rootempty, sym, prune, NT, S.nv, S.clauses.size(), r ? "SURVIVES" : "IMPOSSIBLE", S.conflicts, S.decisions);
    fflush(stdout);
    if (r) {
        char fn[80]; sprintf(fn, "witness_n%d_t%d%s.txt", n, t, rootempty ? "_rootempty" : "");
        FILE *f = fopen(fn, "w");
        fprintf(f, "# top-scheme n=%d surviving t=%d%s. Config = n-char binary string, cell 1 first.\n", n, t, rootempty ? " with Q(0^n)=EMPTY" : "");
        fprintf(f, "# line: config Q E(PUSH0) E(PUSH1) E(POP); unlisted configs: Q=EMPTY, all E = stay\n");
        fprintf(f, "n %d t %d\n", n, t);
        const char *qs[] = {"0", "1", "EMPTY"};
        for (int c = 0; c < N; c++) {
            bool reach = false; for (int T = 0; T < NT; T++) if (S.val[P(Lv[T * N + c])] == 1) reach = true;
            if (!reach) continue;
            int qq = 2; for (int k = 0; k < 3; k++) if (S.val[P(qv[c * 3 + k])] == 1) qq = k;
            auto pr = [&](int x) { for (int b = 0; b < n; b++) fputc('0' + ((x >> b) & 1), f); };
            pr(c); fprintf(f, " %s", qs[qq]);
            for (int op = 0; op < 3; op++) {
                int tgt = c; for (auto &kv : ev[c * 3 + op]) if (S.val[P(kv.second)] == 1) { tgt = kv.first; break; }
                fputc(' ', f); pr(tgt);
            }
            fputc('\n', f);
        }
        fclose(f);
    }
    return r ? 0 : 1;
}
