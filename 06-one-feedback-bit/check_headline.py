# Checks the two finite facts behind the headline, from the definitions only.
# 1. The 13 strategies in solver-a/wit_6_2.txt (n = 6, t = 2) have pairwise disjoint outcome sets.
# 2. A(5) = 6: no 7 binary words of length 5 have pairwise disjoint single-deletion sets.
import itertools
def D(s): return {s[:i] + s[i+1:] for i in range(len(s))}
W = [l.split() for l in open('solver-a/wit_6_2.txt') if not l.startswith('#')]
O = [{u + a} | {u + x for x in D(a)} | {x + b for x in D(u)} for u, a, b in W]
print("strategies:", len(W), " pairwise disjoint:",
      all(O[i].isdisjoint(O[j]) for i in range(len(O)) for j in range(i)))
words = [''.join(p) for p in itertools.product('01', repeat=5)]
balls = [D(w) for w in words]
best = 0
def rec(i, used, c):
    global best
    if c + (len(words) - i) <= best: return
    if i == len(words): best = max(best, c); return
    if balls[i].isdisjoint(used): rec(i + 1, used | balls[i], c + 1)
    rec(i + 1, used, c)
rec(0, set(), 0)
print("A(5) =", best)
