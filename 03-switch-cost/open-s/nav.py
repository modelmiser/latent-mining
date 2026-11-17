# navigate solver-a strategy: nav.py file depth c0 c1 c2 ... (child indices), prints subtree below path to given depth
import sys
f=sys.argv[1]; md=int(sys.argv[2]); path=[int(x) for x in sys.argv[3:]]
nodes={}
for line in open(f):
    w=line.split()
    if w[0]=='header': continue
    i=int(w[1]); t=int(w[2]); k=int(w[3]); rel=[(int(w[4+2*j]),int(w[5+2*j])) for j in range(k)]
    m=int(w[4+2*k]); ch=[(w[5+2*k+2*j],int(w[6+2*k+2*j])) for j in range(m)]
    nodes[i]=(t,rel,ch)
def st(c):
    A,fl,js=c.split(':'); A=int(A)
    js=[int(x) for x in js.split('.')] if js else []
    s=' '.join('%d/d%d'%(x&7,x>>3) for x in js)
    return '{%s} A=%s%s'%(s, ('%d/d%d'%(A&7,A>>3)) if A else '-', ' sw' if fl=='1' else '')
def show(i,dep):
    t,rel,ch=nodes[i]
    ind='  '*dep
    r=' '.join('(r%d,p%d,d%d)'%(t,p,d) for p,d in rel) or 'nothing'
    print('%s[t=%d] release %s'%(ind,t,r))
    if dep>=md: print(ind+'  ...'); return
    for n,(c,cid) in enumerate(ch):
        if cid<0: print('%s  %d-> %s : LOST'%(ind,n,st(c)))
        else:
            print('%s  %d-> %s'%(ind,n,st(c))); show(cid,dep+1)
i=0
for c in path:
    t,rel,ch=nodes[i]
    print('[t=%d] release %s ; take %s'%(t,' '.join('(p%d,d%d)'%x for x in rel) or 'nothing', st(ch[c][0])))
    i=ch[c][1]
show(i,0)
