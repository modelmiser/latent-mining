#!/usr/bin/env python3
"""Referee's own witness checker.  Alive sets are derived from the definition: for each x the lie set
L_x (forced by (i)) is rebuilt from the whole answer history and (ii) is tested pairwise against ell_s,
each ell_s having been computed from C_{s-1}.  No obligation abstraction is used for strategies.
  chk.py a FILE n k q [mut]      solver-a tree format
  chk.py b FILE [k=K] [q=Q] [n=N] [mut]   solver-b tree format
  chk.py pol FILE [mut]          referee policy format (ms policy output)
  chk.py trap FILE [k=K] [drop=i]  solver-b trap set (multiset level; own enumeration of questions)
  chk.py traplit FILE seed trials len   random literal plays against the trap responder
mut = flip membership of element 1 in the root question (deliberately broken witness)."""
import sys, random, itertools

def alive_set(n, k, hist, ells):
    """hist: list of (S, ans). Literal validity of (x, L_x) for each x."""
    C = set()
    t = len(hist)
    for x in range(1, n + 1):
        L = [s for s in range(1, t + 1) if (x in hist[s - 1][0]) != hist[s - 1][1]]
        ok = True
        for i, s in enumerate(L):
            lim = s + k * ells[s - 1]
            for u in L[i + 1:]:
                if u <= lim: ok = False; break
            if not ok: break
        if ok: C.add(x)
    return C

def obligations(n, k, hist, ells, C):
    t = len(hist); r = {}
    for x in C:
        D = 0
        for s in range(1, t + 1):
            if (x in hist[s - 1][0]) != hist[s - 1][1]: D = max(D, s + k * ells[s - 1])
        r[x] = max(0, D - t)
    return r

class Reject(Exception): pass

def verify(n, k, q, ask):
    stats = dict(leaves=0, maxd=0, nodes=0)
    def rec(path, hist, ells, C):
        stats['nodes'] += 1
        if len(C) == 1:
            stats['leaves'] += 1; stats['maxd'] = max(stats['maxd'], len(hist)); return
        if len(hist) >= q: raise Reject("|C|=%d after %d questions, path %s" % (len(C), len(hist), path))
        S = ask(path, hist, ells, C)
        ell = min(len(S & C), len(C - S))
        legal = 0
        for ans in (True, False):
            h2 = hist + [(S, ans)]; e2 = ells + [ell]
            C2 = alive_set(n, k, h2, e2)
            if not C2: continue
            legal += 1
            rec(path + ('Y' if ans else 'N'), h2, e2, C2)
        if legal == 0: raise Reject("no legal answer?! path %s" % path)
    rec('', [], [], set(range(1, n + 1)))
    return stats

