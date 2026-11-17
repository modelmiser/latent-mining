# Compare three feasibility deciders on random + exhaustive small job lists:
#  (a) vgame's frontier machinery (C++), (b) mach.feasible (memo DFS), (c) mach.feasible_brute (raw enumeration)
import sys, random, subprocess, itertools
from mach import feasible, feasible_brute
cost = int(sys.argv[1]); N = int(sys.argv[2]); seed = int(sys.argv[3])
random.seed(seed)
lists = []
# exhaustive: all multisets of <=3 jobs with P<=2,T<=4
types = [(r,p,d) for r in range(4) for p in (1,2) for d in range(r+p,5)]
for k in range(0,4):
    for c in itertools.combinations_with_replacement(types,k): lists.append(list(c))
nex = len(lists)
for _ in range(N):
    T = random.randint(2,7); P = random.randint(1,3); n = random.randint(1,5)
    js = []
    for _ in range(n):
        p = random.randint(1,min(P,T)); r = random.randint(0,T-p); d = random.randint(r+p,T)
        js.append((r,p,d))
    lists.append(js)
inp = '\n'.join(' '.join([str(len(j))]+[str(x) for job in j for x in job]) for j in lists)+'\n'
out = subprocess.run(['./vgame','3','8','0','S' if cost else 'F','feaslist'],input=inp,capture_output=True,text=True).stdout.split()
assert len(out)==len(lists), (len(out),len(lists))
mis = 0; yes = 0
for js, o in zip(lists, out):
    a = (o=='1'); b = feasible(js,cost); c = feasible_brute(js,cost)
    yes += b
    if not (a==b==c):
        mis += 1
        if mis<10: print("MISMATCH", js, a, b, c)
print("cost=%d lists=%d (exhaustive %d + random %d) feasible=%d mismatches=%d" % (cost,len(lists),nex,N,yes,mis))
