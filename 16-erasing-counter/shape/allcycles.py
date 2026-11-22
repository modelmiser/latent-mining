#!/usr/bin/env python3
"""Second, independently written program: bitmask simulator, FULL functional graph of f_T over all 2^m states.
usage: allcycles.py '<tree>'   -> prints depth, L(T), and the multiset of cycle lengths, longest first.
Shares no code with check.py (own tokenizer, own step)."""
import sys
import re
from collections import Counter


def build(s):
    toks = re.findall(r'[()\[\]]|\d+', s)
    nodes = []  # (kind, val, c0, c1)
    p = [0]

    def rec(pathmask, depth):
        t = toks[p[0]]
        if t == '[':
            if toks[p[0] + 1] == ']':
                p[0] += 2
                nodes.append((0, -1, 0, 0))
                return len(nodes) - 1, depth, 1
            w = int(toks[p[0] + 1]) - 1
            if not (pathmask >> w) & 1:
                raise SystemExit('illegal: leaf writes off its path')
            p[0] += 3
            nodes.append((0, w, 0, 0))
            return len(nodes) - 1, depth, 1
        i = int(toks[p[0] + 1]) - 1
        if (pathmask >> i) & 1:
            raise SystemExit('illegal: index twice on a path')
        p[0] += 2
        a, da, la = rec(pathmask | (1 << i), depth + 1)
        b, db, lb = rec(pathmask | (1 << i), depth + 1)
        p[0] += 1
        nodes.append((1, i, a, b))
        return len(nodes) - 1, max(da, db), lb * (la + 1)

    root, depth, L = rec(0, 0)
    m = 1 + max([n[1] for n in nodes if n[0] == 1], default=-1)
    return nodes, root, depth, L, m


def main():
    nodes, root, depth, L, m = build(sys.argv[1])
    n = 1 << m
    f = [0] * n
    for s in range(n):
        t = root
        P = 0
        while nodes[t][0] == 1:
            i = nodes[t][1]
            P |= 1 << i
            t = nodes[t][3] if (s >> i) & 1 else nodes[t][2]
        o = s & ~P
        if nodes[t][1] >= 0:
            o |= 1 << nodes[t][1]
        f[s] = o
    col = [0] * n
    lens = Counter()
    for s in range(n):
        if col[s]:
            continue
        x = s
        pos = {}
        c = 0
        while not col[x]:
            col[x] = s + 1
            pos[x] = c
            c += 1
            x = f[x]
        if col[x] == s + 1:
            lens[c - pos[x]] += 1
    mx = max(lens)
    print('depth', depth, 'indices', m, 'L(T)', L, 'longest cycle', mx, 'VIOLATES_P1' if mx > L else 'ok',
          'cycle lengths', sorted(lens.items(), reverse=True)[:6])


if __name__ == '__main__':
    main()
