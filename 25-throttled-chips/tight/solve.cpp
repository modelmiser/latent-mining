// Exact solver for ONE list, written from the definition in chain/3-question.txt.
// Firer knows the whole list, so "the list wins" <=> the set of positions Firer can have reached
// with every height <= h-1 after every step becomes empty after some add.
// Modes:
//   solve C hmin hmax        : for each h, least m such that m zeros win (=C(h)), by growing m
//   solve L h a1 a2 ... am   : does this list win at h?  prints WINS (round) or LOSES + a surviving line
//   solve S h m w seed iters : random + hill-climb search for a winning list of length m on w vertices
// Board: cells 0..W-1 (W = 32), 4 bits per cell in an unsigned __int128; list entries are shifted
// to the middle; the program aborts if a chip ever reaches cell 0 or W-1 (so no chip is lost).
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 u128;
static const int W = 32;
static inline int get(u128 s, int i) { return (int)((s >> (4 * i)) & 15); }
static inline u128 one(int i) { return ((u128)1) << (4 * i); }
struct H128 { size_t operator()(const u128& x) const {
    uint64_t a = (uint64_t)x, b = (uint64_t)(x >> 64);
    a ^= b + 0x9e3779b97f4a7c15ULL + (a << 6) + (a >> 2); a *= 0xff51afd7ed558ccdULL; a ^= a >> 33; return a; } };
typedef unordered_set<u128, H128> Set;

// returns 0 if the list loses, else the round at whose add every line is dead.
// if line != null and the list loses, stores one surviving line (fired cell or -1 per round)
static size_t maxset;
int play(int h, const vector<int>& lst, vector<int>* line = nullptr) {
    if (h > 15) { fprintf(stderr, "h too large\n"); exit(2); }
    int m = lst.size();
    vector<unordered_map<u128, pair<u128, int>, H128>> par;   // only when a line is wanted
    Set cur; cur.insert(0);
    vector<u128> lastStates;
    for (int t = 1; t <= m; t++) {
        int a = lst[t - 1];
        Set nxt; unordered_map<u128, pair<u128, int>, H128> pm;
        for (u128 s : cur) {
            if (get(s, a) + 1 >= h) continue;            // height h right after the add
            u128 s1 = s + one(a);
            if (!nxt.count(s1)) { nxt.insert(s1); if (line) pm[s1] = {s, -1}; }
            for (int v = 1; v < W - 1; v++) {
                int e = get(s1, v);
                if (e < 2) continue;
                if (get(s1, v - 1) + 1 >= h || get(s1, v + 1) + 1 >= h) continue;
                if (v - 1 == 0 || v + 1 == W - 1) { fprintf(stderr, "chip reached the edge\n"); exit(3); }
                u128 s2 = s1 - 2 * one(v) + one(v - 1) + one(v + 1);
                if (!nxt.count(s2)) { nxt.insert(s2); if (line) pm[s2] = {s, v}; }
            }
        }
        if (nxt.empty()) return t;
        maxset = max(maxset, nxt.size());
        if (line) par.push_back(std::move(pm));
        cur.swap(nxt);
    }
    if (line) {
        line->assign(m, -1); u128 s = *cur.begin();
        for (int t = m; t >= 1; t--) { auto pr = par[t - 1][s]; (*line)[t - 1] = pr.second; s = pr.first; }
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) return 1;
    string mode = argv[1];
    if (mode == "C") {
        int h0 = atoi(argv[2]), h1 = atoi(argv[3]);
        for (int h = h0; h <= h1; h++) {
            // the round at which the infinite zero list kills every line = C(h)
            vector<int> lst(5 * h + 5, W / 2); maxset = 0;
            int r = play(h, lst);
            printf("h=%d  C(h)=%d  5h-10=%d  (zeros of length %d lose, of length %d win; max set %zu)\n",
                   h, r, 5 * h - 10, r - 1, r, maxset); fflush(stdout);
        }
    } else if (mode == "L") {
        int h = atoi(argv[2]); vector<int> lst;
        for (int i = 3; i < argc; i++) lst.push_back(atoi(argv[i]) + W / 2);
        vector<int> line; int r = play(h, lst, &line);
        if (r) printf("h=%d length %zu: WINS, every line dead at the add of round %d\n", h, lst.size(), r);
        else { printf("h=%d length %zu: LOSES, surviving line (vertex fired per round, . = pass):", h, lst.size());
               for (int v : line) { if (v < 0) printf(" ."); else printf(" %d", v - W / 2); } printf("\n"); }
    } else if (mode == "S") {
        int h = atoi(argv[2]), m = atoi(argv[3]), w = atoi(argv[4]); unsigned seed = atoi(argv[5]); long iters = atol(argv[6]);
        mt19937 rng(seed); long evals = 0; int bestEver = 0;
        // score = number of... we maximise "how early a truncated copy dies": play the list followed by
        // enough zeros?  Simpler: score = -(size of the surviving set after the last add); 0 survivors = win.
        auto score = [&](const vector<int>& l) -> long {
            // returns 0 if wins; else number of surviving positions after the last add+fire
            int mm = l.size(); Set cur; cur.insert(0);
            for (int t = 1; t <= mm; t++) {
                int a = l[t - 1]; Set nxt;
                for (u128 s : cur) {
                    if (get(s, a) + 1 >= h) continue;
                    u128 s1 = s + one(a); nxt.insert(s1);
                    if (t == mm) continue;
                    for (int v = 1; v < W - 1; v++) {
                        if (get(s1, v) < 2) continue;
                        if (get(s1, v - 1) + 1 >= h || get(s1, v + 1) + 1 >= h) continue;
                        nxt.insert(s1 - 2 * one(v) + one(v - 1) + one(v + 1));
                    }
                }
                if (nxt.empty()) return 0;
                cur.swap(nxt);
            }
            return (long)cur.size();
        };
        long found = 0;
        for (long it = 0; it < iters; it++) {
            vector<int> l(m); for (int& x : l) x = W / 2 + (int)(rng() % w);
            if (it % 2 == 0) { for (int& x : l) x = W / 2 + w / 2 + ((rng() % 4 == 0) ? (int)(rng() % 3) - 1 : 0); }
            long sc = score(l); evals++;
            for (int step = 0; step < 300 && sc > 0; step++) {
                vector<int> l2 = l; int i = rng() % m;
                if (rng() % 3 == 0) { int j = rng() % m; swap(l2[i], l2[j]); }
                else l2[i] = W / 2 + (int)(rng() % w);
                long s2 = score(l2); evals++;
                if (s2 <= sc) { l = l2; sc = s2; }
            }
            if (sc == 0) { found++; printf("WINNING list h=%d m=%d:", h, m); for (int x : l) printf(" %d", x - W / 2); printf("\n"); fflush(stdout); if (found >= 3) break; }
            (void)bestEver;
        }
        printf("search h=%d m=%d w=%d seed=%u restarts=%ld evaluations=%ld winning lists found=%ld\n", h, m, w, seed, iters, evals, found);
    }
    return 0;
}
