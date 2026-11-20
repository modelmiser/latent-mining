#!/usr/bin/env python3
# Computes M(w,r) = least m such that a scheme on m cells survives r ops starting from stack w with Q(start)=top(w) required
# (w ranges over all stacks of length <= r+1; a longer stack behaves like its top r+1 entries). Uses ./sat, bootstrapping Mtable.txt.
import subprocess,sys,itertools,os
rmax=int(sys.argv[1]); tab='Mtable.txt'
done={}
if os.path.exists(tab):
    for l in open(tab):
        w,r,m=l.split(); done[(w,int(r))]=int(m)
for r in range(rmax+1):
    for L in range(r+2):
        for bits in itertools.product('01',repeat=L):
            w=''.join(bits) or '-'
            if (w,r) in done: continue
            m=0
            while True:
                out=subprocess.run(['./sat',str(m),str(r),'S',w,'-',tab],capture_output=True,text=True)
                if out.returncode==0: break
                assert out.returncode==1,(out.returncode,out.stdout,out.stderr)
                m+=1
            done[(w,r)]=m
            open(tab,'a').write("%s %d %d\n"%(w,r,m))
    row={}
    for (w,rr),m in done.items():
        if rr==r: row.setdefault(m,[]).append(w)
    print("r=%d:"%r,{m:(len(v), v if len(v)<=8 else '...') for m,v in sorted(row.items())},flush=True)
