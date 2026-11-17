#!/usr/bin/env python3
"""Independent verifier of an exported adversary strategy tree (JSON from game.cpp).
Shares no code with game.cpp.  Uses the UNSIMPLIFIED machine of the brief (run / idle / switch tick,
'most recently run job A', 'previous tick was a switch tick').
Checks, for EVERY online action sequence:
  (1) after every release the whole released multiset is feasible for the promise machine
      (exact DFS over hindsight schedules from tick 0; switch cost 1 for promise S, 0 for F);
  (2) wherever the tree stops, the online machine can no longer keep every lateness <= L
      (exact DFS over all continuations with no further releases).
Online idle and switch ticks are both looked up as 'e' in the tree; this is sound because the tree was
built against a scheduler whose every non-run tick acts as a switch tick, i.e. one with a superset of options.
usage: verify.py tree.json"""
import sys, json
from functools import lru_cache
sys.setrecursionlimit(1000000)
J=json.load(open(sys.argv[1])); P=J["P"]; T=J["T"]; L=J["L"]; PROM=J["promise"]
stats=dict(nodes=0, promise_checks=0, leaves=0, maxjobs=0)

def canfinish(jobs, A, flag, t, cost):
    """jobs: tuple of (r, deadline, rem). Is there a schedule from tick t completing every job by its deadline?"""
    return _cf(jobs, A, flag, t, cost)
@lru_cache(maxsize=None)
def _cf(jobs, A, flag, t, cost):
    if all(j[2]==0 for j in jobs): return True
    for (r,d,rem) in jobs:
        if rem>0 and max(t,r)+rem>d: return False
    # idle
    if _cf(jobs, A, False, t+1, cost): return True
    # switch tick
    if cost and A>=0 and jobs[A][2]>0 and not flag:
        if _cf(jobs, A, True, t+1, cost): return True
    for i,(r,d,rem) in enumerate(jobs):
        if rem==0 or r>t: continue
        if cost and A>=0 and A!=i and jobs[A][2]>0 and not flag: continue
        if i>0 and jobs[i-1]==jobs[i] and A!=i and A!=i-1: continue   # identical twin, neither is A
        nj=jobs[:i]+((r,d,rem-1),)+jobs[i+1:]
        if _cf(nj, i, False, t+1, cost): return True
    return False

def promise_ok(released):
    stats["promise_checks"]+=1
    jobs=tuple(sorted((r,d,p) for (r,p,d) in released))
    return canfinish(jobs, -1, False, 0, 1 if PROM=='S' else 0)

def online_dead(jobs, A, flag, t):
    """True iff no continuation keeps every lateness <= L (jobs: list of (r,d,rem); order fixed so A index is stable)"""
    js=tuple((r,d+L,rem) for (r,d,rem) in jobs)
    return not canfinish(js, A, flag, t, 1)

def play(node, t, released, jobs, A, flag, path):
    stats["nodes"]+=1
    released=released+[(t,p,d) for p,d in node["rel"]]
    for p,d in node["rel"]:
        assert 1<=p<=P and t+p<=d<=T and t<T, ("illegal job",t,p,d)
    jobs=jobs+[(t,d,p) for p,d in node["rel"]]
    stats["maxjobs"]=max(stats["maxjobs"],len(released))
    if node["rel"] and not promise_ok(released):
        raise SystemExit(f"PROMISE VIOLATED at t={t} path={path} released={released}")
    # every online action
    acts=[("e",jobs,A,False),("e",jobs,A,True)]
    for i,(r,d,rem) in enumerate(jobs):
        if rem==0: continue
        if A>=0 and A!=i and jobs[A][2]>0 and not flag: continue
        nj=jobs[:i]+[(r,d,rem-1)]+jobs[i+1:]
        if rem==1 and t+1-d>L:   # completes too late: adversary already has lateness >= L+1
            acts.append((None,nj,i,False)); continue
        acts.append((f"r{rem},{d}",nj,i,False))
    for lab,nj,nA,nf in acts:
        if lab is None: stats["leaves"]+=1; continue
        child=node["moves"].get(lab)
        if child is not None:
            play(child, t+1, released, nj, nA, nf, path+[lab])
        else:
            stats["leaves"]+=1
            if not online_dead(nj, nA, nf, t+1):
                raise SystemExit(f"TREE INCOMPLETE: after path={path+[lab]} at t={t+1} online can still keep lateness <= {L}; jobs={nj}")
play(J["tree"],0,[],[],-1,False,[])
print(f"VERIFIED: adversary forces lateness >= {L+1}; P={P} T={T} promise={PROM}; {stats}")
