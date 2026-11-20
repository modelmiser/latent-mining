#!/usr/bin/env python3
"""family.py -- the "weight" protocol P_S for a set S of positive integers (1 in S, K = max S).
States: +u and -u for u in S, plus two weightless states wa, wb.  qA = +K, qB = -K.
Output: +u and wa say A; -u and wb say B.
Rules (both orders):
  (+u,-v): u=v -> (wa,wb);  u>v -> first available of: (+(u-v), wa) if u-v in S; (+p,+q) with p+q=u-v, p,q in S;
           (+(u-c), -(v-c)) with 0<c<v, u-c and v-c in S.     u<v: mirror image.
  (+u, wb), u>=2 -> (+p,+q) with p+q=u, p,q in S.   (-u, wa): mirror image.     everything else: identity.
usage: family.py [--oneweak] u1 u2 ...   (--oneweak: one weightless state n with output A instead of wa, wb;
       only -tokens split, on n; 2|S|+1 states)   prints one protocol line (format of check.cpp / slow.py); exits 1 if S is not closed."""
import sys
def closed_info(S):
    """returns (ok, net, split): net[(u,v)] for u>v, split[u] for u>=2"""
    Ss = set(S); net = {}; split = {}
    for u in S:
        if u >= 2:
            c = [(p, u - p) for p in sorted(S, reverse=True) if p < u and (u - p) in Ss and p >= u - p]
            if not c: return False, ('split', u), None
            split[u] = c[-1]   # most balanced split
        for v in S:
            if v >= u: continue
            D = u - v
            if D in Ss: net[(u, v)] = ('pos', D, 0); continue
            c = [(p, D - p) for p in sorted(S, reverse=True) if p < D and (D - p) in Ss and p >= D - p]
            if c: net[(u, v)] = ('pos', c[0][0], c[0][1]); continue
            c = [cc for cc in range(1, v) if (u - cc) in Ss and (v - cc) in Ss]
            if c: net[(u, v)] = ('part', u - c[-1], v - c[-1]); continue
            return False, ('net', u, v), None
    return True, net, split
def build(S, oneweak=False):
    """oneweak=True: the variant with a single weightless state n (output A): wb is identified with wa, +tokens never split."""
    S = sorted(S, reverse=True); K = S[0]
    ok, net, split = closed_info(S)
    if not ok: raise ValueError('S not closed: %r' % (net,))
    names = [('+', K), ('-', K)] + [(sg, u) for u in S[1:] for sg in '+-'] + (['wa'] if oneweak else ['wa', 'wb'])
    idx = {nm: i for i, nm in enumerate(names)}
    if oneweak: idx['wb'] = idx['wa']
    d = {}
    def put(x, y, x2, y2):
        d[(idx[x], idx[y])] = (idx[x2], idx[y2]); d[(idx[y], idx[x])] = (idx[y2], idx[x2])
    def tok(sg, u): return (sg, u) if u > 0 else ('wa' if sg == '+' else 'wb')
    for u in S:
        for v in S:
            if u == v: put(('+', u), ('-', v), 'wa', 'wb')
            elif u > v:
                kind, p, q = net[(u, v)]
                for (a, b) in (('+', '-'), ('-', '+')):   # a = sign of the larger token
                    if kind == 'pos': put((a, u), (b, v), tok(a, p), tok(a, q))
                    else: put((a, u), (b, v), (a, p), (b, q))
        if u >= 2:
            p, q = split[u]
            if not oneweak: put(('+', u), 'wb', ('+', p), ('+', q))
            put(('-', u), 'wa', ('-', p), ('-', q))
    w = ''.join('A' if (nm == 'wa' or (nm != 'wb' and nm[0] == '+')) else 'B' for nm in names)
    line = 's=%d qA=0 qB=1 w=%s d: ' % (len(names), w) + ' '.join('%d,%d->%d,%d' % (k + v) for k, v in sorted(d.items()))
    return line, names
if __name__ == '__main__':
    one = '--oneweak' in sys.argv
    S = [int(a) for a in sys.argv[1:] if a != '--oneweak']
    try: line, names = build(S, one)
    except ValueError as e: print(e, file=sys.stderr); sys.exit(1)
    print('# S=%s K=%d states=%d names=%s' % (sorted(S), max(S), len(names), ' '.join(n if isinstance(n, str) else n[0] + str(n[1]) for n in names)))
    print(line)
