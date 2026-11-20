import itertools,subprocess,sys,os,time
lo,hi=int(sys.argv[1]),int(sys.argv[2]); t0=time.time()
ws=[''.join(b) or '-' for L in range(8) for b in itertools.product('01',repeat=L)]
ok=0
for w in ws[lo:hi]:
    if time.time()-t0>540: print('time budget hit at',w); break
    f='m6/w_%s_r6.txt'%w
    if not os.path.exists(f):
        rc=subprocess.run(['timeout','120','../solver-b/sat','7','6','S',w,f],capture_output=True).returncode
        if rc!=0: print('sat rc',rc,w); continue
    r=subprocess.run(['python3','vcheck.py',f,'S'],capture_output=True,text=True)
    hdr=open(f).read().split('\n')[1:4]
    if r.returncode==0 and hdr==['n 7','t 6','init '+w]: ok+=1
    else: print('BAD',w,r.stdout)
print('ok',ok,'of',len(ws[lo:hi]),'total stacks',len(ws),'%.0fs'%(time.time()-t0))
