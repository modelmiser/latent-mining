// Fast solver: states up to bin relabelling. A state is the age-ordered list of
// balls (receiver w, other endpoint o); bins are relabelled 0,1,2.. in order of first
// appearance in that list (w before o). Bins not touched by any present ball are
// interchangeable, so this is an exact canonical form (the only symmetry is S_n).
// Usage: fast n L mode(0=fifo,1=any-removal) [dumpfile]
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int n, L, MODE;
struct St { int m; unsigned char w[8], o[8]; };
static u64 enc(const St& s) {
    u64 k = (u64)s.m << 58;
    for (int i = 0; i < s.m; i++) k |= (u64)(s.w[i] * 16 + s.o[i]) << (8 * i);
    return k;
}
static St dec(u64 k) {
    St s; s.m = k >> 58;
    for (int i = 0; i < s.m; i++) { int b = (k >> (8 * i)) & 255; s.w[i] = b >> 4; s.o[i] = b & 15; }
    return s;
}
static void canon(St& s) {
    int map[16]; for (int i = 0; i < 16; i++) map[i] = -1; int k = 0;
    for (int i = 0; i < s.m; i++) {
        if (map[s.w[i]] < 0) map[s.w[i]] = k++;
        if (map[s.o[i]] < 0) map[s.o[i]] = k++;
        s.w[i] = map[s.w[i]]; s.o[i] = map[s.o[i]];
    }
}
static int used(const St& s) { int k = 0; for (int i = 0; i < s.m; i++) { k = max(k, (int)s.w[i] + 1); k = max(k, (int)s.o[i] + 1); } return k; }
static int maxload(const St& s) { int l[16] = {0}, b = 0; for (int i = 0; i < s.m; i++) b = max(b, ++l[s.w[i]]); return b; }
// options: each option has 1 successor (removal) or 2 (offer). desc: -1-j for removal of ball j, a*16+b for offer
struct Opt { int cnt; u64 s[2]; int desc; };
static void options(const St& s, vector<Opt>& out) {
    out.clear();
    if (s.m) {
        int lim = MODE ? s.m : 1;
        for (int j = 0; j < lim; j++) {
            St t; t.m = s.m - 1; int q = 0;
            for (int i = 0; i < s.m; i++) if (i != j) { t.w[q] = s.w[i]; t.o[q] = s.o[i]; q++; }
            canon(t); out.push_back({1, {enc(t), 0}, -1 - j});
        }
    }
    int k = used(s);
    int c[16]; for (int i = 0; i < 16; i++) c[i] = i;
    for (int i = 0; i < s.m; i++) { int a = c[s.w[i]], b = c[s.o[i]]; if (a != b) for (int x = 0; x < k; x++) if (c[x] == a) c[x] = b; }
    int top = min(n, k + 2);
    for (int a = 0; a < top; a++) for (int b = a + 1; b < top; b++) {
        if (a < k && b < k && c[a] == c[b]) continue;
        if (a >= k && b != a + 1) continue;       // two fresh bins: only (k,k+1)
        if (a < k && b > k) continue;             // one fresh bin: always label k
        if (a == k && b == k + 1) {
            St t = s; t.w[t.m] = a; t.o[t.m] = b; t.m++;
            u64 e = enc(t); out.push_back({2, {e, e}, a * 16 + b});
        } else {
            St t = s; t.w[t.m] = a; t.o[t.m] = b; t.m++; St t2 = s; t2.w[t2.m] = b; t2.o[t2.m] = a; t2.m++;
            canon(t); canon(t2);
            out.push_back({2, {enc(t), enc(t2)}, a * 16 + b});
        }
    }
}
static string show(u64 k) { St s = dec(k); string r; for (int i = 0; i < s.m; i++) { if (i) r += ' '; r += to_string(s.w[i]) + ":" + to_string(s.o[i]); } return r; }
int main(int argc, char** argv) {
    n = atoi(argv[1]); L = atoi(argv[2]); MODE = atoi(argv[3]);
    unordered_map<u64, int> idx; vector<u64> order;
    idx.reserve(1 << 20);
    St e; e.m = 0; idx[enc(e)] = 0; order.push_back(enc(e));
    vector<unsigned char> win; vector<Opt> opts;
    for (size_t i = 0; i < order.size(); i++) {
        St s = dec(order[i]);
        if (maxload(s) >= L) { win.push_back(1); continue; }
        win.push_back(0);
        options(s, opts);
        for (auto& o : opts) for (int j = 0; j < o.cnt; j++) if (!idx.count(o.s[j])) { idx[o.s[j]] = order.size(); order.push_back(o.s[j]); }
    }
    size_t N = order.size();
    vector<int> rank(N, 0), mv(N, 0);
    // Jacobi-style layering: in sweep t only successors of rank < t count, so rank = least number of
    // Adversary moves needed (terminal states have rank 0). Stops after a sweep with no change.
    bool changed = true; int sweeps = 0;
    while (changed) {
        changed = false; sweeps++;
        for (size_t ii = N; ii-- > 0;) {
            if (win[ii]) continue;
            St s = dec(order[ii]); options(s, opts);
            for (auto& o : opts) {
                int r = 0; bool ok = true;
                for (int j = 0; j < o.cnt; j++) { int t = idx[o.s[j]]; if (!win[t] || rank[t] >= sweeps) { ok = false; break; } r = max(r, rank[t]); }
                if (ok) { win[ii] = 1; rank[ii] = r + 1; mv[ii] = o.desc; changed = true; break; }
            }
        }
    }
    size_t nw = 0; for (size_t i = 0; i < N; i++) nw += win[i];
    printf("n=%d L=%d mode=%s states=%zu adversary_wins=%d winstates=%zu sweeps=%d rank0=%d\n", n, L, MODE ? "any" : "fifo", N, (int)win[0], nw, sweeps, rank[0]);
    if (argc > 4 && string(argv[4]) == "ALL") {   // dump win flag of every state (for cross-check)
        FILE* f = fopen(argv[5], "w");
        for (size_t i = 0; i < N; i++) fprintf(f, "%s|%d\n", show(order[i]).c_str(), (int)win[i]);
        fclose(f);
    } else if (argc > 4) {
        FILE* f = fopen(argv[4], "w");
        vector<char> seen(N, 0); vector<int> q; q.push_back(0); seen[0] = 1;
        if (win[0]) {
            fprintf(f, "# ADVERSARY strategy n=%d L=%d mode=%s. line: rank|move|state ; state = balls oldest first as receiver:other, bins labelled by first appearance; move E=expire, R j=remove ball j, O a b=offer {a,b} (labels >= #used bins are fresh bins); rank 0 = some load >= L\n", n, L, MODE ? "any" : "fifo");
            for (size_t h = 0; h < q.size(); h++) {
                int i = q[h]; St s = dec(order[i]);
                if (maxload(s) >= L) { fprintf(f, "0|T|%s\n", show(order[i]).c_str()); continue; }
                options(s, opts);
                for (auto& o : opts) if (o.desc == mv[i]) {
                    if (o.desc < 0) { if (MODE) fprintf(f, "%d|R %d|%s\n", rank[i], -1 - o.desc, show(order[i]).c_str()); else fprintf(f, "%d|E|%s\n", rank[i], show(order[i]).c_str()); }
                    else fprintf(f, "%d|O %d %d|%s\n", rank[i], o.desc >> 4, o.desc & 15, show(order[i]).c_str());
                    for (int j = 0; j < o.cnt; j++) { int t = idx[o.s[j]]; if (!seen[t]) { seen[t] = 1; q.push_back(t); } }
                }
            }
        } else {
            fprintf(f, "# RULE safe set n=%d L=%d mode=%s. one state per line (balls oldest first as receiver:other, bins labelled by first appearance; empty line = empty state). Claim: contains the empty state, every load < L, closed under removal, and for every legal offer at least one answer stays in the set.\n", n, L, MODE ? "any" : "fifo");
            for (size_t h = 0; h < q.size(); h++) {
                int i = q[h]; St s = dec(order[i]);
                fprintf(f, "%s\n", show(order[i]).c_str());
                options(s, opts);
                for (auto& o : opts) {
                    if (o.cnt == 1) { int t = idx[o.s[0]]; if (!seen[t]) { seen[t] = 1; q.push_back(t); } }
                    else {
                        int t0 = idx[o.s[0]], t1 = idx[o.s[1]];
                        int t = (!win[t0] && seen[t0]) ? t0 : (!win[t1] && seen[t1]) ? t1 : (!win[t0] ? t0 : t1);
                        if (!seen[t]) { seen[t] = 1; q.push_back(t); }
                    }
                }
            }
        }
        fclose(f);
        printf("witness lines=%zu\n", q.size());
    }
    return 0;
}
