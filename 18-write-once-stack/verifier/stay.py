import vcheck, itertools, glob
OPS=vcheck.OPS
# (i) the two cited stay events
for path,seq in [('../solver-a/witness_n5_t4.txt',['PUSH1']*3),('../solver-b/wit_n5_t4_L.txt',['PUSH0']*3)]:
    n,t,init,Q,E=vcheck.parse(path); assert isinstance(vcheck.verify(n,t,init,Q,E,'L'),tuple)
    c='0'*n; tr=[c]
    for op in seq: c=E.get((c,op),c); tr.append(c)
    print(path,tr,'third op stays:',tr[2]==tr[3], 't=',t)
# (ii) lemma by brute force from the definition of "agree on all common legal continuations"
def top(s): return s[-1] if s else 'E'
def app(s,op): return s[:-1] if op=='POP' else s+op[4]
def agree(s1,s2,k):
    if top(s1)!=top(s2): return False
    if k==0: return True
    for op in OPS:
        if op=='POP' and (not s1 or not s2): continue
        if not agree(app(s1,op),app(s2,op),k-1): return False
    return True
bad=0;cnt=0
for L in range(0,7):
    for bits in itertools.product('01',repeat=L):
        S=''.join(bits)
        for r in range(1,6):
            for b in '01':
                lhs=agree(S,S+b,r-1); rhs=(len(S)>=r and set(S[len(S)-r:])<={b}); cnt+=1
                if lhs!=rhs: bad+=1; print('PUSH mismatch',S,r,b,lhs,rhs)
            if S:
                lhs=agree(S,S[:-1],r-1); rhs=(len(S)>=r+1 and len(set(S[len(S)-r-1:]))==1); cnt+=1
                if lhs!=rhs: bad+=1; print('POP mismatch',S,r,lhs,rhs)
print('lemma cases',cnt,'mismatches',bad)
# (iii) every stay event in every headline witness obeys the condition (history length>=1)
viol=0;stays=0
files=[(f,'L') for f in glob.glob('../solver-a/witness_n?_t?.txt')+glob.glob('../solver-b/s_n*_L.txt')]+[(f,'S') for f in glob.glob('../solver-a/witness_*_rootempty.txt')+glob.glob('../solver-b/s_n*_S.txt')]
for path,mode in files:
    n,t,init,Q,E=vcheck.parse(path)
    if not isinstance(vcheck.verify(n,t,init,Q,E,mode),tuple): print('not valid',path); continue
    def rec(c,st,left,depth):
        global viol,stays
        if left==0: return
        for op in OPS:
            if op=='POP' and not st: continue
            st2=app(st,op); c2=E.get((c,op),c)
            if c2==c and (depth>=1 or mode=='S'):
                stays+=1; r=left
                good=(len(st)>=r and set(st[len(st)-r:])<={op[4]}) if op!='POP' else (len(st)>=r+1 and len(set(st[len(st)-r-1:]))==1)
                if not good: viol+=1
            rec(c2,st2,left-1,depth+1)
    rec('0'*n,init,t,0)
print('files',len(files),'stay events',stays,'violating the stated condition',viol)
# (iv) weights: in t=n-1 witnesses, max and min weight after s ops
for path in ['../solver-a/witness_n8_t7.txt','../solver-b/s_n8_t7_L.txt']:
    n,t,init,Q,E=vcheck.parse(path); W={}
    def rec2(c,st,left,d):
        W.setdefault(d,set()).add(c.count('1'))
        if left==0: return
        for op in OPS:
            if op=='POP' and not st: continue
            rec2(E.get((c,op),c),app(st,op),left-1,d+1)
    rec2('0'*n,'',t,0); print(path,{d:(min(v),max(v)) for d,v in W.items()})
# (v) mwit with |w|=r+1 remain valid when the real stack is longer
n,t,init,Q,E=vcheck.parse('../solver-b/mwit/w_011010_r5.txt')
print('longer real stack:',all(isinstance(vcheck.verify(n,t,p+init,Q,E,'S'),tuple) for p in ['0','1','01','110']))
