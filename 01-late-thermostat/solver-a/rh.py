import sys, time
from game import *
for d in range(int(sys.argv[1]), int(sys.argv[2])+1):
    t=time.time()
    R, fs, res = relay(d)
    print(f"d={d} R={R} maps={fs} all={sorted((v,f) for f,v in res.items() if v is not None)}", flush=True)
    H, st, n = history(d, lo=max(0,(d//2)))
    print(f"d={d} H={H} beliefs={n} t={time.time()-t:.1f}", flush=True)
