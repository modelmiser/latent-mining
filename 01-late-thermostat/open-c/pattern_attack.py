"""The period-4 / period-6 (n,p) patterns that survive the idealised lemma system (proofs.txt section 4):
run the exact best-response adversary against them (z_t = 0 and all z-variants over one period for period 4)."""
import sys, itertools
sys.dont_write_bytecode=True
from attack import breach
pat4=[(-1,1),(1,-1),(1,-1),(1,-1)]
pat6=[(-1,0),(0,1),(1,-1),(1,-1),(1,-1),(1,-1)]
for d,pat in ((3,pat4),(7,pat4),(11,pat4),(5,pat6),(11,pat6)):
    L=len(pat); worst=0; unb=0
    for shift in range(L):
        for zs in itertools.product((-1,0,1),repeat=L):
            clock=lambda t:(pat[(t+shift)%L][0],zs[t%L],pat[(t+shift)%L][1])
            r=breach(clock,d,d,12*d)
            if r is None: unb+=1
            else: worst=max(worst,r[0])
    print(f"d={d} pattern period {L}: {L*3**L} clocks (all phase shifts, all periodic z): unbreached at band {d}: {unb}; latest breach t={worst}")
