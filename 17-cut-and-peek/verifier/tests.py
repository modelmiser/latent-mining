import itertools, cmath, math, sys
from collections import Counter
from vcheck import check, parse_a
A='../solver-a/'
o,u0,u1,R,n,m = parse_a(A+'w_n3_m36.txt')
print("witness m36:", check(o,u0,u1,R))
flip=lambda s,p: s[:p]+('1' if s[p]=='0' else '0')+s[p+1:]
# --- broken witnesses: each must be rejected
broken = {
 'u1 card flipped at 0': (o,u0,flip(u1,0),R),
 'house card (pos 6) flipped in u0 only': (o,flip(u0,6),u1,R),
 'house card (pos 6) flipped in both': (o,flip(u0,6),flip(u1,6),R),
 'R missing 30': (o,u0,u1,R[:-1]),
 'R shifted one element 6->7': (o,u0,u1,[0,7,12,18,24,30]),
 'R = Z_36': (o,u0,u1,list(range(36))),
 'owner of pos 7 changed 1->2': (o[:7]+'2'+o[8:],u0,u1,R),
 'two positions swapped (0,1) in u0,u1,owner': (o[1]+o[0]+o[2:],u0[1]+u0[0]+u0[2:],u1[1]+u1[0]+u1[2:],R),
 'player 3 made constant': (o,u0,''.join(u0[p] if o[p]=='3' else u1[p] for p in range(m)),R),
 'empty R': (o,u0,u1,[]),
}
nrej=0
for k,v in broken.items():
    ok,why=check(*v); print("  broken [%s]: ok=%s (%s)"%(k,ok,why)); nrej+= (not ok)
print("rejected %d of %d broken"%(nrej,len(broken)))
# symmetric variants must still be accepted (checker not over-strict)
rotk=lambda s,k: s[-k:]+s[:-k]
print("rotated by 5 (cards, owners and R):", check(rotk(o,5),rotk(u0,5),rotk(u1,5),[(q+5)%m for q in R]))
print("R translated by 1 only:", check(o,u0,u1,[(q+1)%m for q in R]))
print("u0<->u1 swapped (should FAIL: computes NOR-like, privacy on wrong set):", check(o,u1,u0,R))
# --- structural facts about the witness vs the solvers' lemmas
D={i:[p for p in range(m) if o[p]==i and u0[p]!=u1[p]] for i in '123'}
P={i:[p for p in range(m) if o[p]==i] for i in '123'}
print("|P_i|",{i:len(P[i]) for i in P},"|D_i|",{i:len(D[i]) for i in D},"H",o.count('0'))
print("hearts u0 on D_i",{i:sum(u0[p]=='1' for p in D[i]) for i in D})
def row(x): return [int((u1 if (o[p]!='0' and x[int(o[p])-1]) else u0)[p]) for p in range(m)]
print("heart counts all 8 rows:",[sum(row(x)) for x in itertools.product((0,1),repeat=3)])
print("every translate of R meets every D_i:", all(any((q-j)%m in D[i] for q in R) for j in range(m) for i in D))
# strands: necklace classes
def neck(s): return min(tuple(s[k:]+s[:k]) for k in range(len(s)))
for x in itertools.product((0,1),repeat=3):
    w=row(x); print("  x",x,"strand necklaces:",sorted(''.join(map(str,neck([w[s+6*k] for k in range(6)]))) for s in range(6)))
# AND-0 rows rotations of each other? (T2 hypothesis must fail)
w0=row((0,0,0)); rots={tuple(w0[k:]+w0[:k]) for k in range(m)}
print("AND-0 rows that are rotations of w(000):",[x for x in itertools.product((0,1),repeat=3) if tuple(row(x)) in rots])
# T1: exists S subset R with C_S(1)!=C_S(0); all |S|<3 have equal C_S; delta divisible by 8
def C(S,x):
    w=row(x); return sum(math.prod(1-2*w[(q+j)%m] for q in S) for j in range(m))
bad=[];deltas=Counter()
for k in range(0,7):
    for S in itertools.combinations(R,k):
        vals=[C(S,x) for x in itertools.product((0,1),repeat=3)]
        if len(set(vals[:7]))!=1: bad.append(S)
        d=vals[7]-vals[0]; deltas[(k,d)]+=1
        if d!=0:
            # parity classes all nonempty?
            vs={tuple(sum(((q+j)%m in D[i]) for q in S)%2 for i in '123') for j in range(m)}
            if len(vs-{(0,0,0)})!=7: print("T1 VIOLATION",S)
print("T1: private C_S violations:",bad," (|S|,delta) histogram:",dict(deltas))
# --- L4 family (solver A): m>=5
for mm in range(4,26):
    ow=['0']*mm; a=['0']*mm
    ow[0]='1'; ow[mm-2]='1'; ow[mm-3]='2'; ow[mm-1]='2'; a[mm-2]='1'; a[mm-1]='1'
    b=[('1' if a[p]=='0' else '0') if ow[p]!='0' else a[p] for p in range(mm)]
    print("L4 m=%d:"%mm, check(''.join(ow),''.join(a),''.join(b),list(range(mm)))[0], end='; ')
print()
# --- L5': b copies of five-card trick
o5,a5,b5,R5,_,_=parse_a(A+'w_n2_m5.txt')
for bb in range(1,6):
    mm=5*bb; O=[None]*mm;U0=[None]*mm;U1=[None]*mm
    for s in range(bb):
        for k in range(5): O[s+bb*k]=o5[k];U0[s+bb*k]=a5[k];U1[s+bb*k]=b5[k]
    print("L5' b=%d:"%bb, check(''.join(O),''.join(U0),''.join(U1),[bb*q for q in R5])[0], end='; ')
print()
# --- T2/L6 algebra, non-vacuous numeric test: for every m<=40 and all theta in Z_m^3, every phi_ik with
# w^th_i + w^th_k = 1 + w^phi_ik  (three pairs) must give w^th1+w^th2+w^th3-2 = w^(phi12+phi13+phi23-th1-th2-th3)
tot=0;viol=0;excB=0
for mm in range(2,41):
    w=[cmath.exp(2j*math.pi*k/mm) for k in range(mm)]
    sol={}
    for a_ in range(mm):
        for b_ in range(a_,mm):
            sol[(a_,b_)]=[p for p in range(mm) if abs(w[a_]+w[b_]-1-w[p])<1e-9]
    g=lambda a_,b_: sol[(min(a_,b_),max(a_,b_))]
    for t1 in range(mm):
        for t2 in range(mm):
            for t3 in range(mm):
                for p12 in g(t1,t2):
                    for p13 in g(t1,t3):
                        for p23 in g(t2,t3):
                            tot+=1
                            lhs=w[t1]+w[t2]+w[t3]-2; rhs=w[(p12+p13+p23-t1-t2-t3)%mm]
                            if abs(lhs-rhs)>1e-9: viol+=1
                            if abs(lhs+1)<1e-9 and sum(t==0 for t in (t1,t2,t3))==1: excB+=1
print("T2 frequency identity: %d solution tuples (of which %d 'antipodal/type B'), violations: %d"%(tot,excB,viol))
