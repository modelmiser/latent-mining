import subprocess,sys
# V(m,r): value of the position with a saturated group of m agents (each knows exactly the group) and r fresh agents
def V(m,r):
    n=m+r
    rows=[format((1<<m)-1,'x')]*m+[format(1<<i,'x') for i in range(m,n)]
    out=subprocess.run(['./ab2',str(n),'1','60','24'],env={'START':' '.join(rows)},capture_output=True,text=True).stdout
    last=[l for l in out.splitlines() if 'YES' in l]
    return int(last[-1].split('val>=')[1].split()[0]) if last else 0
N=int(sys.argv[1])
print('rows m=1..; cols r=1..')
for m in range(1,N):
    print(m,[V(m,r) for r in range(1,N-m+1)],flush=True)
