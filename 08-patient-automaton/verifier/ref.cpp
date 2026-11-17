// Referee's own program, written from the problem statement.
// usage: ref n dmax prune(0/1) [lo hi]   -- counts |W_d(n)|, |W_inf(n)| over automata index range
// Automaton index: digits base n; first n digits = letter a on states 0..n-1, next n = letter b.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string>
using namespace std;
typedef unsigned long long u64;
int n; int NS;
int mp[2][8];
int img[2][256];
int pc[256];

void build() {
    for (int l = 0; l < 2; l++) for (int S = 0; S < NS; S++) {
        int r = 0; for (int i = 0; i < n; i++) if (S >> i & 1) r |= 1 << mp[l][i];
        img[l][S] = r;
    }
}
// ---- blind game: track the set of unordered pairs that some Spoiler fill keeps apart
int pid[8][8]; int npairs; int pa[32], pb[32];
static vector<unsigned char> seen;
bool blind() {
    // pair transitions
    int stepx[2][32];      // after sync letter x: pair index or -1 (merged)
    for (int x = 0; x < 2; x++) for (int p = 0; p < npairs; p++) {
        int u = mp[x][pa[p]], v = mp[x][pb[p]];
        stepx[x][p] = (u == v) ? -1 : pid[u][v];
    }
    int full = (1 << npairs) - 1;
    fill(seen.begin(), seen.end(), 0);
    vector<int> q; q.push_back(full); seen[full] = 1;
    for (size_t h = 0; h < q.size(); h++) {
        int U = q[h];
        for (int x = 0; x < 2; x++) {
            int mid = 0;
            for (int p = 0; p < npairs; p++) if (U >> p & 1) { int r = stepx[x][p]; if (r >= 0) mid |= 1 << r; }
            if (mid == 0) return true;          // every surviving pair merged by x: singleton after this letter
            int V = 0;
            for (int p = 0; p < npairs; p++) if (mid >> p & 1) for (int y = 0; y < 2; y++) { int r = stepx[y][p]; if (r >= 0) V |= 1 << r; }
            // V == 0 is fine: then any next letter wins (mid==0 at next step)
            if (!seen[V]) { seen[V] = 1; q.push_back(V); }
        }
    }
    return false;
}
// ---- delay game: positions (S, w), w = d pending Synchronizer letters, oldest = top bit
static vector<unsigned char> W;
bool delay(int d) {
    int NW = 1 << d;
    W.assign((size_t)NS * NW, 0);
    for (int S = 1; S < NS; S++) if (pc[S] == 1) for (int w = 0; w < NW; w++) W[(size_t)S * NW + w] = 1;
    int Q = NS - 1;
    bool ch = true;
    while (ch) {
        ch = false;
        for (int S = 1; S < NS; S++) {
            if (pc[S] == 1) continue;
            for (int w = 0; w < NW; w++) {
                if (W[(size_t)S * NW + w]) continue;
                bool ok = false;
                for (int x = 0; x < 2 && !ok; x++) {
                    int c, u;
                    if (d == 0) { c = x; u = 0; }
                    else { c = w >> (d - 1); u = ((w & ((1 << (d - 1)) - 1)) << 1) | x; }
                    int Sc = img[c][S];
                    bool all = true;
                    for (int y = 0; y < 2; y++) if (!W[(size_t)img[y][Sc] * NW + u]) { all = false; break; }
                    if (all) ok = true;
                }
                if (ok) { W[(size_t)S * NW + w] = 1; ch = true; }
            }
        }
    }
    for (int w = 0; w < NW; w++) if (W[(size_t)Q * NW + w]) return true;
    return false;
}
string canon() {
    vector<int> p(n); for (int i = 0; i < n; i++) p[i] = i;
    string best;
    do {
        for (int sw = 0; sw < 2; sw++) {
            string s(2 * n, '0');
            for (int l = 0; l < 2; l++) for (int i = 0; i < n; i++) s[(l ^ sw) * n + p[i]] = '1' + p[mp[l][i]];
            if (best.empty() || s < best) best = s;
        }
    } while (next_permutation(p.begin(), p.end()));
    return best;
}
int main(int argc, char** argv) {
    if (!strcmp(argv[1], "one")) {   // ref one dmax < lines "a-row b-row" as digit strings 1..n
        int dmax = atoi(argv[2]); char sa[32], sb[32];
        for (int i = 0; i < 256; i++) pc[i] = __builtin_popcount(i);
        while (scanf("%31s %31s", sa, sb) == 2) {
            n = strlen(sa); NS = 1 << n;
            npairs = 0; for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) { pid[i][j] = pid[j][i] = npairs; pa[npairs] = i; pb[npairs] = j; npairs++; }
            seen.assign(1 << npairs, 0);
            for (int i = 0; i < n; i++) { mp[0][i] = sa[i] - '1'; mp[1][i] = sb[i] - '1'; }
            build();
            printf("%s %s blind=%d win_d=", sa, sb, (int)blind());
            for (int d = 0; d <= dmax; d++) printf("%d", (int)delay(d));
            printf("\n");
        }
        return 0;
    }
    n = atoi(argv[1]); int dmax = atoi(argv[2]); int prune = atoi(argv[3]);
    NS = 1 << n;
    for (int i = 0; i < 256; i++) pc[i] = __builtin_popcount(i);
    npairs = 0; for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) { pid[i][j] = pid[j][i] = npairs; pa[npairs] = i; pb[npairs] = j; npairs++; }
    seen.assign(1 << npairs, 0);
    u64 tot = 1; for (int i = 0; i < 2 * n; i++) tot *= n;
    u64 lo = 0, hi = tot; if (argc > 5) { lo = strtoull(argv[4], 0, 10); hi = strtoull(argv[5], 0, 10); }
    vector<u64> cnt(dmax + 1, 0); u64 cinf = 0, viol = 0;
    map<string, u64> classes;   // survivors at dmax not blind-winnable, by canonical form
    for (u64 idx = lo; idx < hi; idx++) {
        u64 t = idx; for (int l = 0; l < 2; l++) for (int i = 0; i < n; i++) { mp[l][i] = t % n; t /= n; }
        build();
        bool bl = blind(); if (bl) cinf++;
        bool prev = true; bool last = false;
        for (int d = 0; d <= dmax; d++) {
            bool w;
            if (prune && bl) w = true;
            else if (prune && !prev) w = false;
            else w = delay(d);
            if (!prune) { if (bl && !w) viol++; if (w && !prev) viol++; }
            if (w) cnt[d]++;
            prev = w; last = w;
        }
        if (last && !bl) classes[canon()]++;
    }
    printf("n=%d range [%llu,%llu) prune=%d\n", n, lo, hi, prune);
    for (int d = 0; d <= dmax; d++) printf("W_%d %llu\n", d, cnt[d]);
    printf("W_inf %llu\nmonotonicity/inclusion violations (unpruned only) %llu\n", cinf, viol);
    printf("survivors at d=%d not blind: %zu classes\n", dmax, classes.size());
    if (classes.size() <= 400) for (auto& kv : classes) printf("CLASS %s members_in_range %llu\n", kv.first.c_str(), kv.second);
    return 0;
}
