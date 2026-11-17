// Independent exact solver for F(n,t), n <= 7 (universe {0,1}^(n-1) fits in 64 bits).
// Shares no code with fast.py.  No dominance pruning; only identical sets are merged
// (identical nonempty sets intersect, so at most one of them can be used: safe).
// Model: strategies conflict iff same full word ua, or short-outcome sets meet.
//   Same (u,a) with n-t>=1 => short sets share uD(a) (nonempty).  t=n => strategy = u.
// Branching: pick the free element x lying in the fewest available sets; try each
// available set containing x, then the branch "x is not covered at all".
// Bound: every available set S has sum_{e in S} w(e) >= 1 where
//   w(e) = ceil(L / min{|S'| : S' available, e in S'}), L = 27720, so
//   #more sets <= floor(sum_{free e} w(e) / L).   Exact integer arithmetic.
// usage: bb n t [target]   (target: stop pruning threshold start = target-1, optional)
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;
typedef uint64_t u64;
static int n, t;
static vector<u64> sets; static vector<int> sz;
static int best = 0; static vector<int> bestsel, cur;
static unsigned long long nodes = 0;
static const long L = 27720;

static void rec(u64 freeM, vector<int>& avail) {
    nodes++;
    if ((int)cur.size() > best) { best = cur.size(); bestsel = cur; }
    if (avail.empty()) return;
    int U = 1 << (n - 1);
    static thread_local int dummy;
    (void)dummy;
    int mins[64], cntv[64];
    for (int e = 0; e < U; e++) { mins[e] = 1000; cntv[e] = 0; }
    for (int id : avail) {
        u64 m = sets[id]; int s = sz[id];
        while (m) { int e = __builtin_ctzll(m); m &= m - 1; cntv[e]++; if (s < mins[e]) mins[e] = s; }
    }
    long tot = 0; int x = -1, bc = 1 << 30;
    for (int e = 0; e < U; e++) if (cntv[e]) {
        tot += (L + mins[e] - 1) / mins[e];
        if (cntv[e] < bc) { bc = cntv[e]; x = e; }
    }
    if ((long)cur.size() + tot / L <= best) return;
    // branch: sets containing x
    for (int id : avail) if (sets[id] >> x & 1) {
        vector<int> na; na.reserve(avail.size());
        for (int j : avail) if (!(sets[j] & sets[id])) na.push_back(j);
        cur.push_back(id);
        rec(freeM & ~sets[id], na);
        cur.pop_back();
    }
    // branch: x uncovered
    vector<int> na; na.reserve(avail.size());
    for (int j : avail) if (!(sets[j] >> x & 1)) na.push_back(j);
    rec(freeM & ~(1ULL << x), na);
}

int main(int argc, char** argv) {
    n = atoi(argv[1]); t = atoi(argv[2]);
    if (n > 7 || n < 2) return 1;
    int m = n - t;
    set<u64> seen; vector<int> su, sa, sb;
    for (int u = 0; u < (1 << t); u++)
        for (int a = 0; a < (1 << m); a++)
            for (int b = 0; b < (1 << m); b++) {
                u64 S = 0;
                // u a' : delete bit position i of a (i = 0 is least significant)
                for (int i = 0; i < m; i++) {
                    int hi = a >> (i + 1), lo = a & ((1 << i) - 1);
                    int ap = (hi << i) | lo;            // length m-1
                    S |= 1ULL << ((u << (m - 1)) | ap);
                }
                for (int i = 0; i < t; i++) {
                    int hi = u >> (i + 1), lo = u & ((1 << i) - 1);
                    int up = (hi << i) | lo;            // length t-1
                    S |= 1ULL << ((up << m) | b);
                }
                if (seen.insert(S).second) { sets.push_back(S); sz.push_back(__builtin_popcountll(S)); su.push_back(u); sa.push_back(a); sb.push_back(b); }
            }
    if (argc > 3) best = atoi(argv[3]) - 1;
    vector<int> avail(sets.size());
    for (size_t i = 0; i < sets.size(); i++) avail[i] = i;
    // order by size (small first) to find good solutions early
    stable_sort(avail.begin(), avail.end(), [](int x, int y) { return sz[x] < sz[y]; });
    u64 fm = (n - 1 == 64) ? ~0ULL : ((n - 1 >= 64) ? ~0ULL : ((1ULL << (1 << (n - 1)) % 64) - 1));
    if ((1 << (n - 1)) == 64) fm = ~0ULL;
    rec(fm, avail);
    printf("n=%d t=%d distinct_sets=%zu F=%d nodes=%llu%s\n", n, t, sets.size(), best, nodes,
           (argc > 3 && bestsel.empty()) ? " (no packing of target size exists)" : "");
    return 0;
}
