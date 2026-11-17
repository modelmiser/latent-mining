import sys,random
sg=lambda v:(v>0)-(v<0)
def trace(d,ws):
    r=(d-1)//2
    F={0};R=set();U=[0]*d; y=0; T=0; W=0; prevw=0
    print(' c  w |  W   T   P | o | F-hull R-hull (abs) | u(T move)')
    for c,w in enumerate(ws):
        if prevw!=0: w=0
        # belief already conditioned on obs
        Y=F|R; lo=min(Y);hi=max(Y); u=-sg(lo+hi)
        S=sum(U); P=T-(-S)  # T_{c-d}=T_c - (T_c-T_{c-d}); T_c-T_{c-d} = -S
        P=T+S
        f=lambda s:'[%3d,%3d]'%(min(s)+T,max(s)+T) if s else '    .    '
        print('%2d %2d | %3d %3d %3d | %s | %s %s | %+d   %s'%(c,w,W,T,P,'<=>'[sg(W-P)+1],f(F),f(R),-u,'X' if abs(y+u)>r else ''))
        z=y+u; U=U[1:]+[u]; S=sum(U); T-=u
        F2={v+u for v in Y}; R2={v+u+1 for v in F}|{v+u-1 for v in F}
        y=z+w; W+=w; prevw=w
        o=sg(y-S)
        F={v for v in F2 if sg(v-S)==o}; R={v for v in R2 if sg(v-S)==o}
        assert (y in (R if w else F))
d=int(sys.argv[1]); ws=eval(sys.argv[2])
trace(d,ws)
