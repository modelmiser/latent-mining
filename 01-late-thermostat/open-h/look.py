import sys
from red import run
d=int(sys.argv[1]); c=[]
print(run(d,'mid',0,c))
def isint(s): return not s or max(s)-min(s)+1==len(s)
nonint=sum(1 for F,R,U,u in c if not isint(F) or not isint(R))
print('states',len(c),'noninterval',nonint)
bel={}
for F,R,U,u in c: bel.setdefault((F,R),set()).add((sum(U),u))
print('distinct beliefs',len(bel))
fmt=lambda s: '[%d,%d]'%(min(s),max(s)) if s else '[]' if isint(s) else str(sorted(s))
def fm(s): 
    if not s: return '.'
    return ('[%d,%d]'%(min(s),max(s))) if isint(s) else str(sorted(s))
for (F,R),v in sorted(bel.items(),key=lambda kv:(len(kv[0][0]|kv[0][1]),min(kv[0][0]|kv[0][1]))):
    print(fm(F),fm(R),sorted(v))
