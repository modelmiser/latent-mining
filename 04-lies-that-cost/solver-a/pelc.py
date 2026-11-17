import sys,re
def pelc(n):
    q=0
    while True:
        if n%2==0 and n*(q+1)<=2**q: return q
        if n%2==1 and n*(q+1)+(q-1)<=2**q: return q
        q+=1
bad=[];cnt=0
for line in sys.stdin:
    m=re.match(r'k=(\d+) n=(\d+) Q=(-?\d+|INF)',line)
    if not m: continue
    n=int(m.group(2)); Q=m.group(3); cnt+=1
    if Q!=str(pelc(n)): bad.append((n,Q,pelc(n)))
print("checked",cnt,"mismatches vs Pelc U1:",bad)
