import subprocess,sys
# every single-line deletion of a losing certificate must be rejected (if the set is exactly the reachable closure)
for src in ['../solver-a/lose_E_h3_k0.txt','../solver-a/lose_D_h3_k1.txt','../solver-b/wit/E_h3_k0.txt','../solver-b/wit/D_h3_k1.txt']:
    l=open(src).read().split('\n'); body=[i for i in range(1,len(l)) if l[i].strip()]; rej=0
    for i in body:
        open('mut/tmp_drop.txt','w').write('\n'.join(l[:i]+l[i+1:]))
        out=subprocess.run(['python3','wcheck.py','mut/tmp_drop.txt'],capture_output=True,text=True).stdout
        rej+= 'REJECT' in out
    print(src, 'single-line deletions rejected: %d of %d'%(rej,len(body)))
