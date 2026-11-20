# convert solver formats to referee format. usage: conv.py a|b infile outfile
import sys,re
kind,fi,fo=sys.argv[1:4]
if kind=='a':
    line=[l for l in open(fi) if 's=' in l and 'd:' in l][0]
    s=int(re.search(r's=(\d+)',line).group(1)); qA=int(re.search(r'qA=(\d+)',line).group(1)); qB=int(re.search(r'qB=(\d+)',line).group(1))
    w=re.search(r'w=([AB]+)',line).group(1)
    rules=re.findall(r'(\d+),(\d+)->(\d+),(\d+)',line.split('d:')[1])
else:
    L=[l.split('#')[0].strip() for l in open(fi)]; L=[l for l in L if l]
    s=int(L[0]); qA,qB=L[1].split(); w=L[2]; rules=[l.split() for l in L[3:]]
with open(fo,'w') as f:
    f.write(f"{s} {qA} {qB}\n{w}\n"); 
    for r in rules: f.write(' '.join(r)+"\n")
