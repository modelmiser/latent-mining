# Verifier's own checker for solver-b JSON adversary trees. Uses only mach.py (naive machine).
# usage: check_b.py tree.json [-v]
import sys, json
from mach import actions, step, feasible, fmt_sched
sys.setrecursionlimit(1000000)
J = json.load(open(sys.argv[1])); VERB = len(sys.argv) > 2
P, T, L, PROM = J["P"], J["T"], J["L"], J["promise"]
cost = 1 if PROM == 'S' else 0
st = dict(nodes=0, releases_checked=0, leaves=0, plays=0)
fcache = {}
def promise(jobs):
    k = tuple(sorted(jobs))
    if k not in fcache: fcache[k] = feasible(k, cost, want_schedule=True)
    return fcache[k]
omemo = {}
seen = set()
def online_can(jobs, t, rem, A, prevsw):
    """Can the online machine (switch cost 1), with no further releases, finish every job with lateness <= L?"""
    if all(x == 0 for x in rem): return True
    for j in range(len(jobs)):
        if rem[j] and t + rem[j] > jobs[j][2] + L: return False
    key = (jobs, t, rem, A, prevsw)
    if key in omemo: return omemo[key]
    r = False
    for act in actions(t, jobs, rem, A, prevsw, 1):
        rem2, A2, ps2 = step(rem, A, prevsw, act)
        if online_can(jobs, t+1, rem2, A2, ps2): r = True; break
    omemo[key] = r
    return r
def play(node, t, jobs, rem, A, prevsw, path):
    key = (id(node), tuple(sorted((jobs[j] + (rem[j], j == A)) for j in range(len(jobs)))), prevsw)
    if not VERB:
        if key in seen: return
        seen.add(key)
    st["nodes"] += 1
    assert set(node.keys()) <= {"rel", "moves"}
    for p, d in node["rel"]:
        if not (t < T and 1 <= p <= P and t + p <= d <= T): raise SystemExit("REJECT illegal job %s at t=%d path=%s" % ((t,p,d), t, path))
    jobs = jobs + tuple((t, p, d) for p, d in node["rel"])
    rem = rem + tuple(p for p, d in node["rel"])
    if node["rel"]:
        st["releases_checked"] += 1
        s = promise(jobs)
        if s is None: raise SystemExit("REJECT promise violated at t=%d path=%s released=%s" % (t, path, list(jobs)))
        if VERB: print("  " * len(path) + "t=%d release %s ; hindsight for %s: %s" % (t, node["rel"], sorted(jobs), fmt_sched(tuple(sorted(jobs)), s)))
    used = set()
    for act in actions(t, jobs, rem, A, prevsw, 1):      # EVERY legal online action, idle and switch separately
        rem2, A2, ps2 = step(rem, A, prevsw, act)
        if act[0] == 'run':
            j = act[1]; lab = "r%d,%d" % (rem[j], jobs[j][2])
        else: lab = "e"
        child = node["moves"].get(lab)
        if child is not None:
            used.add(lab)
            play(child, t+1, jobs, rem2, A2, ps2, path + [lab + ("(sw)" if act[0]=='switch' else "")])
        else:
            st["leaves"] += 1
            if online_can(jobs, t+1, rem2, A2, ps2):
                raise SystemExit("REJECT tree incomplete: after path=%s then %s at t=%d online can still keep lateness <= %d; jobs=%s rem=%s" % (path, act, t, L, list(jobs), rem2))
    unused = set(node["moves"].keys()) - used
    if unused and VERB: print("  " * len(path) + "note: tree has unreachable move labels %s here" % sorted(unused))
play(J["tree"], 0, (), (), None, False, [])
print("ACCEPT: adversary forces lateness >= %d; P=%d T=%d promise=%s; %s" % (L+1, P, T, PROM, st))
