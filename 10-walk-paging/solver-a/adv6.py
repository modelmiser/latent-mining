import adv
m=6; row=[]
for k in range(2,14):
    v,n=adv.play(k,m); t=max(m+1,2*m-k//2+1)
    row.append("%s%s"%(v,"" if v==t else "(!=%d)"%t)); print("ADV m=6 k=%d: %s nodes %d"%(k,row[-1],n),flush=True)
