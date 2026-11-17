#!/usr/bin/env python3
"""Independent upper-bound checker: verifies that a cert file is a fixed point of the Bellman equation
value(state) = max over attempts of the sum over answers of value(successor); terminal (all output) = 1; no useful attempt = 0.
usage: check_cert.py k n certfile ; prints the certified N(k,n) or REJECT."""
import sys
k=int(sys.argv[1]); n=int(sys.argv[2])
val={}
for line in open(sys.argv[3]):
    r,st,v=[x.strip() for x in line.split('|')]
    val[(int(r),tuple('' if s.strip()=='-' else s.strip() for s in st.split(';')))]=int(v)
def clear(st): return tuple('0'*len(s) for s in st)
bad=0
for (r,st),v in val.items():
    if r==0 and all(s=='' for s in st):
        if v!=1: bad+=1
        continue
    opts=[]
    if r>0 and len(st[0])<2:
        opts.append([(r-1,(st[0]+'0',)+st[1:])])
    for i in range(k-1):
        if st[i] and len(st[i+1])<2:
            t=list(st); t[i+1]+=t[i][-1]; t[i]=t[i][:-1]; opts.append([(r,tuple(t))])
    for i in range(k):
        if st[i] and st[i][-1]=='0':
            t=list(st); t[i]=t[i][:-1]; f=list(st); f[i]=f[i][:-1]+'1'
            opts.append([(r,clear(t)),(r,tuple(f))])
    try:
        best=max([sum(val[s] for s in o) for o in opts],default=0)
    except KeyError as e:
        print('REJECT missing state',e); sys.exit(1)
    if best!=v: bad+=1
if bad: print('REJECT: %d states violate the Bellman equation'%bad); sys.exit(1)
print('cert OK: N(%d,%d) = %d  (%d states)'%(k,n,val[(n,tuple('' for _ in range(k)))],len(val)))
