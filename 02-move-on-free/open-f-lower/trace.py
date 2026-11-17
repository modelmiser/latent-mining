import sys
from battery import *
M=int(sys.argv[1]); a=sys.argv[2]; mi=int(sys.argv[3])
log=[]
g=run(ADV[a], MGR[mi], M, log=log)
prev=None
for l in log:
    k=l.split()[0]+l.split()[1][0]
    if '--all' in sys.argv or k!=prev or 'moves=[(' in l: print(l)
    prev=k
print(log[-1]); print(MGR[mi].name, a, 'extent', g.extent, 'vs', math.ceil(5*M/3))
