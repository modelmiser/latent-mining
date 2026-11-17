# builds referee's own copies of the witness families and broken ones (independent construction)
def W(n):
    w=[]
    for k in list(range(1,n))+[n-1]: w+=list(range(k,0,-1))
    return w
def OET(n,r,first):
    w=[]
    for t in range(r): w+=list(range(1+(t+first)%2,n,2))
    return w
good=open('wit_good.txt','w'); bad=open('wit_bad.txt','w')
for n in range(3,13):
    good.write(f"{n}: {' '.join(map(str,W(n)))}\n")
    for f in (0,1): good.write(f"{n}: {' '.join(map(str,OET(n,n+2,f)))}\n")
for n in range(3,10):
    w=W(n)
    B=[w[:-1], w[1:], w[:len(w)//2]+w[len(w)//2+1:],            # one letter missing (end, start, middle)
       w[:-(n-1)],                                              # plain insertion sort
       w[:-1]+[2],                                              # last letter slipped (sorts, not slip-proof)
       OET(n,n+1,0), OET(n,n+1,1), OET(n,n,0),
       list(reversed(range(1,n)))*((n+2)//2+1)]                 # wrong order of passes, long enough
    for b in B: bad.write(f"{n}: {' '.join(map(str,b))}\n")
bad.write("4: 1 2 1 3 2 1 3 2 4\n")   # letter outside the alphabet
bad.write("2: 1\n")                   # n=2 not allowed
