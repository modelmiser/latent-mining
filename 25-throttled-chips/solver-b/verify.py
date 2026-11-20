#!/usr/bin/env python3
"""Checks witnesses.txt with the slow direct implementation (slow.py).  For 'LOSE ... | AUTO' it finds a
surviving line itself, prints it, and re-plays it step by step with an independent replay function."""
import sys
from slow import wins, survivor
def replay(lst, h, line):
    """True iff the given Firer line is legal and keeps every vertex <= h-1 after every step."""
    c = {}
    if len(line) != len(lst): return False
    for a, mv in zip(lst, line):
        c[a] = c.get(a, 0) + 1
        if c[a] >= h: return False
        if mv != 'P':
            x = int(mv)
            if c.get(x, 0) < 2: return False
            c[x] -= 2; c[x-1] = c.get(x-1, 0) + 1; c[x+1] = c.get(x+1, 0) + 1
            if max(c[x-1], c[x+1]) >= h: return False
    return True
bad = 0
for raw in open(sys.argv[1] if len(sys.argv) > 1 else 'witnesses.txt'):
    s = raw.strip()
    if not s or s.startswith('#'): continue
    expectfail = s.startswith('EXPECTFAIL'); 
    if expectfail: s = s[len('EXPECTFAIL'):].strip()
    kind, rest = s.split(None, 1)
    if kind == 'WIN':
        v = [int(x) for x in rest.split()]; h, lst = v[0], v[1:]
        ok = wins(lst, h); extra = ''
    else:
        left, line = rest.split('|'); v = [int(x) for x in left.split()]; h, lst = v[0], v[1:]
        line = line.split()
        if line == ['AUTO']:
            line = survivor(lst, h)
            extra = ' line found: ' + ' '.join(map(str, line)) if line else ' no line found'
            ok = line is not None and replay(lst, h, [str(x) for x in line])
        else:
            extra = ''; ok = replay(lst, h, line)
    status = 'OK' if ok else 'MISMATCH'
    if expectfail: status += ' (expected MISMATCH)' if not ok else ' (ERROR: wrong object accepted)'
    if ok == expectfail: bad += 1
    print('%-9s %s h=%d m=%d list=%s%s' % (status, kind, h, len(lst), ' '.join(map(str, lst)), extra))
print('verify: %s' % ('ALL AS EXPECTED' if bad == 0 else '%d PROBLEMS' % bad))
