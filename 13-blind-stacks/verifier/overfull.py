# mechanical test of the "overfull suffix" lemma (A Thm 2 / B claim C(j)) on the flag DP:
# all-U (phase-start) states where some suffix S_j..S_k holds more items than stacks must have V < t!  (t = items left)
import math, sys
sys.argv = ['x', 'none']
exec(open('flagcheck.py').read().split("what = sys.argv[1]")[0])
for k in (1, 2, 3, 4):
    U = 4
    states, V, Q = dp(k, U)
    tested = viol = full = 0
    for st in states:
        if any('K' in s for s in st): continue
        over = any(sum(len(s) for s in st[j:]) > k - j for j in range(k))
        for u in range(U + 1):
            t = sum(len(s) for s in st) + u
            if over:
                tested += 1
                if V[u][st] >= math.factorial(t): viol += 1
            elif u == 0 and V[u][st] == math.factorial(t): full += 1
    print("k=%d overfull phase-start states tested=%d violations=%d ; non-overfull u=0 states with V=t!: %d" % (k, tested, viol, full))
