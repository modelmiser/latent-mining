# check_inf.py FILE -- checks a certificate that Q_k(n) is infinite.
# FILE: header "k K n N", then one state per line = sorted remaining obligations of alive candidates.
# Accept iff: root (0,...,0) is listed, no listed state has one candidate, and for every listed state
# and EVERY question (enumerated here over explicit subsets) some legal answer leads to a listed state.
# Relies on the obligation-multiset abstraction (proofs.txt, Lemma 1).
import sys, itertools
lines=[l.split() for l in open(sys.argv[1]) if l.strip() and not l.startswith('#')]
k=int(lines[0][1]); n=int(lines[0][3])
T=set(tuple(int(v) for v in l) for l in lines[1:])
assert (0,)*n in T, "REJECT root missing"
for st in T:
    assert len(st)>=2, "REJECT singleton state listed"
    m=len(st)
    for mask in range(1<<m):
        ins=[(mask>>i)&1 for i in range(m)]
        ell=min(sum(ins),m-sum(ins))
        ok=False
        for ans in (1,0):
            new=[]
            for r,i in zip(st,ins):
                if i==ans: new.append(max(r-1,0))      # truthful
                elif r==0: new.append(k*ell)           # lie allowed
            if new and tuple(sorted(new)) in T: ok=True
        if not ok: raise SystemExit("REJECT: state %s question %s escapes"%(st,ins))
print("ACCEPT %s: k=%d n=%d responder can stay inside %d listed states forever"%(sys.argv[1],k,n,len(T)))
