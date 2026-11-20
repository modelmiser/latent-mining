# Lemma C(a) reach test: for every line (no height limit) on lists inside {0..W-1} of length n,
# record max distance of a chip beyond the rightmost add position vs rho(#firings so far).
import sys, itertools
def rho(F):
    if F <= 0: return 0
    k = 0
    while (k+2)*(k+3)//2 <= F: k += 1
    return k + 1
def run(lst):
    p = max(lst); worst = 0
    S = {((), 0)}
    for a in lst:
        T = set()
        for st, f in S:
            d = dict(st); d[a] = d.get(a, 0) + 1
            T.add((tuple(sorted(d.items())), f))
            for x, c in d.items():
                if c >= 2:
                    e = dict(d); e[x] -= 2
                    if not e[x]: del e[x]
                    e[x-1] = e.get(x-1, 0) + 1; e[x+1] = e.get(x+1, 0) + 1
                    T.add((tuple(sorted(e.items())), f + 1))
        S = T
        for st, f in S:
            reach = max(x for x, c in st) - p
            if reach > rho(f): return ("VIOLATION", lst, st, f)
            worst = max(worst, reach - rho(f))
    return worst
n = int(sys.argv[1]); W = int(sys.argv[2]); tight = -99
for lst in itertools.product(range(W), repeat=n):
    if W - 1 not in lst: continue
    r = run(lst)
    if isinstance(r, tuple): print(r); sys.exit(1)
    tight = max(tight, r)
print("Lemma C(a): n=%d W=%d no violation; max(reach - rho(F)) = %d (0 means the bound is attained)" % (n, W, tight))
