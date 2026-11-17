# Verifier's own checker for solver-a strategy files ("node id t k (p d)*k m (canon child)*m"). Uses mach.py only.
import sys
from mach import actions, step, feasible
sys.setrecursionlimit(1000000)
tok = open(sys.argv[1]).read().split()
assert tok[0] == 'header'
P, T, L, MODE = int(tok[2]), int(tok[4]), int(tok[6]), int(tok[8])
cost = 1 if MODE == 1 else 0
assert MODE in (0, 1)
nodes = {}; i = 9
while i < len(tok):
    assert tok[i] == 'node'; nid, t, k = int(tok[i+1]), int(tok[i+2]), int(tok[i+3]); i += 4
    rel = [(int(tok[i+2*a]), int(tok[i+2*a+1])) for a in range(k)]; i += 2*k
    m = int(tok[i]); i += 1
    ch = {tok[i+2*a]: int(tok[i+2*a+1]) for a in range(m)}; i += 2*m
    assert nid not in nodes; nodes[nid] = (t, rel, ch)
st = dict(positions=0, releases_checked=0, leaves=0)
fcache = {}
def promise(jobs):
    k = tuple(sorted(jobs))
    if k not in fcache: fcache[k] = feasible(k, cost)
    return fcache[k]
omemo = {}
def online_can(jobs, t, rem, A, prevsw):
    if all(x == 0 for x in rem): return True
    for j in range(len(jobs)):
        if rem[j] and t + rem[j] > jobs[j][2] + L: return False
    key = (jobs, t, rem, A, prevsw)
    if key in omemo: return omemo[key]
    r = False
    for act in actions(t, jobs, rem, A, prevsw, 1):
        rem2, A2, ps2 = step(rem, A, prevsw, act)
        if online_can(jobs, t+1, rem2, A2, ps2): r = True; break
    omemo[key] = r; return r
def canon(jobs, rem, A, prevsw):
    codes = sorted(jobs[j][2]*8 + rem[j] for j in range(len(jobs)) if rem[j])
    a = fl = 0
    if A is not None and rem[A] > 0: a = jobs[A][2]*8 + rem[A]; fl = 1 if prevsw else 0
    return "%d:%d:%s" % (a, fl, ".".join(map(str, codes)))
seen = set()
def walk(nid, t, jobs, rem, A, prevsw):
    key = (nid, tuple(sorted((jobs[j] + (rem[j], j == A)) for j in range(len(jobs)))), prevsw)
    if key in seen: return
    seen.add(key); st["positions"] += 1
    if nid not in nodes: raise SystemExit("REJECT missing node %d" % nid)
    nt, rel, ch = nodes[nid]
    if nt != t: raise SystemExit("REJECT tick mismatch node %d" % nid)
    for p, d in rel:
        if not (t < T and 1 <= p <= P and t + p <= d <= T): raise SystemExit("REJECT illegal job at node %d" % nid)
    jobs = jobs + tuple((t, p, d) for p, d in rel); rem = rem + tuple(p for p, d in rel)
    if rel:
        st["releases_checked"] += 1
        if not promise(jobs): raise SystemExit("REJECT promise violated at node %d: %s" % (nid, sorted(jobs)))
    for act in actions(t, jobs, rem, A, prevsw, 1):
        rem2, A2, ps2 = step(rem, A, prevsw, act)
        c = ch.get(canon(jobs, rem2, A2, ps2), -1)
        if c >= 0: walk(c, t+1, jobs, rem2, A2, ps2)
        else:
            st["leaves"] += 1
            if online_can(jobs, t+1, rem2, A2, ps2):
                raise SystemExit("REJECT incomplete at node %d: after %s scheduler can still keep lateness <= %d (jobs %s rem %s)" % (nid, act, L, jobs, rem2))
walk(0, 0, (), (), None, False)
print("ACCEPT: forces lateness >= %d; P=%d T=%d promise=%s; nodes=%d %s" % (L+1, P, T, 'S' if cost else 'F', len(nodes), st))
