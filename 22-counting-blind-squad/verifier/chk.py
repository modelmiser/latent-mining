#!/usr/bin/env python3
"""Referee's witness checker, written from the problem text (not from either solver's code).
Never-fires is decided exactly with a visited set of configurations.
usage: chk.py selftest | chk.py all
"""
import sys, itertools

Q, G, F = 'Q', 'G', 'F'

def run(d, n):
    """d: dict (state,k)->state.  Returns ('SYNC',t) / ('PARTIAL',t) / ('LOOP',t) / ('UNDEF',(x,k))."""
    c = tuple([G] + [Q] * (n - 1))
    seen = {c}
    t = 0
    while True:
        nxt = []
        for i in range(n):
            x = c[i]
            k = 0
            if i - 1 >= 0 and c[i - 1] != x: k += 1
            if i + 1 < n and c[i + 1] != x: k += 1
            if (x, k) not in d: return ('UNDEF', (x, k))
            nxt.append(d[(x, k)])
        t += 1
        nf = nxt.count(F)
        if nf == n: return ('SYNC', t)
        if nf: return ('PARTIAL', t)
        c = tuple(nxt)
        if c in seen: return ('LOOP', t)
        seen.add(c)

def validate_rule(d, states):
    """structure check: total function on (S\\{F}) x {0,1,2}, values in S, d(Q,0)=Q, Q,G,F present."""
    if not {Q, G, F} <= set(states): return 'named states missing'
    if len(set(states)) != len(states): return 'duplicate states'
    for x in states:
        if x == F: continue
        for k in range(3):
            if (x, k) not in d: return 'missing entry %s,%d' % (x, k)
            if d[(x, k)] not in states: return 'value outside S'
    if any(x == F for (x, k) in d): return 'entry defined at F'
    if len(d) != 3 * (len(states) - 1): return 'wrong entry count'
    if d[(Q, 0)] != Q: return 'd(Q,0) != Q'
    return None

def check(d, states, s, claimed_times, fail_n):
    """claimed_times: dict n->t that must cover exactly 2..L contiguous; fail_n = L+1 must fail."""
    if len(states) != s: return 'REJECT: state count %d != %d' % (len(states), s)
    e = validate_rule(d, states)
    if e: return 'REJECT: ' + e
    ns = sorted(claimed_times)
    if not ns or ns != list(range(2, ns[-1] + 1)): return 'REJECT: lengths not contiguous from 2'
    if fail_n != ns[-1] + 1: return 'REJECT: fail length is not L+1'
    for n in ns:
        r = run(d, n)
        if r != ('SYNC', claimed_times[n]): return 'REJECT: n=%d gives %s, claimed %d' % (n, r, claimed_times[n])
    r = run(d, fail_n)
    if r[0] == 'SYNC': return 'REJECT: n=%d synchronises at %d, claimed FAIL' % (fail_n, r[1])
    return 'ACCEPT L=%d (n=%d: %s at %d)' % (ns[-1], fail_n, r[0], r[1])

def from_a(line, s):
    names = [Q, G, F] + ['U%d' % i for i in range(1, s - 2)]
    v = [int(x) for x in line.split()]
    assert len(v) == 3 * (s - 1), 'length'
    d = {}; i = 0
    for x in [Q, G] + names[3:]:
        for k in range(3):
            d[(x, k)] = names[v[i]]; i += 1
    return d, names

def from_b(path):
    d = {(Q, 0): Q}; times = {}; fail = None; states = [Q, G, F]; mode = 0
    for ln in open(path):
        ln = ln.strip()
        if not ln or ln.startswith('#'): continue
        if ln == 'TIMES': mode = 1; continue
        p = ln.split()
        if mode == 0:
            x, k, y = p[0], int(p[1]), p[2]
            assert (x, k) not in d, 'duplicate entry'
            d[(x, k)] = y
            for z in (x, y):
                if z not in states: states.append(z)
        else:
            if p[1] == 'FAIL': fail = int(p[0])
            else: times[int(p[0])] = int(p[1])
    if fail is None: fail = -1
    return d, states, times, fail

BASE = ''
A_WIT = [
    (3, '0 2 1 1 2 2', [1]),
    (4, '0 3 3 0 0 1 2 3 0', [3, 4]),
    (5, '0 3 3 3 1 0 2 4 1 2 1 0', [5, 8, 7, 10]),
    (6, '0 4 3 2 3 0 5 3 0 1 5 0 4 1 3', [8, 10, 10, 19, 21, 20, 20]),
    (7, '0 3 1 6 1 0 5 4 1 2 5 3 4 1 0 3 5 1', [9, 10, 17, 19, 14, 23, 43, 39, 24, 38]),
]

