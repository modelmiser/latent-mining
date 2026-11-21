// replay.cpp -- independent replay of a list under the lag rule (solver-a Theorem 2).
// Written from the rule's text, not from gen.py: different language, an ordered set instead of a
// lazy heap, chips and firings kept separately and the lag recomputed from k and F.
// usage: replay LIST TIE [EXPECT]     TIE = L | R | H ;  EXPECT = level (5*lag) that must be
// reached after some round, else exit status 1.  Also checks Theorem U: lag after every round
// <= 2 + H_n, n = number of vertices within distance 1 of a list entry (exit status 2 if violated).
#include <cstdio>
#include <cstdlib>
#include <set>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
typedef long long ll;
char tieR;
vector<ll> k, F, eta;
ll level(ll x) { return 3 * k[x] + k[x - 1] + k[x + 1] - 5 * F[x]; }
ll keyf(ll x) { return tieR == 'L' ? x : tieR == 'R' ? -x : (x * 2654435761LL + 12345) % 4294967291LL; }
int main(int argc, char **argv) {
    FILE *f = fopen(argv[1], "r"); tieR = argv[2][0];
    ll expect = argc > 3 ? atoll(argv[3]) : -1;
    vector<ll> lst; ll a, mx = 0;
    while (fscanf(f, "%lld", &a) == 1) { if (a < 2) { puts("vertex < 2"); return 3; } lst.push_back(a); mx = max(mx, a); }
    k.assign(mx + 4, 0); F.assign(mx + 4, 0); eta.assign(mx + 4, 0);
    vector<ll> cur(mx + 4, -1);          // level under which x is currently in the set, or -1
    set<pair<ll, ll> > S;                // (-level, key) of ready vertices with lag >= 1
    vector<ll> who;                      // key -> vertex is not needed for L/R; for H keep a map
    auto upd = [&](ll x) {
        if (cur[x] >= 0) { S.erase(make_pair(-cur[x], keyf(x))); cur[x] = -1; }
        ll l = level(x);
        if (eta[x] >= 2 && l >= 5) { S.insert(make_pair(-l, keyf(x))); cur[x] = l; }
    };
    // vertex from key: invert by table for H
    vector<pair<ll, ll> > inv;
    if (tieR == 'H') { for (ll x = 1; x < mx + 3; x++) inv.push_back(make_pair(keyf(x), x)); sort(inv.begin(), inv.end()); }
    auto vert = [&](ll key) -> ll {
        if (tieR == 'L') return key; if (tieR == 'R') return -key;
        return lower_bound(inv.begin(), inv.end(), make_pair(key, (ll)-1))->second;
    };
    ll recRound = 0, recAdd = 0, passes = 0, t = 0, neg = 0;
    ll excAdd5 = -1000000000, excRound5 = -1000000000;   // 5*(height - t/5)
    vector<ll> lv(mx + 4, 0);            // level of every vertex, to track the maximum after a round
    multiset<ll> all;                    // multiset of nonzero levels
    auto setlv = [&](ll x) { ll l = level(x); if (l < 0) neg++; if (lv[x]) all.erase(all.find(lv[x])); lv[x] = l; if (l) all.insert(l); };
    for (ll a : lst) {
        t++;
        k[a]++; eta[a]++;
        for (ll z = a - 1; z <= a + 1; z++) { setlv(z); upd(z); }
        recAdd = max(recAdd, *all.rbegin());
        excAdd5 = max(excAdd5, 5 * eta[a] - t);
        if (S.empty()) passes++;
        else {
            ll x = vert(S.begin()->second);
            if (level(x) != -S.begin()->first || eta[x] < 2) { puts("internal error"); return 4; }
            F[x]++; eta[x] -= 2; eta[x - 1]++; eta[x + 1]++;
            setlv(x); for (ll z = x - 1; z <= x + 1; z++) upd(z);
            excRound5 = max(excRound5, 5 * max(eta[x - 1], eta[x + 1]) - t);
        }
        if (!all.empty()) recRound = max(recRound, *all.rbegin());
    }
    ll n = 0; for (ll x = 1; x < mx + 3; x++) if (k[x - 1] || k[x] || k[x + 1]) n++;
    double H = 0; for (ll i = 1; i <= n; i++) H += 1.0 / i;
    ll top = all.empty() ? 0 : *all.rbegin();
    printf("length %lld tie %c  max lag after a round %.1f  after an add %.1f  final max lag %.1f at %zu vertices\n",
           t, tieR, recRound / 5.0, recAdd / 5.0, top / 5.0, all.count(top));
    printf("  max height - t/5: after an add %.1f, after a firing %.1f   passes %lld  negative lags seen %lld\n",
           excAdd5 / 5.0, excRound5 / 5.0, passes, neg);
    printf("  Theorem U: n = %lld, bound 2 + H_n = %.3f, largest lag after a round %.1f : %s\n", n, 2 + H, recRound / 5.0,
           recRound / 5.0 <= 2 + H ? "ok" : "VIOLATED");
    if (recRound / 5.0 > 2 + H) return 2;
    if (expect >= 0) { printf("  expected level %lld after a round: %s\n", expect, recRound >= expect ? "reached" : "NOT REACHED"); if (recRound < expect) return 1; }
    return 0;
}
