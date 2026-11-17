import sys, itertools
A={'a':{1:1,2:1,3:2,4:3},'b':{1:4,2:1,3:3,4:4}}
def im(S,w):
    for c in w: S=frozenset(A[c][q] for q in S)
    return S
Q=frozenset([1,2,3,4])
d=int(sys.argv[1])
subs=[frozenset(c) for r in range(1,5) for c in itertools.combinations([1,2,3,4],r)]
words=[''.join(w) for w in itertools.product('ab',repeat=d)]
INF=10**9
R={(S,w):(0 if len(S)==1 else INF) for S in subs for w in words}
def succ(S,w,x,y):
    if d==0: return (im(S,x+y),'')
    return (im(S,w[0]+y),w[1:]+x)
ch=True
while ch:
    ch=False
    for S in subs:
        for w in words:
            if R[(S,w)]==0: continue
            best=min(max(R[succ(S,w,x,y)] for y in 'ab') for x in 'ab')
            if best<INF and best+1<R[(S,w)]: R[(S,w)]=best+1; ch=True
nm=lambda S:''.join(map(str,sorted(S)))
ws=[w for w in words if R[(Q,w)]<INF]
print('winning initial w:',[(w,R[(Q,w)]) for w in ws])
w0=min(ws,key=lambda w:R[(Q,w)])
# reachable decision table
seen={}; st=[(Q,w0)]
while st:
    S,w=st.pop()
    if (S,w) in seen or len(S)==1: continue
    xs=[x for x in 'ab' if max(R[succ(S,w,x,y)] for y in 'ab')<R[(S,w)]]
    seen[(S,w)]=xs
    for y in 'ab': st.append(succ(S,w,xs[0],y))
for (S,w),xs in sorted(seen.items(),key=lambda kv:-R[kv[0]]): print(nm(S),w,'->',''.join(xs),'rank',R[(S,w)])
