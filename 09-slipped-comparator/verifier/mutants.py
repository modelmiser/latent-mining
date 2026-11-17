# referee: each lemma test must be able to FAIL. Imports nothing from lemmas.py (it runs on import); small copies.
import itertools, random, re
random.seed(7)
def run(w, v):
    v = list(v)
    for i in w:
        if v[i-1] > v[i]: v[i-1], v[i] = v[i], v[i-1]
    return v
def issorted(v): return all(v[k] <= v[k+1] for k in range(len(v)-1))
def sorts(w, n): return all(issorted(run(w, v)) for v in itertools.product((0, 1), repeat=n))
def W(n):
    w = []
    for k in list(range(1, n)) + [n-1]: w += list(range(k, 0, -1))
    return w
# (1) false version of T1: "an active letter's slip acts as the ORIGINAL word" must fail often
n = 4; bad = cases = 0
for w in itertools.product(range(1, n), repeat=6):
    w = list(w)
    for x in itertools.product((0, 1), repeat=n):
        y = list(x)
        for p, i in enumerate(w):
            if y[i-1] > y[i]:
                for j in (i-1, i+1):
                    if 1 <= j <= n-1:
                        cases += 1; bad += run([j]+w[p+1:], y) != run(w[p:], y)
            y = run([i], y)
print("mutant T1 (slip == original): cases", cases, "failures", bad, "(must be > 0)")
# (2) false version of T2: idle letter comparing v and v+2 instead of v, v+1
bad = cases = 0
for L in range(6, 10):
    for w in itertools.product(range(1, n), repeat=L):
        w = list(w); v = list(range(n, 0, -1)); pv = {}; later = set()
        for p, i in enumerate(w):
            a, b = v[i-1], v[i]
            if a > b:
                if a == b+1: pv[b] = p
                v[i-1], v[i] = b, a
            elif b == a+2: later.add(a)
        if not issorted(v): continue
        for x in range(1, n):
            cases += 1; bad += issorted(run(w[:pv[x]]+w[pv[x]+1:], range(n, 0, -1))) != (x in later)
print("mutant T2 (v, v+2): cases", cases, "failures", bad, "(must be > 0)")
# (3) L4 on sorting words that are NOT in U(4): 'wasted >= 1' or 'star in U(3)' must fail somewhere
def inU(w, n): return sorts(w, n) and all(sorts(w[:p]+[i+1]+w[p+1:], n) for p, i in enumerate(w) if i+1 <= n-1)
def decompose(w, n):
    k = 1; star = []; wasted = 0
    for i in w:
        if i == k and k <= n-1: k += 1
        elif i == k-1: wasted += 1
        elif i <= k-2: star.append(i)
        else: star.append(i-1)
    return star, wasted
c = nw = ns = 0
for w in itertools.product(range(1, 4), repeat=8):
    w = list(w)
    if sorts(w, 4):
        c += 1; s, wa = decompose(w, 4); nw += wa < 1; ns += not inU(s, 3)
print("mutant L4 on merely sorting words n=4 L=8: words", c, "no-wasted", nw, "star-not-in-U", ns, "(must be > 0)")
# (4) Z invariant with TWO letters deleted from W_6 must fail at the end somewhere
n = 6; w = W(n); bad = 0
for p in range(len(w)):
    for q in range(p+1, len(w)):
        u = w[:p]+w[p+1:q]+w[q+1:]
        bad += not sorts(u, n)
print("mutant T3: W_6 minus two letters, non-sorting cases", bad, "of", len(w)*(len(w)-1)//2, "(must be > 0)")
