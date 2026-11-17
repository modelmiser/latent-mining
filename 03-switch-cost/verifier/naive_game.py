# Naive full-history minimax of the game value (no frontier compression). Tiny cases only.
import sys, itertools
from mach import actions, step, feasible
sys.setrecursionlimit(100000)

def solve(P, T, promise_cost):
    types_at = lambda t: [(t, p, d) for p in range(1, P+1) for d in range(t+p, T+1)]
    fmemo = {}
    def feas(jobs):
        k = tuple(sorted(jobs))
        if k not in fmemo: fmemo[k] = feasible(k, promise_cost)
        return fmemo[k]
    def releases(t, jobs):
        ty = types_at(t)
        out = []
        def rec(i, cur):
            out.append(tuple(cur))
            for k in range(i, len(ty)):
                cur.append(ty[k])
                if feas(jobs + tuple(cur)): rec(k, cur)
                cur.pop()
        rec(0, [])
        return out
    memo = {}
    def V(t, jobs, rem, A, prevsw):
        # future max lateness under optimal play, start of tick t before release
        if t >= T and all(x == 0 for x in rem): return 0
        key = (t, jobs, rem, A, prevsw)
        if key in memo: return memo[key]
        best_adv = -1
        rels = releases(t, jobs) if t < T else [()]
        for R in rels:
            jobs2 = jobs + R
            rem2 = rem + tuple(p for _, p, _ in R)
            best_on = 10**9
            for act in actions(t, jobs2, rem2, A, prevsw, 1):
                rem3, A3, ps3 = step(rem2, A, prevsw, act)
                late = 0
                if act[0] == 'run' and rem3[act[1]] == 0:
                    late = max(0, t+1 - jobs2[act[1]][2])
                if t >= T and act[0] != 'run' and all(
                        not (A is not None and rem2[A] > 0) for _ in [0]) and False:
                    pass
                # after T, pointless non-run ticks could loop forever: bound the horizon
                if t > T + sum(rem2) * 2 + 2: continue
                v = max(late, V(t+1, jobs2, rem3, A3, ps3))
                if v < best_on: best_on = v
            if best_on > best_adv: best_adv = best_on
        memo[key] = best_adv
        return best_adv
    return V(0, (), (), None, False), len(memo)

if __name__ == '__main__':
    P = int(sys.argv[1]); T = int(sys.argv[2]); c = int(sys.argv[3])
    v, n = solve(P, T, c)
    print("P=%d T=%d promise=%s value=%d states=%d" % (P, T, 'same-cost' if c else 'free', v, n))
