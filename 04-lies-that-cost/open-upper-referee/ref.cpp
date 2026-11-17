// Referee's independent solver for the Q_k game on obligation multisets.
// State: cnt[r], r = remaining obligation of an alive candidate, clipped at j (questions left).
// Transition (solver-a P1): truthful -> max(r-1,0); liar needs r==0 and gets k*ell (0 if ell==0); liar with r>0 dies.
// Modes:
//   t1 K           : T1 for k=K (all nice (a,b;j), j<=14, w<=2^j, 2<=a+b<=2j-3)   [BAD variant: t1bad K -> w<=2^j+1]
//   wide K J M     : all (a,b;j), j<=J, a+b<=M : winnable <=> (nice ? w<=2^j : report only)
//   root K N       : Q_K(n) for n=2..N by search, compare with P(n)
//   noprune K J M  : compare pruned vs unpruned solver on all (a,b;j)
//   trans K1 K2 J  : compare transition functions of k=K1 and k=K2 on all states (a,b;j'), j'<=J, a+b<=8, all questions, recursively
#include <bits/stdc++.h>
using namespace std;
typedef vector<int> St;
int K; bool PRUNE = true;
unordered_map<string, char> memo[64];
long long nodes = 0;

static string key(const St& c) { string s(c.size(), 0); for (size_t i = 0; i < c.size(); i++) s[i] = (char)c[i]; return s; }
static long long weight(const St& c, int j) { long long w = 0; for (int r = 0; r <= j; r++) w += (long long)c[r] * (1 + max(0, j - r)); return w; }
static int total(const St& c) { int m = 0; for (int x : c) m += x; return m; }

// apply question s (s[r] of class r in S) with answer yes (ans=1) or no (ans=0); k given
static St step(const St& c, const vector<int>& s, int j, int ans, int k) {
  int m = total(c), sz = 0; for (int r = 0; r <= j; r++) sz += s[r];
  int ell = min(sz, m - sz);
  St n(j, 0); // new j' = j-1, indices 0..j-1
  int jp = j - 1;
  for (int r = 0; r <= j; r++) {
    int truth = ans ? s[r] : c[r] - s[r];
    int liar = c[r] - truth;
    int rp = min(max(r - 1, 0), jp);
    n[rp] += truth;
    if (r == 0 && liar) { long long ob = (long long)k * ell; int o = (int)min<long long>(ob, jp); n[o] += liar; }
  }
  return n;
}

bool win(const St& c, int j) {
  int m = total(c);
  if (m == 1) return true;
  if (m == 0) { fprintf(stderr, "empty state\n"); exit(2); }
  if (j == 0) return false;
  if (PRUNE && weight(c, j) > (1LL << j)) return false;
  string ky = key(c);
  auto it = memo[j].find(ky);
  if (it != memo[j].end()) return it->second;
  nodes++;
  vector<int> s(j + 1, 0);
  bool res = false;
  while (true) {
    St y = step(c, s, j, 1, K), n = step(c, s, j, 0, K);
    int my = total(y), mn = total(n);
    bool ok = true;
    if (PRUNE) {
      if (my && weight(y, j - 1) > (1LL << (j - 1))) ok = false;
      if (ok && mn && weight(n, j - 1) > (1LL << (j - 1))) ok = false;
    }
    if (ok && my && !win(y, j - 1)) ok = false;
    if (ok && mn && !win(n, j - 1)) ok = false;
    if (ok) { res = true; break; }
    int r = 0;
    while (r <= j) { if (s[r] < c[r]) { s[r]++; break; } s[r] = 0; r++; }
    if (r > j) break;
  }
  memo[j][ky] = res;
  return res;
}
static St mk(int a, int b, int j) { St c(j + 1, 0); c[0] += a; c[j] += b; return c; } // j=0: both in c[0]
static long long P(long long n) { for (int q = 1;; q++) { long long l = n * (q + 1) + ((n & 1) ? q - 1 : 0); if (l <= (1LL << q)) return q; } }

