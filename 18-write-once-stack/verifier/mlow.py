import itertools,subprocess,sys,time
r=int(sys.argv[1]); res={}
t0=time.time()
for L in range(r+2):
    for b in itertools.product('01',repeat=L):
        w=''.join(b) or '-'
        rc=subprocess.run(['timeout','60','./vsolve',str(r),str(r),'S',w],capture_output=True).returncode
        res[rc]=res.get(rc,0)+1
        if rc!=1: print('NOT-IMPOSSIBLE',w,rc,flush=True)
print('r',r,'n=r cells: returncodes (1=IMPOSSIBLE,0=SURVIVES,124=timeout)',res,'%.0fs'%(time.time()-t0))
