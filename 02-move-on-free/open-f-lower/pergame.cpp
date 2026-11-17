// Periodic density game for the P6R family (fast version of pergame.py, plus wrap-around pairs).
// See pergame.py for the model.  usage: pergame "5,8" R1 R2 threads [keptsizes e.g. 9,11] [topN]
//   Manager's stage-1 layout: runs of 1s (lengths cycled from the list) separated by 3-gaps.
//   Adversary keeps a periodic subset S (exhaustive over one super-period).
//   Manager places y=(n-|S|)/2 2s per super-period: any disjoint free pairs (cyclic), rest contiguous in the tail.
//   Adversary frees a periodic subset F of pattern 2s and/or every other tail 2.
//   Manager deletes <= |F| ones with foresight (exact interdiction DP).  Adversary thins by the waste DP.
//   value = slope (W(R2)-W(R1)) / ((R2-R1) n);  2/3 <=> held at 5M/3.
#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
typedef pair<int,int> Blk;
static const int NEG = -1000000;
static vector<int> home_; static vector<int> ones; static int Lsp, n1;

// exact: min over deletions of <=K ones of max over thinnings of W
static int interdict(const vector<Blk>& lay, int K) {
    // state code: 3 comps each in {0:=0, 1:=-1, 2:=-2, 3:=NEG}
    int S = 64;
    vector<int> cur((K + 1) * S, INT_MAX), nxt((K + 1) * S, INT_MAX);
    auto enc = [](int a, int b, int c) { return a | (b << 2) | (c << 4); };
    cur[0 * S + enc(0, 3, 3)] = 0;
    static const int dec[4] = {0, -1, -2, NEG};
    for (auto& bl : lay) {
        int a = bl.first, z = bl.second;
        fill(nxt.begin(), nxt.end(), INT_MAX);
        int e2 = (a + z) % 3;
        for (int k = 0; k <= K; k++) for (int s = 0; s < S; s++) {
            int vm = cur[k * S + s]; if (vm == INT_MAX) continue;
            int v[3];
            for (int e = 0; e < 3; e++) { int d = (s >> (2 * e)) & 3; v[e] = d == 3 ? NEG : vm + dec[d]; }
            int w = NEG;
            for (int e = 0; e < 3; e++) if (v[e] > NEG / 2) w = max(w, v[e] + ((a - e) % 3 + 3) % 3);
            auto push = [&](int kk, int* nv) {
                int m = max(nv[0], max(nv[1], nv[2]));
                int code = 0;
                for (int e = 0; e < 3; e++) {
                    int d = nv[e] > NEG / 2 ? min(2, m - nv[e]) : 3;
                    code |= d << (2 * e);
                }
                int& t = nxt[kk * S + code]; if (m < t) t = m;
            };
            if (z == 1) {
                int nv[3] = {v[0], v[1], v[2]};
                if (w > nv[e2]) nv[e2] = w;
                push(k, nv);
                if (k < K) { int nv2[3] = {v[0], v[1], v[2]}; push(k + 1, nv2); }
            } else {
                int nv[3] = {NEG, NEG, NEG}; nv[e2] = w; push(k, nv);
            }
        }
        swap(cur, nxt);
    }
    int best = INT_MAX;
    for (int x : cur) best = min(best, x);
    return best;
}

struct Val { int w; int fm; int tf; };
// adversary's best W against (S, placement) at R repetitions.  If cut>=0: stop early once w - base*... not used
static Val value(unsigned long long Smask, const vector<int>& pl, int yp, int R) {
    int j = pl.size(), tail_n = yp - j;
    vector<Blk> lay;
    for (int rep = 0; rep < R; rep++) {
        int off = rep * Lsp;
        for (int i = 0; i < n1; i++) if (Smask >> i & 1) lay.push_back({off + ones[i], 1});
        for (int p : pl) lay.push_back({off + p, 2});
    }
    sort(lay.begin(), lay.end());
    int t = lay.empty() ? 0 : lay.back().first + lay.back().second;
    t += ((-t) % 3 + 3) % 3;
    vector<Blk> tail;
    for (int i = 0; i < tail_n * R; i++) tail.push_back({t + 2 * i, 2});
    Val best = {INT_MIN, 0, 0};
    for (int fm = 0; fm < (1 << j); fm++) for (int tf = 0; tf < (tail_n ? 2 : 1); tf++) {
        vector<Blk> l; int K = 0;
        for (auto& b : lay) {
            if (b.second == 2) {
                int p = ((b.first % Lsp) + Lsp) % Lsp; bool rem = false;
                for (int i = 0; i < j; i++) if ((fm >> i & 1) && pl[i] == p) rem = true;
                if (rem) { K++; continue; }
            }
            l.push_back(b);
        }
        for (size_t i = 0; i < tail.size(); i++) { if (tf && (i & 1)) { K++; continue; } l.push_back(tail[i]); }
        int v = interdict(l, K);
        if (v > best.w) best = {v, fm, tf};
    }
    return best;
}

