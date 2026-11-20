# exhaustive s=2: all 4^4 rules d, both (qA,qB) orders, all 4 output maps, straight from the definition
import itertools
from slowcheck import G_upto
best=0
for qA,qB in ((0,1),(1,0)):
  for w in ('AA','AB','BA','BB'):
    for vals in itertools.product([(0,0),(0,1),(1,0),(1,1)],repeat=4):
      d={(0,0):vals[0],(0,1):vals[1],(1,0):vals[2],(1,1):vals[3]}
      if G_upto((2,qA,qB,w,d),2) is None: best+=1
print('s=2 protocols winning N=2:',best,'of',2*4*256)
