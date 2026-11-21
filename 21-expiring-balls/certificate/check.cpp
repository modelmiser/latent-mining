// check.cpp -- checker for a compressed certificate that the Rule keeps every load below L on n bins
// against first-in-first-out expiry.  Written from the game definition (chain/3-question.txt); shares no
// code with solver-b/back2.cpp or verifier/vb.cpp.  The soundness argument is PROOF.md.
//
// usage: check MODE n L certfile [part parts]
//   MODE = min    the reduced check of PROOF.md (families F1, F2, F3)
//          full   the unreduced check over every state of A (PROOF.md, section 5)
//          brute  the literal check of properties (1)-(4) over every state (small n only)
//          count  no certificate needed (give - ): counts all states, for the completeness test of older()
//   part/parts: split the loop over certificate lines between processes (line index mod parts == part).
//               The seed family F3 is checked by part 0 only.  All parts must print ACCEPT.
//   env NOPRUNE=1 switches off the two prunings (P1, P2 of PROOF.md); for cross-checking on small n.
//
// Certificate file: one state per line; balls NEWEST first; each ball two hex digits "wo" = receiving bin,
// other offered bin; bins numbered 0,1,2,... in order of first appearance reading the line left to right.
// A line "-" is the empty state.  The file is a set C of states.  Meaning:
//   A = { non-final states that have some line of C as a prefix }   (prefix = its j newest balls, relabelled)
//   S = { non-final states not in A }       (non-final: forest on at most n bins, every load <= L-1)
// The checker accepts only if S contains the empty state, is closed under Expire, and for every state of S
// and every legal offer at least one answer stays in S.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <cstdint>
typedef unsigned __int128 Key;
typedef unsigned long long u64;

static int n, L;
static bool noprune = false;

struct St { int k; int w[16], o[16]; };          // k balls, index 0 = newest; w = receiver, o = other bin
struct Info { int m; int ld[16]; int cp[16]; bool forest; int maxld; };   // m = number of bins touched

// ---- the game, re-derived -------------------------------------------------------------------------
static Info analyse(const St& s) {               // loads, components (by relabelling), forest test
  Info I; I.m = 0; I.forest = true; I.maxld = 0; bool seen[16];
  for (int i = 0; i < 16; i++) { I.ld[i] = 0; I.cp[i] = i; seen[i] = false; }
  for (int i = 0; i < s.k; i++) {
    int a = s.w[i], b = s.o[i];
    if (!seen[a]) { seen[a] = true; I.m++; } if (!seen[b]) { seen[b] = true; I.m++; }
    if (++I.ld[a] > I.maxld) I.maxld = I.ld[a];
    int ca = I.cp[a], cb = I.cp[b];
    if (ca == cb) I.forest = false;              // includes a == b
    else for (int t = 0; t < 16; t++) if (I.cp[t] == cb) I.cp[t] = ca;
  }
  return I;
}
static St canon(const St& a) {                    // rename bins by first appearance, newest ball first, receiver first
  int mp[16]; for (int i = 0; i < 16; i++) mp[i] = -1; int nx = 0; St c; c.k = a.k;
  for (int i = 0; i < a.k; i++) {
    if (mp[a.w[i]] < 0) mp[a.w[i]] = nx++; if (mp[a.o[i]] < 0) mp[a.o[i]] = nx++;
    c.w[i] = mp[a.w[i]]; c.o[i] = mp[a.o[i]];
  }
  return c;
}
static Key keyOf(const St& c) {                   // c canonical; a ball byte is never 0 because w != o
  Key x = 0; for (int i = 0; i < c.k; i++) x |= (Key)(c.w[i] | (c.o[i] << 4)) << (8 * i); return x;
}
static St dropNewest(const St& e) { St q; q.k = e.k - 1; for (int i = 1; i < e.k; i++) { q.w[i-1] = e.w[i]; q.o[i-1] = e.o[i]; } return q; }
static St addNewest(const St& q, int w, int o) { St t; t.k = q.k + 1; t.w[0] = w; t.o[0] = o; for (int i = 0; i < q.k; i++) { t.w[i+1] = q.w[i]; t.o[i+1] = q.o[i]; } return t; }

