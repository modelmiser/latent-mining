// Second, independent implementation of ADV(P) + exhaustive play against every scheduler.
// usage: play P DELTA [variant: ok|nothreat|fewrounds] [MAXDEV]
// MAXDEV (optional): play only schedulers that follow non-preemptive EDF (run the smallest-deadline legal job;
// if none, idle) except at up to MAXDEV ticks, where they may do anything.  Omitted = every scheduler.
// Same checks as play.py.  Memo: open-addressing set of 128-bit hashes of the full node key
// (time, adversary mode, both plan states, scheduler state, lateness so far).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
typedef unsigned char u8; typedef uint64_t u64;

static int P, DELTA, ROUNDS, L, TMAX, MAXDEV = -1; static string VAR = "ok";
static inline int dK(int rho) { return DELTA + (rho + 1) * P; }
static inline int dU(int rho) { return DELTA + rho * P; }

struct M {                    // machine state: unfinished jobs sorted by (d, rem); locked job; switch flag
    u8 n, Ad, Ar, fl; u8 d[60], r[60];
    void clear() { n = 0; Ad = Ar = fl = 0; }
};
static void fail(const char* msg, int t);
static void add(M& m, int d, int p) {
    if (m.n >= 59) fail("job table overflow", -1);
    int i = m.n++;
    while (i > 0 && (m.d[i-1] > d || (m.d[i-1] == d && m.r[i-1] > p))) { m.d[i] = m.d[i-1]; m.r[i] = m.r[i-1]; --i; }
    m.d[i] = d; m.r[i] = p;
}
static inline bool legal(const M& m, int i) { return m.Ar == 0 || (m.d[i] == m.Ad && m.r[i] == m.Ar) || m.fl; }
// run one unit of job i at tick t; returns lateness of a completion (0 otherwise)
static int run(M& m, int i, int t) {
    if (!legal(m, i)) fail("illegal machine move", t);
    int d = m.d[i], r = m.r[i]; m.fl = 0;
    if (r == 1) {
        for (int j = i; j + 1 < m.n; ++j) { m.d[j] = m.d[j+1]; m.r[j] = m.r[j+1]; }
        --m.n; m.Ad = m.Ar = 0; return max(0, t + 1 - d);
    }
    --r;
    while (i > 0 && m.d[i-1] == d && m.r[i-1] > r) { m.r[i] = m.r[i-1]; --i; }
    m.r[i] = r; m.Ad = d; m.Ar = r; return 0;
}
static bool witness_ok(const M& m, int t) {       // finish locked job, then non-preemptive EDF
    bool skipped = false;
    if (m.Ar) { t += m.Ar; if (t > m.Ad) return false; }
    for (int i = 0; i < m.n; ++i) {
        if (m.Ar && !skipped && m.d[i] == m.Ad && m.r[i] == m.Ar) { skipped = true; continue; }
        t += m.r[i]; if (t > m.d[i]) return false;
    }
    return true;
}
static bool cert(const M& m, int t, int late) {   // Lemma 0 certificate for lateness >= L
    if (late >= L) return true;
    int cum = 0;
    for (int i = 0; i < m.n; ) {
        int d = m.d[i];
        while (i < m.n && m.d[i] == d) cum += m.r[i++];
        if (t + cum - d >= L) return true;
    }
    return false;
}
static bool can_finish(M m, int t, int lim) {     // exact, no memo (used only when the certificate is absent)
    if (m.n == 0) return true;
    int cum = 0;
    for (int i = 0; i < m.n; ++i) { cum += m.r[i]; if (t + cum > m.d[i] + lim) return false; }
    for (int i = 0; i < m.n; ++i) {
        if (i > 0 && m.d[i] == m.d[i-1] && m.r[i] == m.r[i-1]) continue;
        if (!legal(m, i)) continue;
        M x = m; int la = run(x, i, t);
        if (la <= lim && can_finish(x, t + 1, lim)) return true;
    }
    if (m.Ar && !m.fl) { M x = m; x.fl = 1; if (can_finish(x, t + 1, lim)) return true; }
    return false;
}

enum { mA, mR, mW, mP2, mF, mNEXT, mDONE, mFWIN, mCASH0 };
struct Adv { u8 mode, s, aux, has0, has1; signed char rho; M p0, p1; };

