import itertools,subprocess,sys
rmax=int(sys.argv[1]); prune=sys.argv[2]
B={}
for l in open('../solver-b/Mtable.txt'):
    p=l.split()
    if len(p)==3: B[(p[0],int(p[1]))]=int(p[2])
diff=0
for r in range(rmax+1):
    hist={}
    for L in range(r+2):
        for b in itertools.product('01',repeat=L):
            w=''.join(b) or '-'
            m=None
            for n in range(0,r+3):
                rc=subprocess.run(['./vsolve',str(n),str(r),'S',w,'mw_%s_r%d.txt'%(w,r),'-1',prune],capture_output=True).returncode
                if rc==0: m=n;break
                assert rc==1
            # monotone sanity: also n+1 survives
            hist[m]=hist.get(m,0)+1
            if B.get((w,r))!=m: diff+=1; print('DIFF',w,r,m,B.get((w,r)))
    print('r',r,hist,flush=True)
print('diffs vs solver-b Mtable:',diff)