// ---- the certificate as a set ---------------------------------------------------------------------
static std::vector<Key> tab; static u64 tmask; static const Key NONE = ~(Key)0;
static std::vector<St> cert;
static inline u64 mix(Key x) { u64 a = (u64)x, b = (u64)(x >> 64); a ^= b * 0x9E3779B97F4A7C15ULL; a ^= a >> 29; a *= 0xBF58476D1CE4E5B9ULL; a ^= a >> 32; a *= 0x94D049BB133111EBULL; a ^= a >> 31; return a; }
static inline bool has(Key x) { u64 i = mix(x) & tmask; while (tab[i] != NONE) { if (tab[i] == x) return true; i = (i + 1) & tmask; } return false; }
static void put(Key x) { u64 i = mix(x) & tmask; while (tab[i] != NONE) { if (tab[i] == x) return; i = (i + 1) & tmask; } tab[i] = x; }
// membership in A for a non-final state s (any labelling): some prefix of length >= 1 is a certificate line.
// (Length 0 is the empty state, which the checker has already required not to be in C.)
static bool inA(const St& s) {
  Key x = keyOf(canon(s));
  for (int j = 1; j <= s.k; j++) { Key p = j >= 16 ? x : (x & ((((Key)1) << (8 * j)) - 1)); if (has(p)) return true; }
  return false;
}
static void show(const St& s) { if (!s.k) printf("-"); for (int i = 0; i < s.k; i++) printf("%s%x%x", i ? " " : "", s.w[i], s.o[i]); }
static void reject(const char* why, const St* s) { printf("REJECT n=%d L=%d: %s", n, L, why); if (s) { printf(" at state [newest first] "); show(*s); } printf("\n"); exit(1); }

// ---- all ways to add one OLDER ball to a canonical state e, up to renaming of untouched bins ---------
// Each end of the new ball is either a bin already touched (0..m-1) or a bin not touched so far (value m).
// allowFinal: also produce children with a load >= L (only used by mode count).
template <class F> static void older(const St& e, const Info& I, bool allowFinal, F f) {
  for (int a = 0; a <= I.m; a++) for (int b = 0; b <= I.m; b++) {
    bool af = (a == I.m), bf = (b == I.m); int w, o;
    if (af && bf) { if (I.m + 2 > n) continue; w = I.m; o = I.m + 1; }       // two untouched bins
    else if (af || bf) { if (I.m + 1 > n) continue; w = a; o = b; }           // one untouched bin, named m
    else { if (a == b || I.cp[a] == I.cp[b]) continue; w = a; o = b; }        // two touched bins: different components
    if (!allowFinal && I.ld[w] + 1 >= L) continue;
    St c = e; c.w[e.k] = w; c.o[e.k] = o; c.k = e.k + 1; f(c);
  }
}

static u64 nSeeds = 0, nNodes = 0, nTests = 0, nFired = 0;

// The one local test.  e is a non-final state of A with newest ball (0 receives, 1 other).  If the other
// answer to the same offer is also bad (a load L, or a state of A), the state before the offer must be in A.
static void testOffer(const St& e, const Info& I, bool alsoViaA) {
  nTests++;
  bool bad = (I.ld[1] + 1 >= L);                                   // the other answer makes a load L
  if (!bad && alsoViaA) { St f = e; f.w[0] = e.o[0]; f.o[0] = e.w[0]; bad = inA(f); }
  if (!bad) return;
  nFired++;
  St q = dropNewest(e);
  if (!inA(q)) reject("an offer has two bad answers but the state before it is not in A; the state after one answer is", &e);
}