int main(int argc, char** argv) {
  string mode = argv[1]; K = atoi(argv[2]);
  if (mode == "t1" || mode == "t1bad") {
    long long slack = (mode == "t1bad") ? 1 : 0;
    long long cnt = 0, lost = 0;
    for (int j = 1; j <= 14; j++) for (int a = 0; a <= 2 * j; a++) for (int b = max(0, a - 1); a + b <= 2 * j - 3; b++) {
      if (a + b < 2) continue;
      if ((long long)a * (j + 1) + b > (1LL << j) + slack) continue;
      cnt++;
      if (!win(mk(a, b, j), j)) { lost++; if (lost <= 5) printf("  LOST k=%d (a=%d,b=%d;j=%d)\n", K, a, b, j); }
    }
    printf("%s k=%d states=%lld lost=%lld nodes=%lld\n", mode.c_str(), K, cnt, lost, nodes);
    return lost ? 1 : 0;
  }
  if (mode == "wide") {
    int J = atoi(argv[3]), M = atoi(argv[4]);
    long long cnt = 0, bad = 0, nn = 0, nnbad = 0;
    for (int j = 0; j <= J; j++) for (int a = 0; a <= M; a++) for (int b = 0; a + b <= M; b++) {
      if (a + b < 1) continue;
      bool w = win(mk(a, b, j), j);
      bool pred = (long long)a * (j + 1) + b <= (1LL << j);
      if (b >= a - 1) { cnt++; if (w != pred) { bad++; if (bad <= 5) printf("  MISMATCH nice k=%d (%d,%d;%d) win=%d\n", K, a, b, j, w); } }
      else { // non-nice: one-lie weight bound is only necessary; report how often win != (w<=2^j)
        nn++; if (w && !pred) { bad++; printf("  IMPOSSIBLE win above volume bound (%d,%d;%d)\n", a, b, j); }
        if (w != pred) nnbad++;
      }
    }
    printf("wide k=%d J=%d M=%d nice=%lld mismatches=%lld | non-nice=%lld with win!=[w<=2^j]: %lld | nodes=%lld\n", K, J, M, cnt, bad, nn, nnbad, nodes);
    return bad ? 1 : 0;
  }
  if (mode == "root") {
    int N = atoi(argv[3]); int bad = 0;
    for (int n = 2; n <= N; n++) {
      int q = 0; for (q = 0; q <= 40; q++) if (win(mk(n, 0, q), q)) break;
      if (q != P(n)) { bad++; printf("  Q_%d(%d)=%d but P=%lld\n", K, n, q, P(n)); }
    }
    printf("root k=%d n=2..%d  Q_k(n)!=P(n) count: %d  nodes=%lld\n", K, N, bad, nodes);
    return bad ? 1 : 0;
  }
  if (mode == "noprune") {
    int J = atoi(argv[3]), M = atoi(argv[4]);
    vector<char> r1; long long diff = 0, cnt = 0;
    for (int pass = 0; pass < 2; pass++) {
      PRUNE = (pass == 0); for (auto& mm : memo) mm.clear(); size_t idx = 0;
      for (int j = 0; j <= J; j++) for (int a = 0; a <= M; a++) for (int b = 0; a + b <= M; b++) {
        if (a + b < 1) continue;
        bool w = win(mk(a, b, j), j);
        if (pass == 0) r1.push_back(w); else { cnt++; if (r1[idx] != (char)w) { diff++; printf("  DIFF (%d,%d;%d) pruned=%d unpruned=%d\n", a, b, j, r1[idx], w); } idx++; }
      }
    }
    printf("noprune k=%d J=%d M=%d states=%lld diffs=%lld\n", K, J, M, cnt, diff);
    return diff ? 1 : 0;
  }
  if (mode == "trans") {
    int K2 = atoi(argv[3]), J = atoi(argv[4]);
    // BFS over all states reachable from (a,b;J), a+b<=7, under k=K; compare step() under K and K2 for every question
    set<pair<int, St>> seen; deque<pair<int, St>> dq; long long cmp = 0, diff = 0;
    for (int a = 0; a <= 7; a++) for (int b = 0; a + b <= 7; b++) if (a + b >= 2) { auto p = make_pair(J, mk(a, b, J)); if (seen.insert(p).second) dq.push_back(p); }
    while (!dq.empty()) {
      auto [j, c] = dq.front(); dq.pop_front();
      if (j == 0) continue;
      vector<int> s(j + 1, 0);
      while (true) {
        for (int ans = 0; ans < 2; ans++) {
          St x = step(c, s, j, ans, K), y = step(c, s, j, ans, K2);
          cmp++; if (x != y) { diff++; if (diff <= 3) printf("  TRANS DIFF at j=%d\n", j); }
          if (total(x) >= 2) { auto p = make_pair(j - 1, x); if (seen.insert(p).second) dq.push_back(p); }
        }
        int r = 0; while (r <= j) { if (s[r] < c[r]) { s[r]++; break; } s[r] = 0; r++; }
        if (r > j) break;
      }
    }
    printf("trans k=%d vs k=%d from j=%d: states=%zu comparisons=%lld diffs=%lld\n", K, K2, J, seen.size(), cmp, diff);
    return diff ? 1 : 0;
  }
  return 3;
}
