import re,sys
# analyse SA layouts from opt4_sa.log: holes in zone, hitters (greedy), deficits
for line in open('opt4_sa.log'):
    m=re.search(r'SA best found: final extent (\d+).*lay=(\S+)',line)
    if not m: continue
    lay=m.group(2); fin=int(m.group(1))
    z=lay.index('2'); zone=lay[:z]; M=len(zone) if len(zone)%12==0 else None
    holes={}
    for h in re.findall(r'\.+',zone): holes[len(h)]=holes.get(len(h),0)+1
    n1zone=zone.count('1'); n1up=lay[z:].count('1')
    # greedy hitters
    o=[{'.':0,'1':1,'2':2}[c] for c in lay]; keep=[0]*len(o); 
    for r in range(2,len(o)):
        w=o[r-2:r+1]
        if 2 in w or not any(w) or any(keep[r-2:r+1]): continue
        j=r
        while o[j]!=1: j-=1
        keep[j]=1
    h=sum(keep[:z]); 
    print(f"zone={len(zone)} final={fin} zone1s={n1zone} moved_up={n1up} holes={sorted(holes.items())} hitters_in_zone={h} (baseline {len(zone)//3})")
