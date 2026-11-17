// slow.cpp : direct-from-definition reference. Reads lines "code blindT T_0..T_dmax" (from wdfull) on stdin,
// recomputes by brute force on explicit histories with horizon Tmax: "winnable within Tmax" must equal (1<=T<=Tmax),
// and the brute-force minimal T must equal T when T<=Tmax.
// usage: slow n dmax Tmax
#include <bits/stdc++.h>
using namespace std;
int n,dmax,Tmax,d; int A[2][8];
int xs[40], ys[40];
// image of Q under x_1 y_1 ... y_{T-1} x_T, applied state by state
bool single(int T){ int first=-1; for(int s=0;s<n;s++){ int p=s; for(int t=1;t<=T;t++){ p=A[xs[t]][p]; if(t<T) p=A[ys[t]][p]; } if(first<0) first=p; else if(p!=first) return false; } return true; }
bool allsingle(int T,int from){ // ys[from..T-1] free
  if(from>T-1) return single(T);
  for(int y=0;y<2;y++){ ys[from]=y; if(!allsingle(T,from+1)) return false; } return true; }
// Synchronizer to choose x_t knowing ys[1..t-1-d]; returns minimal T (uniform bound) or 0
int rec(int t){ int best=0;
  for(int x=0;x<2;x++){ xs[t]=x; int v=0;
    if(allsingle(t,max(1,t-d))) v=t;
    else if(t<Tmax){
      if(t-d<1) v=rec(t+1);
      else { int m=0; bool ok=true; for(int y=0;y<2&&ok;y++){ ys[t-d]=y; xs[t]=x; int r=rec(t+1); if(!r) ok=false; else m=max(m,r);} v= ok?m:0; }
    }
    if(v && (!best||v<best)) best=v; }
  return best; }
int blindslow(){ for(int T=1;T<=Tmax;T++) for(int w=0;w<(1<<T);w++){ for(int t=1;t<=T;t++) xs[t]=(w>>(t-1))&1; if(allsingle(T,1)) return T; } return 0; }
int main(int argc,char**argv){ n=atoi(argv[1]); dmax=atoi(argv[2]); Tmax=atoi(argv[3]); long long code; long long lines=0,bad=0,winc=0,losec=0,beyond=0;
  while(scanf("%lld",&code)==1){ int bl; scanf("%d",&bl); vector<int> T(dmax+1); for(auto&v:T) scanf("%d",&v);
    long long c=code; for(int x=0;x<2;x++) for(int i=0;i<n;i++){A[x][i]=c%n;c/=n;}
    lines++; int b2=blindslow(); int exp= (bl>=1&&bl<=Tmax)?bl:0; if(b2!=exp){bad++; printf("BLIND MISMATCH code=%lld fast=%d slow=%d\n",code,bl,b2);} 
    for(d=0;d<=dmax;d++){ int r=rec(1); int e=(T[d]>=1&&T[d]<=Tmax)?T[d]:0; if(r!=e){bad++; printf("MISMATCH code=%lld d=%d fast=%d slow=%d\n",code,d,T[d],r);} if(e)winc++; else if(T[d]) beyond++; else losec++; } }
  printf("n=%d dmax=%d Tmax=%d automata=%lld mismatches=%lld (delay cases: win<=Tmax %lld, fast-win beyond horizon %lld, fast-loss %lld)\n",n,dmax,Tmax,lines,bad,winc,beyond,losec); }
