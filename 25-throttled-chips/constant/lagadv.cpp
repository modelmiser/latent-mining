// Greedy adversary against Theorem 2's strategy (fire the ready vertex of largest lag if lag >= 1,
// ties -> leftmost).  Question: is the LAG bounded by a constant?  Adds are restricted to [0,n).
// Phase 0: K left-to-right sweeps (makes every vertex ready).  Phase 1: R rounds; each round the
// adversary picks the add x maximising  sum_z c^(lag5_z)  after Firer's reply (ties: random).
// Usage: lagadv n K R base seed      Prints max lag (in units, = fifths/5) and max height - t/5.
#include <bits/stdc++.h>
using namespace std;
int n; vector<int> k, F, eta, lg; set<pair<int,int>> S; // S: (-lag5,pos) eligible
long long t = 0; int maxlag = 0; long long maxexc = -1e18;
inline bool elig(int z){ return eta[z] >= 2 && lg[z] >= 5; }
void upd(int z, bool ins){ if (ins) { if (elig(z)) S.insert({-lg[z], z}); } else S.erase({-lg[z], z}); }
int reply(){ return S.empty() ? -1 : S.begin()->second; }
void doadd(int x){
    for (int z = x-1; z <= x+1; z++) upd(z, false);
    k[x]++; eta[x]++; lg[x] += 3; lg[x-1]++; lg[x+1]++; t++;
    for (int z = x-1; z <= x+1; z++) { upd(z, true); maxlag = max(maxlag, lg[z]); }
    maxexc = max(maxexc, 5LL*eta[x] - t);
    int f = reply();
    if (f >= 0) {
        for (int z = f-1; z <= f+1; z++) upd(z, false);
        F[f]++; lg[f] -= 5; eta[f] -= 2; eta[f-1]++; eta[f+1]++;
        for (int z = f-1; z <= f+1; z++) upd(z, true);
        maxexc = max(maxexc, 5LL*max(eta[f-1], eta[f+1]) - t);
    }
}
int main(int argc, char** argv){
    n = atoi(argv[1]); int K = atoi(argv[2]); long long R = atoll(argv[3]); double base = atof(argv[4]);
    mt19937 rng(atoi(argv[5]));
    int off = 8; int N = n + 2*off;
    k.assign(N,0); F.assign(N,0); eta.assign(N,0); lg.assign(N,0);
    vector<double> w(400); for (int i = 0; i < 400; i++) w[i] = pow(base, i);
    for (int s = 0; s < K; s++) for (int x = 0; x < n; x++) doadd(off + x);
    printf("after phase 0: t=%lld maxlag=%.1f maxexc=%.1f\n", t, maxlag/5.0, maxexc/5.0);
    int hist[64]; long long nextrep = R/10;
    for (long long r = 0; r < R; r++) {
        double best = -1e300; int bx = -1; int cnt = 0;
        for (int x = off; x < off + n; x++) {
            // simulate add at x
            int l0 = lg[x-1]+1, l1 = lg[x]+3, l2 = lg[x+1]+1;
            int e1 = eta[x]+1;
            int bl = -1, bp = -1; // best eligible (max lag, leftmost)
            if (eta[x-1] >= 2 && l0 >= 5) { bl = l0; bp = x-1; }
            if (e1 >= 2 && l1 >= 5 && l1 > bl) { bl = l1; bp = x; }
            if (eta[x+1] >= 2 && l2 >= 5 && l2 > bl) { bl = l2; bp = x+1; }
            for (auto it = S.begin(); it != S.end(); ++it) {
                int z = it->second; if (z >= x-1 && z <= x+1) continue;
                int l = -it->first;
                if (l > bl || (l == bl && z < bp)) { bl = l; bp = z; }
                break;
            }
            double d = (w[l0]-w[l0-1]) + (w[l1]-w[l1-3]) + (w[l2]-w[l2-1]);
            if (bp >= 0) {
                int cur = (bp == x-1) ? l0 : (bp == x) ? l1 : (bp == x+1) ? l2 : lg[bp];
                d += w[cur-5] - w[cur];
            }
            if (d > best + 1e-9*fabs(best)) { best = d; bx = x; cnt = 1; }
            else if (d >= best - 1e-9*fabs(best)) { cnt++; if (rng() % cnt == 0) bx = x; }
        }
        doadd(bx);
        if (r + 1 == nextrep) {
            memset(hist, 0, sizeof hist); int cur = 0;
            for (int x = off; x < off+n; x++) { hist[min(63, lg[x])]++; cur = max(cur, lg[x]); }
            printf("r=%lld maxlag_ever=%.1f cur_max=%.1f maxexc=%.1f top-levels:", r+1, maxlag/5.0, cur/5.0, maxexc/5.0);
            for (int i = cur; i > cur-6 && i >= 0; i--) printf(" %.1f:%d", i/5.0, hist[i]);
            printf("\n"); fflush(stdout); nextrep += R/10;
        }
    }
    printf("FINAL n=%d K=%d R=%lld base=%g maxlag=%.1f maxexc=%.1f\n", n, K, R, base, maxlag/5.0, maxexc/5.0);
}
