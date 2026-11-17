# cmp.py file : compare "k=.. n=.. Q=.." lines with Pelc's formula for U1
import sys,re
from u1 import pelc
bad=[];cnt=0
for l in open(sys.argv[1]):
    m=re.match(r'k=(\d+) n=(\d+) Q(=|>)(\d+)',l)
    if not m: continue
    cnt+=1
    n=int(m.group(2)); 
    if m.group(3)=='>' or int(m.group(4))!=pelc(n): bad.append((n,l.split('(')[0].strip(),pelc(n)))
print(cnt,"lines; differing from U1:",len(bad)); print(bad[:60])
