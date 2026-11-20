#!/usr/bin/env python3
# Independent witness checker. usage: check.py witness.txt [L|S]
# Reads Q/E lines; unlisted E = identity, unlisted Q = EMPTY. Enumerates EVERY legal op sequence of length <= t.
import sys
def main():
    path=sys.argv[1]; mode=sys.argv[2] if len(sys.argv)>2 else 'L'
    Q={};E={};n=t=None;init=''
    for line in open(path):
        p=line.split()
        if not p or p[0].startswith('#'): continue
        if p[0]=='n': n=int(p[1])
        elif p[0]=='t': t=int(p[1])
        elif p[0]=='init': init='' if p[1]=='-' else p[1]
        elif p[0]=='Q': Q[p[1]]=p[2]
        elif p[0]=='E': E[(p[1],p[2])]=p[3]
    for (c,op),c2 in E.items():
        assert len(c)==n and len(c2)==n and set(c+c2)<=set('01')
        if any(a=='1' and b!='1' for a,b in zip(c,c2)): print("FAIL: not monotone",c,op,c2); return 1
    start='0'*n
    if mode=='S' and Q.get(start,'EMPTY')!=(init[-1] if init else 'EMPTY'): print("FAIL: Q(start) wrong"); return 1
    cnt=0
    stack=[(start,init,0)]
    while stack:
        c,st,s=stack.pop()
        if s==t: continue
        for op in ('PUSH0','PUSH1','POP'):
            if op=='POP':
                if not st: continue
                st2=st[:-1]
            else: st2=st+op[-1]
            c2=E.get((c,op),c)
            want=st2[-1] if st2 else 'EMPTY'
            cnt+=1
            if Q.get(c2,'EMPTY')!=want:
                print("FAIL: after reaching stack %r at step %d config %s Q=%s want %s"%(st2,s+1,c2,Q.get(c2,'EMPTY'),want)); return 1
            stack.append((c2,st2,s+1))
    print("OK n=%d t=%d mode=%s nonempty sequences checked=%d"%(n,t,mode,cnt)); return 0
sys.exit(main())