struct Stats { u64 nodes, wins_fork, wins_cash0, wins_cash1, early, certless; int maxcash, maxrel; } st;
static vector<string> path;
static void fail(const char* msg, int t) {
    printf("P=%d DELTA=%d T=%d variant=%s : FAIL: %s (t=%d)\n  path:", P, DELTA, TMAX, VAR.c_str(), msg, t);
    for (auto& s : path) printf(" %s", s.c_str());
    printf("\n"); exit(1);
}
static int units_min(const M& m, int kd) { for (int i = 0; i < m.n; ++i) if (m.d[i] != kd) return i; return -1; }

static int relbuf_d[64], relbuf_p[64], nrel;
static void cash(Adv& a, M plan, int Dstar, int t, int which) {
    int pend = 0; for (int i = 0; i < plan.n; ++i) if (plan.d[i] <= Dstar) pend += plan.r[i];
    int f = Dstar - t - pend; if (f < 0) fail("PROMISE: no room for cash-in", t);
    for (int k = 0; k < f / P; ++k) { relbuf_d[nrel] = Dstar; relbuf_p[nrel++] = P; add(plan, Dstar, P); }
    if (f % P) { relbuf_d[nrel] = Dstar; relbuf_p[nrel++] = f % P; add(plan, Dstar, f % P); }
    st.maxcash = max(st.maxcash, t + pend);
    a.mode = mDONE; a.rho = which; a.s = t; a.aux = 0; a.has0 = (which == 0); a.has1 = (which == 1);
    if (which == 0) a.p0 = plan; else a.p1 = plan;
}
static void start_round_or_cash(Adv& a, int rho, int t) {   // reference plan is a.p1
    if (a.p1.Ar) fail("reference plan locked at round start", t);
    if (rho == ROUNDS) { cash(a, a.p1, DELTA + ROUNDS * P, t, 1); return; }
    relbuf_d[nrel] = dK(rho); relbuf_p[nrel++] = P;
    add(a.p1, dK(rho), P); a.p0 = a.p1;
    a.mode = mA; a.rho = rho; a.s = t; a.aux = 0; a.has0 = a.has1 = 1;
}
static void adv_release(Adv& a, int t) {
    if ((a.mode == mW && t >= a.aux) || a.mode == mNEXT) {
        start_round_or_cash(a, a.rho + 1, t);
        if (a.mode == mDONE) return;
    }
    if (a.mode == mA || a.mode == mR) {
        if (units_min(a.p0, dK(a.rho)) < 0) {
            relbuf_d[nrel] = dU(a.rho); relbuf_p[nrel++] = 1; add(a.p0, dU(a.rho), 1); add(a.p1, dU(a.rho), 1);
        }
    } else if (a.mode == mP2) {
        if (a.p1.n == 0) { cash(a, a.p1, dK(a.rho), t, 1); return; }
    } else if (a.mode == mF) {
        if ((a.aux == 0 && VAR != "nothreat") || a.p1.n == 0) {
            relbuf_d[nrel] = t + 1; relbuf_p[nrel++] = 1; add(a.p1, t + 1, 1);
        }
    }
}
// kind: 0 inner, 1 decoy, 2 empty
static void adv_observe(Adv& a, int t, int kind, bool kdone) {
    int kd = dK(a.rho);
    if (a.has0) {
        int c = units_min(a.p0, kd); if (c < 0) fail("plan0 has no inner unit", t);
        if (run(a.p0, c, t)) fail("PROMISE: plan0 late", t);
    }
    int kc = -1;
    for (int i = 0; i < a.p1.n; ++i)
        if (a.p1.d[i] == kd && (a.p1.r[i] == P || (a.p1.Ar && a.p1.Ad == kd && a.p1.Ar == a.p1.r[i]))) kc = i;
    if (kc >= 0 && (a.mode == mA || a.mode == mR || a.mode == mW)) {
        if (run(a.p1, kc, t)) fail("PROMISE: plan1 late", t);
    } else {
        if (a.p1.n == 0) { if (a.mode != mW) fail("plan1 idle", t); a.p1.fl = 0; }
        else if (run(a.p1, 0, t)) fail("PROMISE: plan1 late", t);
    }
    int s = a.s;
    switch (a.mode) {
    case mA:
        if (kind == 0) { if (t + 1 == s + P) { a.mode = mP2; a.has0 = 0; } }
        else if (kind == 2) { a.mode = mW; a.aux = s + P; a.has0 = 0; }
        else a.mode = mR;
        break;
    case mR:
        if (kind == 1) { if (kdone) { a.mode = mCASH0; a.has1 = 0; } }
        else if (kind == 2) { a.mode = mW; a.aux = max(t + 1, s + P); a.has0 = 0; }
        else fail("inner while locked on decoy", t);
        break;
    case mW: break;
    case mP2:
        if (kind == 2) a.mode = mNEXT;
        else if (kind == 1) { a.mode = kdone ? mFWIN : mF; a.aux = 0; }
        break;
    case mF:
        if (kind == 1) { if (kdone) a.mode = mFWIN; else a.aux = 1; }
        else if (kind == 2) a.mode = mNEXT;
        else fail("inner during fork", t);
        break;
    default: fail("bad mode", t);
    }
}