def main():
    mode = sys.argv[1]; fn = sys.argv[2]
    opts = dict(a.split('=') for a in sys.argv[3:] if '=' in a)
    mut = 'mut' in sys.argv
    try:
        if mode == 'a':
            n, k, q = int(sys.argv[3]), int(sys.argv[4]), int(sys.argv[5])
            Q = {}
            for line in open(fn):
                if line.startswith('#') or not line.strip(): continue
                p = line.split()
                if p[1] == '?': Q['' if p[0] == '-' else p[0]] = frozenset(int(v) for v in p[2:])
            if mut: Q[''] = Q[''] ^ frozenset([1])
            def ask(path, hist, ells, C):
                if path not in Q: raise Reject("no question at path '%s'" % path)
                return Q[path]
        elif mode == 'b':
            Q = {}; hdr = None
            for line in open(fn):
                line = line.strip()
                if not line or line.startswith('#'): continue
                if hdr is None: p = line.split(); hdr = (int(p[1]), int(p[3]), int(p[5])); continue
                a, _, b = line.partition(':'); a = a.strip()
                Q['' if a == '-' else a.replace('1', 'Y').replace('0', 'N')] = frozenset(int(v) for v in b.split())
            k, n, q = hdr
            k = int(opts.get('k', k)); q = int(opts.get('q', q)); n = int(opts.get('n', n))
            if mut: Q[''] = Q[''] ^ frozenset([1])
            def ask(path, hist, ells, C):
                if path not in Q: raise Reject("no question at path '%s'" % path)
                return Q[path]
        elif mode == 'pol':
            pol = {}; hdr = None
            for line in (sys.stdin if fn == '-' else open(fn)):
                if line.startswith('# k'): p = line.split(); hdr = (int(p[2]), int(p[4]), int(p[6])); continue
                if line.startswith('#') or not line.strip(): continue
                j, c, a = line.split(':')
                pol[(int(j), tuple(int(v) for v in c.split()))] = tuple(int(v) for v in a.split())
            k, n, q = hdr
            k = int(opts.get('k', k)); q = int(opts.get('q', q))
            if mut:
                key = (q, tuple([n] + [0] * q)); a = list(pol[key]); a[0] = a[0] // 2; pol[key] = tuple(a)
            def ask(path, hist, ells, C):
                j = q - len(hist)
                r = obligations(n, k, hist, ells, C)
                cls = {}
                for x in sorted(C): cls.setdefault(min(r[x], j), []).append(x)
                c = tuple(len(cls.get(i, [])) for i in range(j + 1))
                if (j, c) not in pol: raise Reject("policy has no entry for j=%d state %s" % (j, c))
                a = pol[(j, c)]; S = set()
                for i in range(j + 1): S |= set(cls.get(i, [])[:a[i]])
                return frozenset(S)
        elif mode in ('trap', 'traplit'):
            lines = [l.split() for l in open(fn) if l.strip() and not l.startswith('#')]
            k = int(lines[0][1]); n = int(lines[0][3])
            k = int(opts.get('k', k))
            T = [tuple(sorted(int(v) for v in l)) for l in lines[1:]]
            if 'drop' in opts: del T[int(opts['drop'])]
            T = set(T)
            if mode == 'trap':
                if tuple([0] * n) not in T: raise Reject("root not in trap")
                for st in T:
                    if len(st) < 2: raise Reject("state with <2 alive in trap")
                    vals = sorted(set(st)); cnt = [st.count(v) for v in vals]
                    for a in itertools.product(*[range(c + 1) for c in cnt]):
                        ins = sum(a); ell = min(ins, len(st) - ins)
                        stay = False
                        for ans in (True, False):
                            new = []
                            for v, c, ai in zip(vals, cnt, a):
                                tr = ai if ans else c - ai
                                new += [max(v - 1, 0)] * tr
                                if v == 0: new += [k * ell] * (c - tr)
                            if new and tuple(sorted(new)) in T: stay = True
                        if not stay: raise Reject("state %s question %s escapes" % (st, a))
                print("ACCEPT trap %s k=%d n=%d states=%d" % (fn, k, n, len(T))); return
            else:
                rng = random.Random(int(sys.argv[3])); trials = int(sys.argv[4]); ln = int(sys.argv[5])
                for _ in range(trials):
                    hist = []; ells = []; C = set(range(1, n + 1))
                    for t in range(ln):
                        m = rng.random()
                        if m < 0.1: S = frozenset(C) if rng.random() < .5 else frozenset()
                        elif m < 0.4: S = frozenset(rng.sample(sorted(C), rng.randint(1, len(C) - 1)))
                        else: S = frozenset(x for x in range(1, n + 1) if rng.random() < .5)
                        ell = min(len(S & C), len(C - S)); done = False
                        for ans in (True, False):
                            h2 = hist + [(S, ans)]; e2 = ells + [ell]
                            C2 = alive_set(n, k, h2, e2)
                            r = obligations(n, k, h2, e2, C2)
                            if tuple(sorted(r.values())) in T:
                                hist, ells, C = h2, e2, C2; done = True; break
                        if not done: raise Reject("literal play left the trap at t=%d" % (t + 1))
                        if len(C) < 2: raise Reject("literal |C|<2")
                print("ACCEPT traplit %s k=%d n=%d: %d random plays of %d questions, literal |C|>=2 throughout" % (fn, k, n, trials, ln)); return
        st = verify(n, k, q, ask)
        print("ACCEPT %s n=%d k=%d q=%d leaves=%d maxdepth=%d nodes=%d" % (fn, n, k, q, st['leaves'], st['maxd'], st['nodes']))
    except Reject as e:
        print("REJECT %s: %s" % (fn, e)); sys.exit(2)

main()
