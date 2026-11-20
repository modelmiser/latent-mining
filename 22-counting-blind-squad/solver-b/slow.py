#!/usr/bin/env python3
"""Slow, obvious reference implementation of the definition.
States: 0=Q, 1=G, 2=F, 3.. = unnamed.  Rule d[(x,k)] for x != F, k in 0,1,2; d[(Q,0)] = Q.
run(d, n) -> firing time t >= 1, or None (fails: partial fire, or never fires = configuration repeats).
Usage:
  slow.py brute S NMAX     exhaustive over all rules on S states (S=3,4 feasible); prints L(S), counts
  slow.py check FILE       check a witness file (see format in witness files)
"""
import sys, itertools

Q, G, F = 0, 1, 2

def run(d, n):
    cfg = tuple([G] + [Q] * (n - 1))
    seen = {cfg}
    t = 0
    while True:
        nxt = []
        for i, x in enumerate(cfg):
            k = 0
            if i > 0 and cfg[i - 1] != x: k += 1
            if i < n - 1 and cfg[i + 1] != x: k += 1
            nxt.append(d[(x, k)])
        t += 1
        cfg = tuple(nxt)
        nf = cfg.count(F)
        if nf == n: return t
        if nf > 0: return None
        if cfg in seen: return None
        seen.add(cfg)

def keys(s):
    ks = []
    for x in range(s):
        if x == F: continue
        for k in range(3):
            if (x, k) == (Q, 0): continue
            ks.append((x, k))
    return ks

def brute(s, nmax):
    ks = keys(s)
    best = 1; bestrules = []; hist = {}
    for vals in itertools.product(range(s), repeat=len(ks)):
        d = dict(zip(ks, vals)); d[(Q, 0)] = Q
        N = 1
        for n in range(2, nmax + 1):
            if run(d, n) is None: break
            N = n
        hist[N] = hist.get(N, 0) + 1
        if N > best: best = N; bestrules = []
        if N == best and len(bestrules) < 5: bestrules.append(vals)
    print("s=%d rules=%d L=%d%s" % (s, s ** len(ks), best, " (hit NMAX!)" if best == nmax else ""))
    print("histogram of max-prefix N (1 = fails n=2):", sorted(hist.items()))
    print("key order:", ks)
    for v in bestrules: print("example:", v)

def parse(fn):
    """witness format: lines 'x k v' with state names Q G F A B C ...; lines 'n t' under 'TIMES'."""
    names = {}
    def st(a):
        if a == 'Q': return Q
        if a == 'G': return G
        if a == 'F': return F
        return 3 + ord(a) - ord('A')
    d = {(Q, 0): Q}; times = {}; mode = 'rule'
    for line in open(fn):
        line = line.split('#')[0].split()
        if not line: continue
        if line[0] == 'TIMES': mode = 'times'; continue
        if mode == 'rule':
            d[(st(line[0]), int(line[1]))] = st(line[2])
        else:
            times[int(line[0])] = None if line[1] == 'FAIL' else int(line[1])
    return d, times

def check(fn):
    d, times = parse(fn)
    assert d[(Q, 0)] == Q
    ok = True
    for n, t in sorted(times.items()):
        r = run(d, n)
        print("n=%d claimed=%s got=%s %s" % (n, t, r, "ok" if r == t else "MISMATCH"))
        ok &= (r == t)
    print("WITNESS OK" if ok else "WITNESS REJECTED")
    return ok

if __name__ == "__main__":
    if sys.argv[1] == 'brute': brute(int(sys.argv[2]), int(sys.argv[3]))
    else: sys.exit(0 if check(sys.argv[2]) else 1)
