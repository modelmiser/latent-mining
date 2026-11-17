import sys, time
from game import *
d=int(sys.argv[1]); lo=int(sys.argv[2])
t=time.time()
C,w=clock(d,lo=lo)
print(f"d={d} C={C} prefix={w[0]} cycle={w[1]} t={time.time()-t:.1f}", flush=True)