// F3: seeds.  Walk every non-final state none of whose proper prefixes is a seed; each seed met must be in A.
static void walkSeeds(const St& e) {
  Info I = analyse(e);
  for (int a = 0; a < I.m; a++) if (I.ld[a] == L - 1) for (int b = a + 1; b < I.m; b++) if (I.ld[b] == L - 1 && I.cp[a] != I.cp[b]) {
    nSeeds++; if (!inA(e)) reject("a state with two bins of load L-1 in different components is not in A", &e); return; }
  if (!noprune) {                                                   // pruning P1 (PROOF.md)
    int room = n - 2 - e.k, need = 1 << 20, untouched = n - I.m;
    if (untouched >= 2) need = 2 * (L - 1);
    for (int a = 0; a < I.m; a++) {
      if (untouched >= 1 && 2 * (L - 1) - I.ld[a] < need) need = 2 * (L - 1) - I.ld[a];
      for (int b = a + 1; b < I.m; b++) if (I.cp[a] != I.cp[b] && 2 * (L - 1) - I.ld[a] - I.ld[b] < need) need = 2 * (L - 1) - I.ld[a] - I.ld[b];
    }
    if (need > room) return;
  }
  older(e, I, false, [&](const St& c) { walkSeeds(c); });
}
// F1 and F2: from a certificate line, walk its extensions in which bin 1 has not yet reached load L-1.
static void walkMin(const St& e, bool root) {
  Info I = analyse(e); nNodes++;
  if (root) testOffer(e, I, true); else if (I.ld[1] == L - 1) testOffer(e, I, false);
  if (I.ld[1] >= L - 1) return;
  if (!noprune && e.k + (L - 1 - I.ld[1]) > n - 1) return;          // pruning P2 (PROOF.md)
  older(e, I, false, [&](const St& c) { if (!has(keyOf(c))) walkMin(c, false); });
}
// Unreduced: every state of A, each visited from its longest prefix in C.
static void walkFull(const St& e) {
  Info I = analyse(e); nNodes++;
  testOffer(e, I, true);
  older(e, I, false, [&](const St& c) { if (!has(keyOf(c))) walkFull(c); });
}
// Literal: every non-final state q; if q is in S, check Expire and every legal offer directly.
static u64 nAll = 0, nS = 0;
static void walkBrute(const St& q) {
  Info I = analyse(q); nAll++;
  if (!inA(q)) {
    nS++;
    if (q.k) { St r = q; r.k--; if (inA(r)) reject("S not closed under Expire", &q); }
    for (int u = 0; u <= I.m; u++) for (int v = u; v <= I.m; v++) {
      int x, y;
      if (u == I.m && v == I.m) { if (I.m + 2 > n) continue; x = I.m; y = I.m + 1; }
      else if (v == I.m) { if (I.m + 1 > n) continue; x = u; y = I.m; }
      else { if (u == v || I.cp[u] == I.cp[v]) continue; x = u; y = v; }
      bool ok = false;
      for (int side = 0; side < 2 && !ok; side++) {
        int r = side ? y : x, t = side ? x : y;
        if (I.ld[r] + 1 >= L) continue;
        St s = addNewest(q, r, t); if (!inA(s)) ok = true;
      }
      if (!ok) { char why[96]; snprintf(why, sizeof why, "a state of S has no answer in S to the offer {%d,%d}", x, y); reject(why, &q); }
    }
  }
  older(q, I, false, [&](const St& c) { walkBrute(c); });
}
// count: all states with any loads; sum over states of n!/(n-m)! must equal (#labelled forests with k edges) * k! * 2^k
static u64 cntStates[16]; static Key cntOrbit[16];
static void walkCount(const St& e) {
  Info I = analyse(e); cntStates[e.k]++; Key orb = 1; for (int i = 0; i < I.m; i++) orb *= (Key)(n - i); cntOrbit[e.k] += orb;
  older(e, I, true, [&](const St& c) { walkCount(c); });
}

