import vcheck, itertools, random
random.seed(1)
OPS=vcheck.OPS
def reached(n,t,init,Q,E):
    seen=set(); ev=set()
    def rec(c,st,left):
        if left==0: return
        for op in OPS:
            if op=='POP' and not st: continue
            st2=st[:-1] if op=='POP' else st+op[4]
            c2=E.get((c,op),c); seen.add(c2); ev.add((c,op)); rec(c2,st2,left-1)
    rec('0'*n,init,t); return seen,ev
def ok(r): return isinstance(r,tuple)
for path,mode in [('../solver-b/s_n7_t6_L.txt','L'),('../solver-a/witness_n7_t6_rootempty.txt','S'),('../solver-b/hyb_p1.txt','S'),('../solver-b/mwit/w_0110_r5.txt','S'),('../solver-a/witness_n8_t7.txt','L')]:
    n,t,init,Q,E=vcheck.parse(path)
    assert ok(vcheck.verify(n,t,init,Q,E,mode))
    seen,ev=reached(n,t,init,Q,E)
    # 1. flip Q on each reached config: must all be rejected
    a=b=0
    for c in seen:
        for q in ('0','1','EMPTY'):
            if q==Q.get(c,'EMPTY'): continue
            Q2=dict(Q); Q2[c]=q; a+=1; b+= not ok(vcheck.verify(n,t,init,Q2,E,mode))
    # 2. non-monotone: clear one set bit in any E target: all rejected
    c2n=d2=0
    for (c,op),d in E.items():
        for i in range(n):
            if c[i]=='1':
                E2=dict(E); E2[(c,op)]=d[:i]+'0'+d[i+1:]; c2n+=1; d2+= not ok(vcheck.verify(n,t,init,Q,E2,mode))
    # 3. redirect a used E entry to a config with different Q: rejected
    e3=f3=0
    for (c,op) in ev:
        d=E.get((c,op),c)
        for d2_ in Q:
            if Q[d2_]!=Q.get(d,'EMPTY') and all(not(x=='1' and y=='0') for x,y in zip(c,d2_)):
                E2=dict(E); E2[(c,op)]=d2_; e3+=1; f3+= not ok(vcheck.verify(n,t,init,Q,E2,mode)); break
    # 4. t+1 must be rejected only if scheme is at its limit (report)
    r4=ok(vcheck.verify(n,t+1,init,Q,E,mode))
    # 5. wrong init stack (flip top) in S mode / or push extra
    r5=ok(vcheck.verify(n,t,(init[:-1]+('1' if init[-1:]=='0' else '0')) if init else '1',Q,E,mode))
    print(path,'Qflip rejected %d/%d; nonmono rejected %d/%d; redirect rejected %d/%d; t+1 accepted=%s; wrong-init accepted=%s'%(b,a,d2,c2n,f3,e3,r4,r5))
