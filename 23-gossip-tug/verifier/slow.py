# Referee: the definition, directly. No symmetry, no pruning. python3 slow.py N
import sys
from functools import lru_cache
sys.setrecursionlimit(10000)

def run(n):
    full = (1 << n) - 1

    def legal(K, a, b):
        return a != b and (not (K[b] >> a) & 1 or not (K[a] >> b) & 1)

    @lru_cache(maxsize=None)
    def val(K):
        if all(k == full for k in K):
            return 0
        best = None
        for a in range(n):
            worst = None
            for b in range(n):
                if legal(K, a, b):
                    L = list(K); L[a] = L[b] = K[a] | K[b]
                    v = 1 + val(tuple(L))
                    if worst is None or v < worst:
                        worst = v
            if worst is not None and (best is None or worst > best):
                best = worst
        assert best is not None  # a non-terminal position has an active agent
        return best

    @lru_cache(maxsize=None)
    def shortest(K):  # minimum over ALL plays (both choices adversarial to Delayer)
        if all(k == full for k in K):
            return 0
        return min(1 + shortest(tuple(K[a] | K[b] if i in (a, b) else K[i] for i in range(n)))
                   for a in range(n) for b in range(a + 1, n) if legal(K, a, b))

    start = tuple(1 << i for i in range(n))
    a = val(start)
    # chain position after "name agent 0 until inactive": partners 1,2,..,n-1 in order
    chain = [0] * n
    for i in range(1, n):
        chain[i] = (1 << (i + 1)) - 1
    chain[0] = full
    print("n=%d A=%d raw positions=%d ; shortest play from chain position=%d (n-2=%d)" % (
        n, a, val.cache_info().currsize, shortest(tuple(chain)), n - 2))

for n in range(2, int(sys.argv[1]) + 1):
    run(n)
