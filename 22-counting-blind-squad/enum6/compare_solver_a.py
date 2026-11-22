import sys,itertools,re
mine={l.split()[3][2:] for l in open(sys.argv[2])}
def perm(d, pi):
    full=[0]+[int(c) for c in d]; rows=[full[i:i+3] for i in range(0,15,3)]
    m={0:0,1:1,2:2,3:pi[0],4:pi[1],5:pi[2]}
    new={0:rows[0],1:rows[1]}
    for i,x in enumerate((3,4,5)): new[m[x]]=rows[2+i]
    out=[m[v] for x in (0,1,3,4,5) for v in new[x]]
    return ''.join(map(str,out[1:]))
exp=set(); n=0
for l in open(sys.argv[1]):
    m=re.search(r'dfs L=8 rule.*\]:\s*([-\d ]+?)\s+times',l)
    if not m: continue
    v=m.group(1).split(); n+=1
    free=[i for i,x in enumerate(v) if x=='-1']
    for vals in itertools.product('012345',repeat=len(free)):
        w=v[:]
        for i,x in zip(free,vals): w[i]=x
        assert w[0]=='0'
        d=''.join(w[1:])
        for pi in itertools.permutations((3,4,5)): exp.add(perm(d,pi))
print('leaves',n,'expanded',len(exp),'equal to enum list',exp==mine, len(exp-mine), len(mine-exp))
