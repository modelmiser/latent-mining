#!/usr/bin/env python3
# Direct, slow implementation of the definition. Usage: slow.py n maxlen [mode]
# mode: slip (default) or del (deletion-proof, A(n)).
# Enumerates ALL words by increasing length, tests on ALL permutations.
import sys, itertools
def apply(word, v):
    v = list(v)
    for i in word:            # letter i in 1..n-1 acts on wires i,i+1 (0-based i-1,i)
        if v[i-1] > v[i]: v[i-1], v[i] = v[i], v[i-1]
    return v
def sorts(word, n, perms):
    return all(apply(word, p) == sorted(p) for p in perms)
def slips(word, n):
    for p, i in enumerate(word):
        for j in (i-1, i+1):
            if 1 <= j <= n-1:
                yield word[:p] + (j,) + word[p+1:]
def ups(word, n):
    for p, i in enumerate(word):
        if i+1 <= n-1:
            yield word[:p] + (i+1,) + word[p+1:]
def dels(word, n):
    for p in range(len(word)):
        yield word[:p] + word[p+1:]
def proof(word, n, perms, mode='slip'):
    if not sorts(word, n, perms): return False
    var = {'slip': slips, 'del': dels, 'up': ups}[mode]
    return all(sorts(s, n, perms) for s in var(word, n))
if __name__ == '__main__':
    n = int(sys.argv[1]); maxlen = int(sys.argv[2]); mode = sys.argv[3] if len(sys.argv) > 3 else 'slip'
    perms = list(itertools.permutations(range(1, n+1)))
    for L in range(0, maxlen+1):
        good = [w for w in itertools.product(range(1, n), repeat=L) if proof(w, n, perms, mode)]
        print(f"n={n} mode={mode} L={L} count={len(good)}", (' first=' + ' '.join(map(str, good[0]))) if good else '', flush=True)
        if good: break
