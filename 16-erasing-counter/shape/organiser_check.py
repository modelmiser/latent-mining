# organiser's own check, from the definition in chain/3-question.txt
import re,sys
def parse(s):
    t=re.findall(r'\(|\)|\[\d*\]|\d+',s); pos=0
    def p():
        nonlocal pos
        x=t[pos]; pos+=1
        if x.startswith('['): return ('L', int(x[1:-1]) if len(x)>2 else None)
        assert x=='('; i=int(t[pos]); pos+=1; a=p(); b=p(); assert t[pos]==')'; pos+=1
        return ('N',i,a,b)
    r=p(); assert pos==len(t); return r
def check(T,path=(),depth=0):
    if T[0]=='L': assert T[1] is None or T[1] in path; return depth
    assert T[1] not in path
    return max(check(T[2],path+(T[1],),depth+1),check(T[3],path+(T[1],),depth+1))
def L(T): return 1 if T[0]=='L' else L(T[3])*(L(T[2])+1)
def step(T,s):
    s=dict(s); P=[]; n=T
    while n[0]=='N': P.append(n[1]); n=n[3] if s.get(n[1],0) else n[2]
    for i in P: s[i]=0
    if n[1] is not None: s[n[1]]=1
    return s
for line in sys.argv[1:]:
    T=parse(line); d=check(T); s={}; seen={}; k=0
    while True:
        key=tuple(sorted((i,v) for i,v in s.items() if v))
        if key in seen: break
        seen[key]=k; s=step(T,s); k+=1
    print("depth",d,"L",L(T),"cycle through the first repeated state has length",k-seen[key],"(tail",seen[key],")")
