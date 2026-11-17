# pretty-print the top of a strategy file: showstrat.py file maxdepth
import sys
f=sys.argv[1]; md=int(sys.argv[2])
nodes={}
for line in open(f):
    w=line.split()
    if w[0]=='header': hdr=w; continue
    i=int(w[1]); t=int(w[2]); k=int(w[3]); rel=[(int(w[4+2*j]),int(w[5+2*j])) for j in range(k)]
    m=int(w[4+2*k]); ch=[(w[5+2*k+2*j],int(w[6+2*k+2*j])) for j in range(m)]
    nodes[i]=(t,rel,ch)
def st(c):
    A,fl,js=c.split(':'); A=int(A)
    js=[int(x) for x in js.split('.')] if js else []
    s=' '.join('%d/d%d'%(x&7,x>>3) for x in js)
    return '{%s} A=%s%s'%(s, ('%d/d%d'%(A&7,A>>3)) if A else '-', ' sw' if fl=='1' else '')
seen=set()
def show(i,dep):
    t,rel,ch=nodes[i]
    ind='  '*dep
    r=' '.join('(r%d,p%d,d%d)'%(t,p,d) for p,d in rel) or 'nothing'
    if i in seen: print('%s[t=%d] node %d (shown above)'%(ind,t,i)); return
    seen.add(i)
    print('%s[t=%d] release %s'%(ind,t,r))
    if dep>=md: print(ind+'  ...'); return
    for c,cid in ch:
        if cid==-1: print('%s  -> %s : LOST (a job is already doomed to lateness>L)'%(ind,st(c)))
        elif cid==-2: print('%s  -> %s : LOST (no releases left; checked exhaustively)'%(ind,st(c)))
        else:
            print('%s  -> %s'%(ind,st(c))); show(cid,dep+1)
show(0,0)
