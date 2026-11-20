#!/usr/bin/env python3
# Build a witness file from a strand list (lines "cword ownerword"). usage: mkwit.py n strandfile outfile
import sys
n=int(sys.argv[1]); st=[l.split() for l in open(sys.argv[2]) if l.strip()]
b=len(st); a=len(st[0][0]); m=a*b
owner=['0']*m; u0=['0']*m; u1=['0']*m
for s,(cw,ow) in enumerate(st):
    for k in range(a):
        p=s+b*k; owner[p]=ow[k]; u0[p]=cw[k]; u1[p]=cw[k] if ow[k]=='0' else str(1-int(cw[k]))
open(sys.argv[3],"w").write(f"# {b} strands of length {a}; strand s occupies positions s+{b}k; R = multiples of {b}\nn {n}\nm {m}\nowner {''.join(owner)}\nu0 {''.join(u0)}\nu1 {''.join(u1)}\nR {','.join(str(b*k) for k in range(a))}\n")
