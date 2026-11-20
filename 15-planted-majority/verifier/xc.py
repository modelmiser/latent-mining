import random,subprocess,ref,os
random.seed(7); bad=0; dist={}
for it in range(400):
    s=random.choice([2,3,3,4]); w=''.join(random.choice('AB') for _ in range(s))
    rules=[]
    for p in range(s):
        for q in range(s):
            if random.random()<0.5: rules.append((p,q,random.randrange(s),random.randrange(s)))
    if it%3==0: # bias toward sensible: mutate the brief protocol
        s=3; w='ABA'; rules=[(0,1,2,2),(1,0,2,2),(0,2,0,0),(2,0,0,0),(1,2,1,1),(2,1,1,1)]
        k=random.randrange(len(rules)+1)
        if k<len(rules): rules[k]=(rules[k][0],rules[k][1],random.randrange(3),random.randrange(3))
    open('w/x.txt','w').write(f"{s} 0 1\n{w}\n"+''.join("%d %d %d %d\n"%r for r in rules))
    N=5
    t,_=ref.thresh(ref.load('w/x.txt'),N)
    o=subprocess.run(['./chk','w/x.txt',str(N)],capture_output=True,text=True).stdout
    t2=int(o.split('<=a+b<=')[1].split()[0])
    dist[t]=dist.get(t,0)+1
    if t!=t2: bad+=1; print("MISMATCH",s,w,rules,t,t2)
print("mismatches",bad,"threshold distribution",dist)
