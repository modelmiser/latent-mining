#!/usr/bin/env python3
"""Referee tools: definition-level simulator/checker, parsers, lemma tests for solver A and B proofs.
Written from the problem statement and the proof texts, not from the solvers' code."""
import sys, re, glob, os, random

def cdiv(a,b): return -(-a//b)

def parse(path):
    """returns (n,k,[set of (src,dst)]) ; accepts A format ('n k' then 't: a>b ..'), B format ('# n=.. k=..' then 't a>b ..')."""
    n=k=None; rounds=[]
    for ln in open(path):
        ln=ln.strip()
        if not ln: continue
        if ln.startswith('#'):
            m=re.search(r'n=(\d+)\s+k=(\d+)',ln)
            if m: n,k=int(m.group(1)),int(m.group(2))
            continue
        toks=ln.replace(':',' ').split()
        if n is None and len(toks)==2 and '>' not in ln:
            n,k=int(toks[0]),int(toks[1]); continue
        t=int(toks[0])
        if t!=len(rounds)+1: raise ValueError('round numbering')
        rounds.append([tuple(map(int,x.split('>'))) for x in toks[1:]])
    return n,k,rounds

def check(n,k,rounds,extra=0):
    """Definition-level. returns (status, first_completion_round or None, history of K).  rounds: list of lists of (src,dst).
    After the listed rounds, `extra` destruction-free rounds are simulated (to find first completion)."""
    if n<3: return ('ILLEGAL n',None,None)
    K=[frozenset([i]) for i in range(n)]; hist=[K]; prev=set(); first=None
    for t,Dl in enumerate(list(rounds)+[[]]*extra,1):
        D=set(Dl)
        if len(D)!=len(Dl): return ('ILLEGAL duplicate link in round %d'%t,None,None)
        for (s,d) in D:
            if not(0<=s<n and 0<=d<n) or (d-s)%n not in (1,n-1): return ('ILLEGAL not a link round %d'%t,None,None)
        if len(D)>k: return ('ILLEGAL budget round %d'%t,None,None)
        if D&prev: return ('ILLEGAL repeat round %d'%t,None,None)
        new=[]
        for i in range(n):
            s=set(K[i])
            for j in ((i-1)%n,(i+1)%n):
                if (j,i) not in D: s|=K[j]
            new.append(frozenset(s))
        K=new; hist.append(K); prev=D
        if first is None and any(len(x)==n for x in K): first=t
    return ('LEGAL',first,hist)

# ---------- strategies, written from the reports' / proofs' text ----------
def strat_A(n,k):
    m=min(k,n); delta=cdiv(n,m); T=n-delta-1
    P=[(c*n)//m for c in range(m)]; Q=[p-T for p in P]
    R=[]
    for t in range(1,T+1):
        if t%2: j=(t+1)//2; R.append([((p-j+1)%n,(p-j)%n) for p in P])      # A-link into x: (x+1 -> x), x=P_c-j
        else:   j=t//2;     R.append([((q+j-1)%n,(q+j)%n) for q in Q])      # B-link into x: (x-1 -> x), x=Q_c+j
    return T,R
def strat_B(n,k):
    kk=min(k,n); g=cdiv(n,kk); T=n-1-g
    s=[(j*n)//kk for j in range(kk)]
    R=[]
    for t in range(1,T+1):
        if t%2: v=(t+1)//2; R.append([((s[j]+v+T-(2*v-1)+1)%n,(s[j]+v+T-(2*v-1))%n) for j in range(kk)])
        else:
            v=t//2; row=[]
            for j in range(kk):
                e=s[j-1]+T+1; i=(e-v-T+2*v)%n; row.append(((i-1)%n,i))
            R.append(row)
    return T,R

# ---------- solver A's Theorem U inequality, from the proof text ----------
def deadA(n,k,t):
    """True = A's Theorem U claims no legal sequence survives t (only the part stated in the proof; None if not applicable)."""
    Dp=2*t-n+1
    if t%2==0:
        H=t//2
        if Dp<H: return None
        s=Dp-H; w=2*H-Dp
        if w<=0: return True
        return k*t < 2*n+2*s*cdiv(n,w)
    H1=(t-1)//2; H=H1+1
    if Dp<H: return None
    s=Dp-H1; w=2*H1-Dp
    ok=False
    for E in range(k+1):
        for B in range(E+1):
            if w<=0:
                if 2*n<=E+B and k*t>=2*n-B+s*1: ok=True      # at least one diagonal exists; generous to the adversary
            else:
                if k*t>=2*n-B+s*max(0,cdiv(2*n-E-B,w)): ok=True
    return not ok
def boundB(n,k,T):
    """True = B's Theorem U excludes S(T); None if hypothesis m>=h+1 fails."""
    h=cdiv(T,2); m=2*T-n+2
    if m<h+1: return None
    return 2*n*h > k*T*(2*h+1-m)

class Fail(Exception): pass
def req(c,msg):
    if not c: raise Fail(msg)

def lemma_tests(n,k,rounds,stats):
    """Run every lemma of both proofs on the play, at every prefix t that survives. Raises Fail with the lemma name."""
    st,first,hist=check(n,k,rounds)
    req(st=='LEGAL','play illegal: '+st)
    Tsurv=len(rounds) if first is None else first-1
    # extents from the SETS
    a=[[0]*n]; b=[[0]*n]
    for t in range(1,Tsurv+1):
        at=[];bt=[]
        for i in range(n):
            Ki=hist[t][i]; x=0
            while (i+x+1)%n in Ki and x<n-1: x+=1
            y=0
            while (i-y-1)%n in Ki and y<n-1: y+=1
            req(len(Ki)==x+y+1 and x+y<n-1,'L0/E: knowledge not an arc')
            at.append(x);bt.append(y)
        a.append(at);b.append(bt)
    Aev=[set() for _ in range(Tsurv+1)]; Bev=[set() for _ in range(Tsurv+1)]   # events as PIECE indices per round
    Ahits=[0]*(Tsurv+1); Bhits=[0]*(Tsurv+1)
    for t in range(1,Tsurv+1):
        for (s,d) in rounds[t-1]:
            if (s-d)%n==1:   # A-link into d
                Aev[t].add((d+t)%n); Ahits[t]+=1
            if (d-s)%n==1:   # B-link into d   (n>=3 so exactly one of the two)
                Bev[t].add((d-t)%n); Bhits[t]+=1
    # piece values:  v[t][p]=d_{p-t}(t),  u[t][q]=e_{q+t}(t)
    v=[[t-a[t][(p-t)%n] for p in range(n)] for t in range(Tsurv+1)]
    u=[[t-b[t][(q+t)%n] for q in range(n)] for t in range(Tsurv+1)]
    PA={};PB={}   # pair -> creation round
    ndest=0
    for t in range(1,Tsurv+1):
        ndest+=len(rounds[t-1])
        for (val,ev,P,sgn,nm) in ((v,Aev,PA,-1,'A'),(u,Bev,PB,+1,'B')):
            for p in range(n):
                pm=(p+sgn)%n   # neighbour piece the value is read from: p-1 for A, q+1 for B
                req(val[t][p]<=val[t][pm]+1,'L1/(I) slope '+nm)
                req(val[t][p]>=val[t-1][p],'L1/(M) monotone '+nm)
                if p in ev[t]:
                    req(val[t][p]==val[t-1][pm]+1,'L1/(M) event value '+nm)
                    req(pm not in ev[t-1],'L2 link rule '+nm)
                else:
                    req(val[t][p]==val[t-1][p],'L1/(M) no event => unchanged '+nm)
                y=val[t][p]
                if y>=1 and (p,y) not in P:
                    req(p in ev[t],'L3(i) new pair without event '+nm)
                    P[(p,y)]=t
                    req(t>=2*y-1,'L3(iii) creation round '+nm)
                    if y>=2:
                        req((pm,y-1) in P and P[(pm,y-1)]<=t-2,'L3(ii)/(iii) closure '+nm)
                req(y<=(t+1)//2,'L3 / (H) height '+nm)
        req(len(PA)+len(PB)<=ndest<=k*t,'L3(i) |P|<=destructions<=kt')
        stats['prefixes']+=1
        H=(t+1)//2
        nH=sum(1 for (p,y) in list(PA)+list(PB) if y==H)
        if t%2: req(nH<=k,'L3 consequence: value-H pairs <= k (odd t)')
        f=[t-a[t][x] for x in range(n)]; g=[t-b[t][x] for x in range(n)]
        D=2*t-n+2
        req(all(f[x]+g[x]>=D for x in range(n)),'(*) survive condition')
        # ---- B: Lemma Phi and Theorem U ----
        for (val,sgn,hits,nm) in ((v,-1,Ahits,'A'),(u,+1,Bhits,'B')):
            cur=val[t]; phi=0; drops=0; ntop=0
            for x in range(n):
                nx=(x-sgn)%n      # for A: x+1 ; for B (mirror): y-1
                if cur[nx]<=cur[x]: phi+=cur[x]; drops+=cur[x]+1-cur[nx]; ntop+=1
            req(drops==n,'B Phi: drops sum to n '+nm)
            if k<n: req(phi<=sum(hits[:t+1]),'B Lemma Phi '+nm)
            else:
                if phi>sum(hits[:t+1]): stats['phi_fail_k_ge_n']+=1
        bb=boundB(n,k,t)
        if bb is not None:
            stats['B_U_applicable']+=1
            if k<n: req(not bb,'B Theorem U inequality violated by a surviving play')
        # ---- A: Theorem U ----
        Dp=D-1
        dA=deadA(n,k,t)
        if dA is not None:
            stats['A_U_applicable']+=1
            req(not dA,'A Theorem U (E)/(O) claims dead, but this play survives t=%d'%t)
            req(min(f)>=1 and min(g)>=1,'A U: f,g>=1')
            tot=0; m=0; E=0; B=0; sums=0
            mins=[]
            for (val,oth,P,sgn,nm) in ((f,g,PA,-1,'A'),(g,f,PB,+1,'B')):
                # diagonal of pair (p,y): c=p-y for A ; mirror c=q+y for B
                h={}
                for (p,y) in P:
                    c=(p+sgn*y)%n; h[c]=max(h.get(c,0),y)
                for c,hc in h.items():
                    for y in range(1,hc+1): req(((c-sgn*y)%n,y) in P,'A U: diagonal closure '+nm)
                req(sum(h.values())==len(P),'A U: |P|=sum h_c '+nm)
                X={}
                for x in range(n):
                    p=(x-sgn*t)%n      # A-piece at x is p=x+t ; B-piece at x is q=x-t
                    req((p,val[x]) in P,'A U: current value is a pair')
                    c=(p+sgn*val[x])%n; X.setdefault(c,[]).append(x)
                req(sum(len(z) for z in X.values())==n,'A U: sum delta = n')
                xs=set()
                for c,xl in X.items():
                    fv=[val[x] for x in xl]
                    req(len(set(fv))==len(fv) and max(fv)<=h[c] and min(fv)>=1,'A U: distinct values in [1,h_c]')
                    xst=min(xl,key=lambda x:val[x]); dl=len(xl)
                    req(val[xst]<=h[c]-dl+1,'A U: least value bound')
                    req(dl<=oth[xst]+h[c]-Dp,'A U: inequality (1) '+nm)
                    eps=int(h[c]==H); beta=int(oth[xst]==H)
                    if t%2==0:
                        s=Dp-H; w=2*H-Dp
                        req(dl<=h[c]-s and dl<=w,'A U even: delta<=h-s, delta<=w')
                    else:
                        H1=H-1; s=Dp-H1; w=2*H1-Dp
                        req(dl<=h[c]-s+beta and dl<=w+eps+beta,'A U odd: per-diagonal bounds')
                    E+=eps; B+=beta; m+=1; xs.add(xst)
                if t%2==0:
                    s=Dp-H; w=2*H-Dp
                    req(w>=1 and len(X)>=cdiv(n,w),'A U even: m_A>=ceil(n/w)')
                    req(n<=len(P)-s*len(X),'A U even: n<=|P|-s m')
            if t%2:
                H1=H-1; s=Dp-H1; w=2*H1-Dp
                req(E<=nH<=k,'A U odd: E<=#valueH<=k'); req(B<=E,'A U odd: B<=E')
                req(2*n<=len(PA)+len(PB)-s*m+B and 2*n<=w*m+E+B,'A U odd: summed bounds')
                if w>=1: req(k*t>=2*n-B+s*cdiv(2*n-E-B,w),'A U odd: (O)')
            else:
                s=Dp-H; w=2*H-Dp
                req(k*t>=2*n+2*s*cdiv(n,w),'A U even: (E)')
    return Tsurv

if __name__=='__main__':
    pass
