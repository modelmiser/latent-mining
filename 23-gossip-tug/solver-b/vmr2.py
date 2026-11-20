import subprocess,sys,os
# V(m,r): value of position: saturated group of m agents (each knows exactly the group) + r fresh agents. exact if "NO" line seen.
def V(m,r,lg):
    n=m+r
    rows=[format((1<<m)-1,'x')]*m+[format(1<<i,'x') for i in range(m,n)]
    lo=max(1,2*n-6)
    env=dict(os.environ); env['START']=' '.join(rows)
    try:
        out=subprocess.run(['./ab2_v3',str(n),str(lo),'70',str(lg)],env=env,capture_output=True,text=True,timeout=float(sys.argv[2])).stdout
    except subprocess.TimeoutExpired as e:
        out=(e.stdout or b'').decode() if isinstance(e.stdout,bytes) else (e.stdout or '')
    yes=[int(l.split('val>=')[1].split()[0]) for l in out.splitlines() if 'YES' in l]
    no=[int(l.split('val>=')[1].split()[0]) for l in out.splitlines() if '? NO' in l]
    return (max(yes) if yes else None, 'exact' if no else 'lower bound only', ('first test already NO' if no and not yes else ''))
for pair in sys.argv[3:]:
    m,r=map(int,pair.split(','))
    print(f"V({m},{r}) n={m+r}:",V(m,r,int(sys.argv[1])),flush=True)
