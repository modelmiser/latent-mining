#!/usr/bin/env python3
# Hybrid scheme: p pairs (pair scheme) + 6 cells running a computed sub-scheme once every pair has been touched.
# n = 2p+6, t = p+5.  usage: hybrid.py p out   (sub-witnesses are produced by ./sat into mwit/ and re-checked by check.py)
import sys,subprocess,os
p=int(sys.argv[1]); n=2*p+6; t=p+5
def sub(w,r):
    name='mwit/w_%s_r%d.txt'%(w or '-',r)
    if not os.path.exists(name):
        rc=subprocess.run(['./sat',str(r+1),str(r),'S',w or '-',name]).returncode; assert rc==0
        assert subprocess.run(['python3','check.py',name,'S'],capture_output=True).returncode==0
    Q={};E={}
    for l in open(name):
        q=l.split()
        if q and q[0]=='Q': Q[q[1]]=q[2]
        if q and q[0]=='E': E[(q[1],q[2])]=q[3]
    return Q,E
def decode(pairs):  # live pairs spell the stack, dead = number of 11 pairs
    st=''.join('0' if pairs[2*i]=='1' else '1' for i in range(p) if pairs[2*i]!=pairs[2*i+1])
    return st, sum(1 for i in range(p) if pairs[2*i:2*i+2]=='11'), any(pairs[2*i:2*i+2]=='00' for i in range(p))
def E(c,op):
    pairs,reg=c[:2*p],c[2*p:]
    st,dead,opn=decode(pairs)
    if opn:
        x=list(pairs)
        if op=='POP':
            for i in range(p-1,-1,-1):
                if x[2*i]!=x[2*i+1]: x[2*i]=x[2*i+1]='1'; break
        else:
            for i in range(p):
                if x[2*i]=='0'==x[2*i+1]: x[2*i+(op=='PUSH1')]='1'; break
        return ''.join(x)+reg
    r=t-p-dead
    if r<=0: return c
    w=st[-(r+1):]; Q_,E_=sub(w,r)
    k=reg[:r+1]
    return pairs+E_.get((k,op),k)+reg[r+1:]
def Q(c):
    pairs,reg=c[:2*p],c[2*p:]
    st,dead,opn=decode(pairs)
    top=st[-1] if st else 'EMPTY'
    if opn: return top
    r=t-p-dead
    if r<=0: return top
    w=st[-(r+1):]; Q_,E_=sub(w,r)
    return Q_.get(reg[:r+1],'EMPTY')
seen={}; todo=['0'*n]
with open(sys.argv[2],'w') as f:
    f.write("# hybrid scheme p=%d\nn %d\nt %d\n"%(p,n,t))
    while todo:
        c=todo.pop()
        if c in seen: continue
        seen[c]=1; f.write("Q %s %s\n"%(c,Q(c)))
        for op in ('PUSH0','PUSH1','POP'):
            c2=E(c,op); f.write("E %s %s %s\n"%(c,op,c2)); todo.append(c2)