// ---------------- 128-bit hash set
struct HSet {
    vector<u64> a, b; u64 mask, cnt;
    HSet() { a.assign(1 << 20, 0); b.assign(1 << 20, 0); mask = (1 << 20) - 1; cnt = 0; }
    bool insert(u64 x, u64 y) {             // true if new
        if (x == 0 && y == 0) y = 1;
        if (cnt * 10 > (mask + 1) * 6) grow();
        u64 i = x & mask;
        while (a[i] || b[i]) { if (a[i] == x && b[i] == y) return false; i = (i + 1) & mask; }
        a[i] = x; b[i] = y; ++cnt; return true;
    }
    void grow() {
        vector<u64> oa, ob; oa.swap(a); ob.swap(b);
        u64 n = (mask + 1) * 2; a.assign(n, 0); b.assign(n, 0); mask = n - 1;
        for (u64 j = 0; j < oa.size(); ++j) if (oa[j] || ob[j]) {
            u64 i = oa[j] & mask; while (a[i] || b[i]) i = (i + 1) & mask; a[i] = oa[j]; b[i] = ob[j];
        }
    }
} seen;
static inline void ser(const M& m, u8*& p) {
    *p++ = m.n; *p++ = m.Ad; *p++ = m.Ar; *p++ = m.fl;
    memcpy(p, m.d, m.n); p += m.n; memcpy(p, m.r, m.n); p += m.n;
}
static bool visit(int t, const Adv& a, const M& on, int late, int dev) {
    u8 buf[512]; u8* p = buf; *p++ = dev;
    *p++ = t; *p++ = a.mode; *p++ = (u8)a.rho; *p++ = a.s; *p++ = a.aux; *p++ = a.has0; *p++ = a.has1; *p++ = min(late, L);
    if (a.has0) ser(a.p0, p); if (a.has1) ser(a.p1, p); ser(on, p);
    u64 h1 = 1469598103934665603ULL, h2 = 0x9E3779B97F4A7C15ULL;
    for (u8* q = buf; q < p; ++q) {
        h1 = (h1 ^ *q) * 1099511628211ULL;
        h2 ^= *q + 0x9E3779B97F4A7C15ULL + (h2 << 6) + (h2 >> 2); h2 *= 0xff51afd7ed558ccdULL; h2 ^= h2 >> 29;
    }
    return seen.insert(h1, h2);
}
static void terminal(const M& on, int t, int late, const char* what) {
    if (!cert(on, t, late)) {
        if (late <= L - 1 && can_finish(on, t, L - 1)) fail(what, t);
        ++st.certless;
    }
}
static void explore(int t, Adv a, M on, int late, int dev) {
    if (!visit(t, a, on, late, dev)) return;
    ++st.nodes;
    if (a.mode == mFWIN) { ++st.wins_fork; terminal(on, t, late, "SCHEDULER SURVIVES at fork terminal"); return; }
    nrel = 0;
    if (a.mode == mCASH0) cash(a, a.p0, dU(a.rho), t, 0); else adv_release(a, t);
    if (nrel) {
        st.maxrel = max(st.maxrel, t);
        for (int k = 0; k < nrel; ++k) {
            int d = relbuf_d[k], p = relbuf_p[k];
            if (!(1 <= p && p <= P && t + p <= d && d <= TMAX && t < TMAX)) fail("ILLEGAL JOB", t);
            add(on, d, p);
        }
        if (a.has0 && !witness_ok(a.p0, t)) fail("PROMISE: plan0 has no valid completion", t);
        if (a.has1 && !witness_ok(a.p1, t)) fail("PROMISE: plan1 has no valid completion", t);
    }
    if (a.mode == mDONE) { if (a.rho == 0) ++st.wins_cash0; else ++st.wins_cash1;
                           terminal(on, t, late, "SCHEDULER SURVIVES at cash terminal"); return; }
    if (cert(on, t, late)) { ++st.early; return; }
    int kd = dK(a.rho); char b[40];
    int deflt = -1;                               // default action of the restricted scheduler class
    for (int i = 0; i < on.n && deflt < 0; ++i) if (legal(on, i)) deflt = i;
    for (int i = 0; i < on.n; ++i) {
        if (i > 0 && on.d[i] == on.d[i-1] && on.r[i] == on.r[i-1]) continue;
        if (!legal(on, i)) continue;
        int cost = (MAXDEV >= 0 && i != deflt) ? 1 : 0;
        if (MAXDEV >= 0 && dev + cost > MAXDEV) continue;
        M x = on; Adv na = a;
        int kind = (on.d[i] == kd && a.mode != mW) ? 1 : 0; bool kdone = kind == 1 && on.r[i] == 1;
        int la = run(x, i, t);
        adv_observe(na, t, a.mode == mW ? 0 : kind, a.mode == mW ? false : kdone);
        snprintf(b, sizeof b, "%d:run(d%d,r%d)", t, on.d[i], on.r[i]); path.push_back(b);
        explore(t + 1, na, x, max(late, la), dev + cost);
        path.pop_back();
    }
    for (int sw = 0; sw < 2; ++sw) {
        if (sw && !on.Ar) continue;
        int cost = (MAXDEV >= 0 && !(deflt < 0 && sw == 0)) ? 1 : 0;
        if (MAXDEV >= 0 && dev + cost > MAXDEV) continue;
        M x = on; x.fl = sw; Adv na = a;
        adv_observe(na, t, 2, false);
        snprintf(b, sizeof b, "%d:%s", t, sw ? "switch" : "idle"); path.push_back(b);
        explore(t + 1, na, x, late, dev + cost);
        path.pop_back();
    }
}
int main(int argc, char** argv) {
    P = atoi(argv[1]); DELTA = atoi(argv[2]); if (argc > 3) VAR = argv[3]; if (argc > 4) MAXDEV = atoi(argv[4]);
    ROUNDS = (VAR == "fewrounds") ? P - 2 : P - 1; L = P - 1; TMAX = DELTA + ROUNDS * P;
    if (TMAX > 250) { printf("T too large for byte encoding\n"); return 2; }
    Adv a; memset(&a, 0, sizeof a); a.mode = mNEXT; a.rho = -1; a.has1 = 1; a.p1.clear(); a.p0.clear();
    M on; on.clear();
    explore(0, a, on, 0, 0);
    if (MAXDEV < 0) printf("P=%d DELTA=%d T=%d variant=%s : ADVERSARY WINS against every scheduler (lateness >= %d)\n", P, DELTA, TMAX, VAR.c_str(), L);
    else printf("P=%d DELTA=%d T=%d variant=%s : ADVERSARY WINS against every scheduler with <= %d deviations from NP-EDF (lateness >= %d)\n", P, DELTA, TMAX, VAR.c_str(), MAXDEV, L);
    printf("   nodes=%llu maxcash=%d maxrel=%d wins_fork=%llu wins_cash0=%llu wins_cash1=%llu early=%llu certless=%llu\n",
           (unsigned long long)st.nodes, st.maxcash, st.maxrel, (unsigned long long)st.wins_fork, (unsigned long long)st.wins_cash0,
           (unsigned long long)st.wins_cash1, (unsigned long long)st.early, (unsigned long long)st.certless);
    return 0;
}
