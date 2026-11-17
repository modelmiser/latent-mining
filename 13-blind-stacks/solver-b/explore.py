import sys, itertools
from fast import V, N, moves, best_move
from bm import bm
k=int(sys.argv[1]); R=int(sys.argv[2])
seq=[N(k,n) for n in range(1,R+1)]
for skip in range(0,8):
    c=bm(seq[skip:])
    print('N skip',skip,'order',len(c),[str(x) for x in c])
if len(sys.argv)>3:
    opts=[(),(0,),(1,),(0,0),(0,1),(1,0),(1,1)]
    for st in itertools.product(opts,repeat=k):
        s=[V(st,r) for r in range(R)]
        for skip in range(0,10):
            c=bm(s[skip:])
            if len(c)<=(R-skip)//2-3: break
        print(st,'skip',skip,[str(x) for x in c],'policy@',[best_move(st,r) for r in (0,1,2,3,4,5,6,20,R-1)])
