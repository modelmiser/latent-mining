#!/usr/bin/env python3
"""Writes witness_plays.txt: for each (k,m) the walk produced by ADV against ALG,
with ALG's fault count and opt (brute force).  A third-party program can recompute both."""
import sys,io,contextlib
out=open("witness_plays.txt","w")
out.write("# k m F | walk (ADV vs ALG) | ALG faults | opt(walk)  -- ALG faults must equal F=max(m+1,2m+1-floor(k/2)), opt<=m\n")
for k in range(2,8):
    for m in range(1,5):
        sys.argv=["verify.py","none",str(k),str(m)]
        g={}
        with contextlib.redirect_stdout(io.StringIO()):
            try: exec(open("verify.py").read(),g)
            except Exception: pass
        A=g["Adv"](); A.s=0;A.H=None;A.queue=[];A.mid=False;A.cyc=0;A.e=0;A.init=False;A.stopped=False
        W=g["alg_fault"](None,0,0); p=0; f=1; walk=[0]; wf=g["wf_step"](g["WF0"],0)
        while True:
            C=frozenset(range(W[0],W[1]+1))
            q=g["adv_next"](A,C,p)
            if q is None: break
            wf=g["wf_step"](wf,q); assert wf
            if not (W[0]<=q<=W[1]): W=g["alg_fault"](W,q,0); f+=1
            p=q; walk.append(q)
        opt=min(wf.values())
        assert f==g["BOUND"] and opt<=m
        out.write(f"{k} {m} {g['BOUND']} | {' '.join(map(str,walk))} | {f} | {opt}\n")
out.close()
