# random machines (all states reachable or not): compare fast.cpp 'eval' (multiword bitsets) with family.spreads/gval
import random, subprocess, family as F
random.seed(12345); bad=0; tot=0
for w in (5,7,8,9):
    for _ in range(150):
        n=random.randint(2,w+1)
        d=[(random.randrange(n),random.randrange(n)) for _ in range(n)]
        sp=F.spreads(w,d); g=F.gval(d,sp)
        # g in the C++ is over states with nonempty C; unreachable states have empty C in both codes
        exp=" ".join(str(b-a) if x else "-1" for x in sp for a,b in [x or (0,0)])+f" g={g}"
        out=subprocess.run([f"./fastx{w}","eval",str(n)]+[str(v) for p in d for v in p],capture_output=True,text=True).stdout.strip()
        tot+=1
        if out!=exp: bad+=1; print("MISMATCH",w,d,out,"|",exp)
print("tested",tot,"mismatches",bad)
