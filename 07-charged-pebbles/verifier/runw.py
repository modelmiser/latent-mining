import re,glob,os,sys
from wcheck import check
B=''
def rd(f): return open(f).read().split("\n")
bad=0
for f in sorted(glob.glob(B+'solver-a/witness/*.txt')+glob.glob(B+'solver-b/wit_*.txt')):
    m=re.search(r'(?:p(\d+)_k(\d+)|_(\d+)_(\d+))\.txt$',f)
    if 'BAD' in f or not m: print("SKIP",f); continue
    p,k=[int(x) for x in m.groups() if x is not None]
    n=(p*k+2)*2**(p-2); r=check(p,k,n,rd(f))
    r1=check(p,k,n+1,rd(f)); nm=sum(1 for l in rd(f) if l and l[0] in 'PRF')
    print(os.path.relpath(f,B),p,k,n,nm,"moves",r, "| n+1:",r1[:6]); bad+= r!="OK"
print("failures",bad)
# broken witnesses
print("A BAD F_as_R:",check(3,1,10,rd(B+'solver-a/witness/BAD_p3_k1_F_as_R.txt')))
print("A BAD extra :",check(2,1,4,rd(B+'solver-a/witness/BAD_p2_k1_extra.txt')), "| claimed n=5:",check(2,1,5,rd(B+'solver-a/witness/BAD_p2_k1_extra.txt')))
for g in glob.glob(B+'solver-b/neg/wit*'): print("B neg",os.path.basename(g),check(3,1,10,rd(g)))
w=[l for l in rd(B+'solver-b/wit_strat_3_1.txt') if l and l[0] in 'PRF']
def mut(name,ls,p=3,k=1,n=10): print("mine:",name,check(p,k,n,ls))
mut("unmodified",w)
i=[j for j,l in enumerate(w) if l[0]=='F'][0]
mut("F->R",w[:i]+['R'+w[i][1:]]+w[i+1:])
mut("first R->F",[('F'+l[1:]) if j==[x for x,y in enumerate(w) if y[0]=='R'][0] else l for j,l in enumerate(w)])
mut("k=0",w,k=0)
mut("line deleted",w[:5]+w[6:])
mut("P to node+1 (skip)",[w[0],"P 3 1"]+w[2:])
mut("duplicate place",[w[0],w[0]]+w[1:])
mut("p=2",w,p=2)
mut("trunc before last F",w[:max(j for j,l in enumerate(w) if l[0]=='F')])
mut("4 F-moves on k=1 p=3 (append P 11 0, F 10 0)",w+["P 11 0","F 10 0"],n=11)
