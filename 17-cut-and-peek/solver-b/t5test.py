# T5 test: in each witness line (stdin), every translate of R must meet every player's set of flipping positions.
import sys
for line in sys.stdin:
    if 'n=' not in line or line.startswith('#'): continue
    kv=dict(t.split('=') for t in line.split() if '=' in t); n=int(kv['n']); m=int(kv['m'])
    R=[q for q in range(m) if kv['R'][q]=='1']
    ok=all(any(kv['owner'][(q-j)%m]==str(i) and kv['c0'][(q-j)%m]!=kv['c1'][(q-j)%m] for q in R) for j in range(m) for i in range(n))
    print('T5 holds' if ok else 'T5 VIOLATED', '|', line.strip()[:60])
