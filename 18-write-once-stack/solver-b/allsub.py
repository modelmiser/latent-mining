#!/usr/bin/env python3
# For every stack w with |w|<=r+1, r<=5: produce (./sat) and independently check (check.py, mode S) a scheme on r+1 cells surviving r ops from w.
import itertools,subprocess,os
ok=0
for r in range(6):
    for L in range(r+2):
        for b in itertools.product('01',repeat=L):
            w=''.join(b) or '-'; name='mwit/w_%s_r%d.txt'%(w,r)
            if not os.path.exists(name):
                assert subprocess.run(['./sat',str(r+1),str(r),'S',w,name],capture_output=True).returncode==0,(w,r)
            out=subprocess.run(['python3','check.py',name,'S'],capture_output=True,text=True)
            assert out.returncode==0 and out.stdout.startswith('OK'),(w,r,out.stdout)
            hdr=[l.split() for l in open(name).read().split('\n')[1:4]]
            assert hdr[0]==['n',str(r+1)] and hdr[1]==['t',str(r)] and hdr[2]==['init',w]
            ok+=1
print("all sub-witnesses checked:",ok)