int main(int argc, char** argv) {
  if (argc < 5) { fprintf(stderr, "usage: check min|full|brute|count n L certfile [part parts]\n"); return 2; }
  std::string mode = argv[1]; n = atoi(argv[2]); L = atoi(argv[3]);
  int part = argc > 6 ? atoi(argv[5]) : 0, parts = argc > 6 ? atoi(argv[6]) : 1;
  noprune = getenv("NOPRUNE") != 0;
  if (n < 2 || n > 12 || L < 2) { fprintf(stderr, "need 2 <= n <= 12 and L >= 2\n"); return 2; }
  St empty; empty.k = 0;
  if (mode == "count") {
    walkCount(empty);
    for (int k = 0; k < n; k++) { Key v = cntOrbit[k]; char buf[64]; int p = 63; buf[p] = 0; if (!v) buf[--p] = '0'; while (v) { buf[--p] = '0' + (int)(v % 10); v /= 10; }
      printf("n=%d k=%d states=%llu labelled=%s\n", n, k, cntStates[k], buf + p); }
    return 0;
  }
  // read the certificate
  FILE* fi = fopen(argv[4], "r"); if (!fi) { fprintf(stderr, "cannot open certificate\n"); return 2; }
  char line[256];
  while (fgets(line, sizeof line, fi)) {
    St s; s.k = 0; char* p = line; bool isEmpty = false;
    while (*p) {
      if (*p == ' ' || *p == '\n' || *p == '\r') { p++; continue; }
      if (*p == '-') { isEmpty = true; p++; continue; }
      int d[2];
      for (int j = 0; j < 2; j++) { char ch = p[j]; if (ch >= '0' && ch <= '9') d[j] = ch - '0'; else if (ch >= 'a' && ch <= 'f') d[j] = ch - 'a' + 10; else reject("certificate line is not a list of two-digit balls", 0); }
      if (s.k >= 15) reject("certificate line too long", 0);
      s.w[s.k] = d[0]; s.o[s.k] = d[1]; s.k++; p += 2;
    }
    if (s.k == 0) { if (isEmpty) reject("property (1) fails: the empty state is in C, so it is in A", 0); continue; }   // blank line ignored
    if (isEmpty) reject("certificate line mixes - and balls", 0);
    Info I = analyse(s); St c = canon(s);
    if (!I.forest) reject("certificate line is not a legal state (offer inside one component)", &s);
    if (I.m > n) reject("certificate line uses more than n bins", &s);
    if (I.maxld >= L) reject("certificate line has a load >= L", &s);
    for (int i = 0; i < s.k; i++) if (c.w[i] != s.w[i] || c.o[i] != s.o[i]) reject("certificate line is not in canonical labelling", &s);
    cert.push_back(s);
  }
  fclose(fi);
  u64 size = 16; while (size < 2 * cert.size() + 16) size *= 2; tab.assign(size, NONE); tmask = size - 1;
  for (const St& s : cert) put(keyOf(s));
  // (1) the empty state is not in C: enforced while reading.  (2), (3) hold by the form of S (PROOF.md).
  if (mode == "brute") {
    walkBrute(empty);
    printf("ACCEPT n=%d L=%d mode=brute: lines=%zu nonfinal_states=%llu in_S=%llu in_A=%llu\n", n, L, cert.size(), nAll, nS, nAll - nS);
    return 0;
  }
  if (mode != "min" && mode != "full") { fprintf(stderr, "unknown mode\n"); return 2; }
  if (part == 0) walkSeeds(empty);
  for (size_t i = 0; i < cert.size(); i++) if ((int)(i % parts) == part) { if (mode == "min") walkMin(cert[i], true); else walkFull(cert[i]); }
  printf("ACCEPT n=%d L=%d mode=%s part=%d/%d%s: lines=%zu seeds_checked=%llu states_walked=%llu tests=%llu tests_fired=%llu\n",
         n, L, mode.c_str(), part, parts, noprune ? " NOPRUNE" : "", cert.size(), nSeeds, nNodes, nTests, nFired);
  return 0;
}
