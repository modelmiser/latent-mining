import glob,re,os,sys
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from chk import *
B=''
claim={}
for ln in """2:2 2 2
3:2 4 2 2
4:4 4 4 4 4
5:6 8 8 8 6 6
6:10 12 13 12 12 10 10
7:16 20 21 22 20 20 16 16
8:30 32 37 38 38 35 34 30 30""".split('\n'):
    n,v=ln.split(':'); 
    for t,x in enumerate(v.split()): claim[(int(n),t)]=int(x)
claim[(9,3)]=67; claim[(9,4)]=67; claim[(9,1)]=60; claim[(9,7)]=60
bad=0;cnt=0
for f in sorted(glob.glob(B+'solver-a/wit_*.txt')+glob.glob(B+'solver-b/wit_*.txt')):
    m=re.search(r'(\d+)_t?(\d+)\.txt$',f); n,t=int(m.group(1)),int(m.group(2))
    r=check(n,t,load(f),claim[(n,t)]); cnt+=1
    if not r.startswith('OK') or int(r.split()[1])!=claim[(n,t)]: bad+=1; print('PROBLEM',f,r)
print('checked',cnt,'witness files; problems:',bad)
# broken witnesses
w=load(B+'solver-a/wit_6_2.txt')
tests={}
x=list(w); x[1]=('00','1100','0101'); tests['b changed in one strategy']=x
x=list(w); x[12]=('11','1110','1111'); tests['a changed']=x
x=list(w)+[('10','1111','1111')]; tests['14th strategy added']=x
x=list(w); x[3]=x[2]; tests['duplicate strategy']=x
x=list(w); x[0]=('000','000','000'); tests['wrong split lengths']=x
x=list(w)[:12]; tests['only 12 (need 13)']=x
x=list(w); x[5]=('01','1100','1010'[::-1]); tests['b reversed 0101']=x
for k,v in tests.items(): print('broken:',k,'->',check(6,2,v,13))
for f in ['solver-a/bad_wit_6_2.txt','solver-a/bad2_wit_6_2.txt','solver-b/BAD_wit_n6_t2.txt']:
    print(f,check(6,2,load(B+f),13))
print('same witness set in a and b:',set(load(B+'solver-a/wit_6_2.txt'))==set(load(B+'solver-b/wit_min_n6_t2.txt')))