def do_all():
    print('== solver A witnesses (witnesses.txt, transcribed lines) ==')
    for s, line, tm in A_WIT:
        d, st = from_a(line, s)
        times = {n: t for n, t in zip(range(2, 2 + len(tm)), tm)}
        print(' s=%d' % s, check(d, st, s, times, 2 + len(tm)))
    print('== solver B witness files ==')
    for s in (3, 4, 5, 6, 7):
        d, st, times, fail = from_b(BASE + 'solver-b/witness_s%d.txt' % s)
        print(' s=%d' % s, check(d, st, s, times, fail))
    for nm in ('BAD_witness_s5.txt', 'BAD2_witness_s5.txt'):
        d, st, times, fail = from_b(BASE + 'solver-b/' + nm)
        print(' ', nm, check(d, st, 5, times, fail))
    # B's list of all 1244 five-state rules: order Q1 Q2 G0 G1 G2 A0 A1 A2 B0 B1 B2
    print('== solver B all_L5_rules_s5.txt ==')
    seen = set(); bad = 0; tv = set(); nlines = 0
    for ln in open(BASE + 'solver-b/all_L5_rules_s5.txt'):
        p = ln.split()
        if not p or p[0].startswith('#'): continue
        nlines += 1
        ent = list(p[0]) if len(p) == 1 else p[:11]
        keys = [(Q, 1), (Q, 2), (G, 0), (G, 1), (G, 2), ('A', 0), ('A', 1), ('A', 2), ('B', 0), ('B', 1), ('B', 2)]
        d = {(Q, 0): Q}
        for kk, y in zip(keys, ent): d[kk] = y
        st = [Q, G, F, 'A', 'B']
        if validate_rule(d, st): bad += 1; continue
        seen.add(tuple(ent))
        rs = [run(d, n) for n in range(2, 7)]
        if not all(r[0] == 'SYNC' for r in rs[:4]) or rs[4][0] == 'SYNC': bad += 1
        tv.add(tuple(r[1] for r in rs[:4]))
    print('  lines %d distinct %d bad %d distinct time vectors %d min %s max %s' % (nlines, len(seen), bad, len(tv), min(tv), max(tv)))
    if nlines: print('  first line raw:', open(BASE + 'solver-b/all_L5_rules_s5.txt').readline().strip())

def selftest():
    """deliberately broken witnesses: every one must be REJECTED; the unbroken one must be ACCEPTED."""
    s, line, tm = A_WIT[2]
    times = {n: t for n, t in zip(range(2, 6), tm)}
    d, st = from_a(line, s)
    out = []
    out.append(('good witness', check(d, st, s, times, 6), 'ACCEPT'))
    # 1 flip a used entry
    d1 = dict(d); d1[(Q, 1)] = G
    out.append(('entry d(Q,1) changed', check(d1, st, s, times, 6), 'REJECT'))
    # 2 wrong time
    t2 = dict(times); t2[4] = 8
    out.append(('time for n=4 off by one', check(d, st, s, t2, 6), 'REJECT'))
    # 3 claims L=4 (then n=5 must fail but it synchronises)
    t3 = {n: times[n] for n in (2, 3, 4)}
    out.append(('understated L (n=5 claimed FAIL)', check(d, st, s, t3, 5), 'REJECT'))
    # 4 d(Q,0) != Q
    d4 = dict(d); d4[(Q, 0)] = 'U1'
    out.append(('d(Q,0)=U1', check(d4, st, s, times, 6), 'REJECT'))
    # 5 extra state smuggled in (6 states claimed as 5)
    d5, st5 = from_a(A_WIT[3][1], 6)
    out.append(('6-state rule offered as s=5', check(d5, st5, 5, {n: t for n, t in zip(range(2, 9), A_WIT[3][2])}, 9), 'REJECT'))
    # 6 partial fire rule: d(Q,1)=F
    d6 = dict(d); d6[(Q, 1)] = F
    out.append(('d(Q,1)=F', check(d6, st, s, times, 6), 'REJECT'))
    # 7 gap in lengths (skip n=3)
    t7 = {n: times[n] for n in (2, 4, 5)}
    out.append(('n=3 omitted', check(d, st, s, t7, 6), 'REJECT'))
    # 8 rule defined at F
    d8 = dict(d); d8[(F, 0)] = Q
    out.append(('entry at F', check(d8, st, s, times, 6), 'REJECT'))
    # 9 an entry the witness never reads, changed: must still be judged on behaviour (expected ACCEPT if truly unread)
    ok = True
    for nm, res, exp in out:
        good = res.startswith(exp)
        ok &= good
        print(' %-36s -> %-60s %s' % (nm, res, 'as expected' if good else '*** UNEXPECTED ***'))
    print('SELFTEST', 'PASS' if ok else 'FAIL')

if __name__ == '__main__':
    if sys.argv[1] == 'selftest': selftest()
    else: do_all()
