import sys
sys.path.insert(0, "verifier")
from attack import *
from collections import deque
# negative control: demand target+1 from optimal controllers; the test MUST report failure
Bof = {1: 1, 2: 3, 3: 3, 4: 5, 5: 5, 6: 7, 7: 7, 8: 9}
for d in range(1, 9):
    t = 2 * (d // 2) + 1
    try:
        stage, m, info = attack(load_b(d, Bof[d]), d, t + 1)
        print(f"neg-control d={d} demand {t+1}: stage={stage} max|x|={m} -> {'reached (BAD)' if m >= t+1 else 'not reached (expected)'}")
    except Undefined as e:
        print(f"neg-control d={d}: undefined transition {e.args} (BAD)")
# T2: exhaustive reachability for the half-duty clock
for d in range(1, 10):
    start = ((0,) * d, 0, False, 0); seen = {start}; dq = deque([start]); best = 0
    while dq:
        signs, x, rest, par = dq.popleft()
        u = -signs[0] if par == 0 else 0
        for w in ((0,) if rest else (-1, 0, 1)):
            nx = x + u + w; best = max(best, abs(nx)); assert abs(nx) < 50
            st = (signs[1:] + (sg(x),), nx, w != 0, 1 - par)
            if st not in seen: seen.add(st); dq.append(st)
    print(f"clock d={d}: sup|x|={best} (d+1={d+1})")