int main(int argc, char** argv) {
    { stringstream ss(argv[1]); string tok; while (getline(ss, tok, ',')) home_.push_back(atoi(tok.c_str())); }
    int R1 = atoi(argv[2]), R2 = atoi(argv[3]), th = atoi(argv[4]);
    set<int> sizes; if (argc > 5) { stringstream ss(argv[5]); string tok; while (getline(ss, tok, ',')) sizes.insert(atoi(tok.c_str())); }
    int topN = argc > 6 ? atoi(argv[6]) : 12;
    int n = 0; for (int r : home_) n += r;
    int t = 1; while ((t * n) % 3) t++;
    int pos = 0;
    for (int i = 0; i < t; i++) for (int r : home_) { for (int k = 0; k < r; k++) ones.push_back(pos + k); pos += r + 3; }
    Lsp = pos; n1 = ones.size();
    printf("home=%s super-period: %d ones on %d cells; span/M=%.3f; R1=%d R2=%d\n", argv[1], n1, Lsp, (double)Lsp / n1, R1, R2);
    if (n1 > 30 && !getenv("FIXS")) { printf("too large for exhaustive S\n"); return 1; }
    struct Res { double slope; int num; unsigned long long S; int kept, yp; vector<int> pl; Val v1, v2; long npl; };
    vector<Res> results;
    int dM = (R2 - R1) * n1;
    double gbest = -1; bool prune = getenv("PRUNE") != nullptr;
    vector<unsigned long long> masks;
    // FILTER=1: only S such that no dropped 1 is adjacent to another free cell (so the only free pairs lie inside
    //           the reserved 3-gaps, each bounded by kept 1s) -- the "no hiding slot" family.
    bool filt = getenv("FILTER") != nullptr;
    // FILTER=2: additionally S must contain two full residue classes (the "blocker" family of proofs.txt R4)
    bool filt2 = filt && atoi(getenv("FILTER")) == 2;
    unsigned long long cm[3] = {0, 0, 0};
    for (int i = 0; i < n1; i++) cm[ones[i] % 3] |= 1ull << i;
    vector<char> isone(Lsp, 0); vector<int> idx(Lsp, -1);
    for (int i = 0; i < n1; i++) { isone[ones[i]] = 1; idx[ones[i]] = i; }
    if (getenv("FIXS")) {
        string fs = getenv("FIXS"); unsigned long long S = 0;
        for (int i = 0; i < n1; i++) if (fs[ones[i] % fs.size()] == '1') S |= 1ull << i;
        masks.push_back(S);
    } else
    for (unsigned long long S64 = 0; S64 < (1ull << n1); S64++) {
        unsigned long long S = S64;
        int kept = __builtin_popcountll(S);
        if ((n1 - kept) & 1) continue;
        if (!sizes.empty() && !sizes.count(kept)) continue;
        if (filt2) {
            bool two = false;
            for (int d = 0; d < 3; d++) if ((S & cm[(d + 1) % 3]) == cm[(d + 1) % 3] && (S & cm[(d + 2) % 3]) == cm[(d + 2) % 3]) two = true;
            if (!two) continue;
        }
        if (filt) {
            bool ok = true;
            for (int i = 0; i < n1 && ok; i++) if (!(S >> i & 1)) {
                int c = ones[i];
                for (int d = -1; d <= 1; d += 2) {
                    int c2 = ((c + d) % Lsp + Lsp) % Lsp;
                    if (!isone[c2] || !(S >> idx[c2] & 1)) ok = false;
                }
            }
            if (!ok) continue;
        }
        masks.push_back(S);
    }
    #pragma omp parallel for num_threads(th) schedule(dynamic, 1)
    for (size_t mi = 0; mi < masks.size(); mi++) {
        unsigned long long S = masks[mi];
        int kept = __builtin_popcountll(S), yp = (n1 - kept) / 2;
        vector<char> occ(Lsp, 0);
        for (int i = 0; i < n1; i++) if (S >> i & 1) occ[ones[i]] = 1;
        vector<int> pairs;
        for (int p = 0; p < Lsp; p++) if (!occ[p] && !occ[(p + 1) % Lsp]) pairs.push_back(p);
        // enumerate placements (cyclically disjoint)
        vector<vector<int>> pls; vector<int> cur;
        function<void(size_t)> rec = [&](size_t start) {
            pls.push_back(cur);
            if ((int)cur.size() == yp || (getenv("MAXIN") && (int)cur.size() >= atoi(getenv("MAXIN")))) return;
            for (size_t i = start; i < pairs.size(); i++) {
                int p = pairs[i];
                if (!cur.empty() && p <= cur.back() + 1) continue;
                if (!cur.empty() && p == Lsp - 1 && cur[0] == 0) continue;
                cur.push_back(p); rec(i + 1); cur.pop_back();
            }
        };
        rec(0);
        Res best; best.slope = 1e9; best.npl = pls.size(); best.S = S; best.kept = kept; best.yp = yp; best.num = 0; best.v1 = {0,0,0}; best.v2 = {0,0,0};
        if (getenv("FIXS")) {
            // manager's min over placements, in parallel; static evaluation (F empty) first as a filter
            double bestsl = 1e9; long done = 0;
            #pragma omp parallel for num_threads(th) schedule(dynamic, 4)
            for (size_t pi = 0; pi < pls.size(); pi++) {
                auto& pl = pls[pi];
                Val v1 = value(S, pl, yp, R1), v2 = value(S, pl, yp, R2);
                double sl = (double)(v2.w - v1.w) / dM;
                #pragma omp critical
                { done++; if (sl < bestsl) { bestsl = sl; best.slope = sl; best.num = v2.w - v1.w; best.pl = pl; best.v1 = v1; best.v2 = v2;
                    string str(Lsp + 1, '.');
                    for (int k = 0; k < n1; k++) if (S >> k & 1) str[ones[k]] = '1';
                    for (int p : pl) { str[p] = '2'; if (p + 1 < Lsp) str[p + 1] = '2'; else str[Lsp] = '>'; }
                    printf("    new best %.4f after %ld/%zu placements: %s tail=%d advF=%d tf=%d\n", sl, done, pls.size(), str.c_str(), yp - (int)pl.size(), v2.fm, v2.tf); fflush(stdout); } }
            }
        } else
        for (auto& pl : pls) {
            if (prune && best.slope < gbest - 1e-9) break;   // this S cannot be the maximiser any more
            Val v1 = value(S, pl, yp, R1), v2 = value(S, pl, yp, R2);
            double sl = (double)(v2.w - v1.w) / dM;
            if (getenv("DEBUGS") && (unsigned long long)atoll(getenv("DEBUGS")) == S) {
                string s(Lsp + 1, '.');
                for (int k = 0; k < n1; k++) if (S >> k & 1) s[ones[k]] = '1';
                for (int p : pl) { s[p] = '2'; if (p + 1 < Lsp) s[p + 1] = '2'; else s[Lsp] = '>'; }
                #pragma omp critical
                printf("    DEBUG %s tail=%d slope=%.4f advF=%d tf=%d W1=%d W2=%d\n", s.c_str(), yp - (int)pl.size(), sl, v2.fm, v2.tf, v1.w, v2.w);
            }
            if (sl < best.slope) { best.slope = sl; best.num = v2.w - v1.w; best.S = S; best.kept = kept; best.yp = yp; best.pl = pl; best.v1 = v1; best.v2 = v2; }
        }
        #pragma omp critical
        { results.push_back(best); if (best.slope > gbest) gbest = best.slope; }
    }
    sort(results.begin(), results.end(), [](const Res& a, const Res& b) { return a.slope > b.slope || (a.slope == b.slope && a.S < b.S); });
    for (int i = 0; i < (int)results.size() && i < topN; i++) {
        Res& r = results[i];
        string s(Lsp + 1, '.');
        for (int k = 0; k < n1; k++) if (r.S >> k & 1ull) s[ones[k]] = '1';
        for (int p : r.pl) { s[p] = '2'; if (p + 1 < Lsp) s[p + 1] = '2'; else s[Lsp] = '>'; }
        printf("  slope=%.4f (%d/%d) kept=%d y=%d  manager's best placement: %s tail=%d (of %ld placements)  adv F=%d tailfree=%d  W(R1)=%d W(R2)=%d\n",
               r.slope, r.num, dM, r.kept, r.yp, s.c_str(), r.yp - (int)r.pl.size(), r.npl, r.v2.fm, r.v2.tf, r.v1.w, r.v2.w);
    }
    if (!results.empty())
        printf("  ==> periodic game value (max over %zu S of min over placements): slope=%.4f   (2/3=0.6667 means held at 5M/3; 3/4 means 7M/4... as H/M = 1 + slope)\n",
               results.size(), results[0].slope);
    return 0;
}
