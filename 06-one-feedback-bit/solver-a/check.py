#!/usr/bin/env python3
"""Standalone witness checker using only the textual definition (strings, no bitmasks).
usage: check.py n t file [expected_size]
file lines: 'u a b' ('-' = empty string); '#' comments.
Checks lengths, distinctness of strategies and pairwise disjointness of full outcome sets."""
import sys

def D(s):
    return {s[:i] + s[i+1:] for i in range(len(s))}

def O(u, a, b):
    return {u + a} | {u + x for x in D(a)} | {x + b for x in D(u)}

def check(n, t, path, expected=None):
    strat = []
    for line in open(path):
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        u, a, b = [('' if x == '-' else x) for x in line.split()]
        assert set(u + a + b) <= set('01'), line
        assert len(u) == t and len(a) == n - t and len(b) == n - t, f"bad lengths: {line}"
        strat.append((u, a, b))
    assert len(set(strat)) == len(strat), "repeated strategy"
    owner = {}
    for s in strat:
        for w in O(*s):
            assert len(w) in (n, n - 1)
            if w in owner:
                print(f"REJECT: outcome {w} shared by {owner[w]} and {s}")
                return False
            owner[w] = s
    if expected is not None and len(strat) != expected:
        print(f"REJECT: size {len(strat)} != expected {expected}")
        return False
    print(f"OK n={n} t={t} {path}: {len(strat)} strategies, pairwise disjoint outcome sets")
    return True

if __name__ == '__main__':
    n, t, path = int(sys.argv[1]), int(sys.argv[2]), sys.argv[3]
    exp = int(sys.argv[4]) if len(sys.argv) > 4 else None
    sys.exit(0 if check(n, t, path, exp) else 1)
