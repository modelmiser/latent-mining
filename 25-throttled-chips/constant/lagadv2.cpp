// Greedy adversary against Theorem 2's strategy (fire the ready vertex of largest lag if lag >= 1,
// ties -> leftmost).  Question: is the LAG bounded by a constant?  Adds are restricted to [0,n).
// Phase 0: K left-to-right sweeps (makes every vertex ready).  Phase 1: R rounds; each round the
// adversary picks the add x maximising  sum_z c^(lag5_z)  after Firer's reply (ties: random).
// Usage: lagadv n K R base seed      Prints max lag (in units, = fifths/5) and max height - t/5.
#include <bits/stdc++.h>
using namespace std;
int n; vector<int> k, F, eta, lg; set<pair<int,int>> S; // S: (-lag5,pos) eligible
long long t = 0; FILE* dump = nullptr; int maxlag = 0; long long maxexc = -1e18;
inline bool elig(int z){ return eta[z] >= 2 && lg[z] >= 5; }
void upd(int z, bool ins){ if (ins) { if (elig(z)) S.insert({-lg[z], z}); } else S.erase({-lg[z], z}); }
int reply(){ return S.empty() ? -1 : S.begin()->second; }
void doadd(int x){
    if (dump) fprintf(dump, "%d\n", x);
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
    if (getenv("DUMP")) dump = fopen(getenv("DUMP"), "w");
    int off = 8; int N = n + 2*off;
    k.assign(N,0); F.assign(N,0); eta.assign(N,0); lg.assign(N,0);
    vector<double> w(400); for (int i = 0; i < 400; i++) w[i] = pow(base, i);
    for (int s = 0; s < K; s++) for (int x = 0; x < n; x++) doadd(off + x);
    printf("after phase 0: t=%lld maxlag=%.1f maxexc=%.1f\n", t, maxlag/5.0, maxexc/5.0);
    // staged adversary: target level tau (fifths).  Each round pick the add maximising the number
    // of vertices with lag >= tau after Firer's reply (secondary: sum base^lag).  If no add
    // increases that number, tau += 1.  Stop after R rounds or when tau exceeds maxlag by 40.
    int tau = 6; long long cnt_tau = 0; int stuck = 0;
    for (int x = off-1; x <= off+n; x++) cnt_tau += lg[x] >= tau;
    long long r = 0; int lastprint = -1;
    while (r < R) {
        long long bestc = -1000000; double best = -1e300; int bx = -1; int cnt = 0;
        for (int x = off; x < off + n; x++) {
            int l0 = lg[x-1]+1, l1 = lg[x]+3, l2 = lg[x+1]+1;
            int e1 = eta[x]+1;
            int bl = -1, bp = -1;
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
            long long dc = (l0 >= tau) - (l0-1 >= tau) + (l1 >= tau) - (l1-3 >= tau) + (l2 >= tau) - (l2-1 >= tau);
            if (bp >= 0) {
                int cur = (bp == x-1) ? l0 : (bp == x) ? l1 : (bp == x+1) ? l2 : lg[bp];
                d += w[cur-5] - w[cur]; dc += (cur-5 >= tau) - (cur >= tau);
            }
            if (dc > bestc || (dc == bestc && d > best + 1e-9*fabs(best))) { bestc = dc; best = d; bx = x; cnt = 1; }
            else if (dc == bestc && d >= best - 1e-9*fabs(best)) { cnt++; if (rng() % cnt == 0) bx = x; }
        }
        if (bestc <= 0) {
            if (tau != lastprint) { printf("stage tau=%.1f ended: r=%lld count(lag>=tau)=%lld maxlag_ever=%.1f\n", tau/5.0, r, cnt_tau, maxlag/5.0); fflush(stdout); lastprint = tau; }
            tau++; cnt_tau = 0; for (int x = off-1; x <= off+n; x++) cnt_tau += lg[x] >= tau;
            if (tau > maxlag + 2) break;
            continue;
        }
        doadd(bx); cnt_tau += bestc; r++;
    }
    if (dump) fclose(dump);
    printf("FINAL n=%d K=%d R=%lld base=%g maxlag=%.1f maxexc=%.1f\n", n, K, R, base, maxlag/5.0, maxexc/5.0);
}
