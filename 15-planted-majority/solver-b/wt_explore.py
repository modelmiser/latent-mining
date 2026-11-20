# exploration (NOT exhaustive): weight-conserving protocols only. For each weight vector u (u0=K,u1=-K), outputs = sign,
# zero-weight states get every output pattern (sorted). Calls search4 with a short timeout. Prints hits.
import itertools, subprocess, sys
s=int(sys.argv[1]); N=int(sys.argv[2]); Kmax=int(sys.argv[3]); tl=sys.argv[4]
seen=set(); hits=0; tried=0; timeouts=0
for K in range(1,Kmax+1):
    for rest in itertools.combinations_with_replacement(range(K,-K-1,-1), s-2):
        # A/B symmetry: canonical under negation
        neg=tuple(sorted((-v for v in rest),reverse=True))
        if neg<rest: continue
        u=(K,-K)+rest
        zeros=[i for i,v in enumerate(u) if v==0]
        for nb in range(len(zeros)+1):
            mask=0
            for i,v in enumerate(u):
                if v<0: mask|=1<<i
            for i in zeros[len(zeros)-nb:]: mask|=1<<i
            tried+=1
            try:
                r=subprocess.run(['nice','-n','15','timeout',tl,'./search4',str(s),str(N),'0','1','0',str(mask),','.join(map(str,u))],capture_output=True,text=True)
            except Exception as e: print(e); continue
            if 'SOL' in r.stdout:
                hits+=1; print('HIT u=%s mask=%d :: %s'%(u,mask,[l for l in r.stdout.split('\n') if l.startswith('SOL')][0])); sys.stdout.flush()
            elif 'RESULT' not in r.stdout: timeouts+=1
print('s=%d N=%d Kmax=%d tried=%d hits=%d timeouts=%d'%(s,N,Kmax,tried,hits,timeouts))
