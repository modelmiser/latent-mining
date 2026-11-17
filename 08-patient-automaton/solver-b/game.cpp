// Delayed synchronization game. See brief. States 0..n-1, letters 0=a,1=b.
// Modes:
//   full n dmax        : all n^(2n) automata, no shortcuts; table |W_d|, |W_inf|; asserts monotonicity
//   sym n dmax [list]  : canonical reps under (state perms x letter swap), shortcuts by monotonicity
//   one n dmax a1..an b1..bn : one automaton (1-indexed targets); prints d-winnable, Tmin, blind T
//   dump n dmax        : per automaton line: code, blindT, Tmin(d) d=0..dmax   (rank mode, exact min T)
#include <bits/stdc++.h>
using namespace std;
typedef unsigned u32;
int n; int t[2][8]; u32 img[2][256]; u32 FULL;
void setup(){FULL=(1u<<n)-1;for(int x=0;x<2;x++)for(u32 S=0;S<=FULL;S++){u32 r=0;for(int q=0;q<n;q++)if(S>>q&1)r|=1u<<t[x][q];img[x][S]=r;}}
static inline bool single(u32 S){return (S&(S-1))==0;}
// ---- d-game, attractor on revealed image S with pending word w (d letters, oldest = bit 0)
vector<char> win;
bool dwin(int d){
  u32 NS=1u<<n, NW=1u<<d; win.assign((size_t)NS*NW,0);
  for(u32 w=0;w<NW;w++)for(u32 S=1;S<NS;S++)if(single(S))win[S|(w<<n)]=1;
  bool ch=true;
  while(ch){ch=false;
    for(u32 w=0;w<NW;w++)for(u32 S=1;S<NS;S++){ if(win[S|(w<<n)])continue;
      for(int x=0;x<2;x++){ bool ok=true;
        for(int y=0;y<2&&ok;y++){ u32 S2,w2; if(d==0){S2=img[y][img[x][S]];w2=0;} else {S2=img[y][img[w&1][S]];w2=(w>>1)|((u32)x<<(d-1));}
          if(!win[S2|(w2<<n)])ok=false;}
        if(ok){win[S|(w<<n)]=1;ch=true;break;}}
    }}
  for(u32 w=0;w<NW;w++)if(win[FULL|(w<<n)])return true;
  return false;
}
// ---- exact minimal T for the d-game (rank mode). terminal(S,w,x): all 2^d spoiler fills give singleton after x
bool allsingle(u32 S,u32 w,int left,int x){ if(left==0)return single(img[x][S]);
  u32 S1=img[w&1][S]; return allsingle(img[0][S1],w>>1,left-1,x)&&allsingle(img[1][S1],w>>1,left-1,x);}
int tmin(int d){ // returns minimal T (>= d+1 convention: values below d+1 reported as min blind-prefix T), 0 if none
  u32 NS=1u<<n, NW=1u<<d; const int INF=1e9; vector<int> R((size_t)NS*NW,INF);
  for(u32 w=0;w<NW;w++)for(u32 S=1;S<NS;S++)for(int x=0;x<2;x++)if(allsingle(S,w,d,x)){R[S|(w<<n)]=0;break;}
  bool ch=true;
  while(ch){ch=false;
    for(u32 w=0;w<NW;w++)for(u32 S=1;S<NS;S++){ int best=R[S|(w<<n)]; if(best==0)continue;
      for(int x=0;x<2;x++){ int m=0;
        for(int y=0;y<2;y++){ u32 S2,w2; if(d==0){S2=img[y][img[x][S]];w2=0;} else {S2=img[y][img[w&1][S]];w2=(w>>1)|((u32)x<<(d-1));}
          m=max(m,R[S2|(w2<<n)]);}
        if(m<INF&&m+1<best){best=m+1;}}
      if(best<R[S|(w<<n)]){R[S|(w<<n)]=best;ch=true;}
    }}
  int b=INF; for(u32 w=0;w<NW;w++)b=min(b,R[FULL|(w<<n)]);
  return b>=INF?0:b+d+1;
}
// ---- blind game: BFS over sets of unmerged pairs. returns minimal T, 0 if not blind-winnable
int pid[8][8]; int NP;
int blindT(){
  NP=0; for(int p=0;p<n;p++)for(int q=p+1;q<n;q++){pid[p][q]=pid[q][p]=NP++;}
  vector<u32> nx[2]; vector<char> mg[2]; for(int x=0;x<2;x++){nx[x].assign(NP,0);mg[x].assign(NP,0);}
  for(int p=0;p<n;p++)for(int q=p+1;q<n;q++)for(int x=0;x<2;x++){int i=pid[p][q];int p1=t[x][p],q1=t[x][q];
    if(p1==q1){mg[x][i]=1;continue;} for(int y=0;y<2;y++){int p2=t[y][p1],q2=t[y][q1]; if(p2!=q2)nx[x][i]|=1u<<pid[p2][q2];}}
  vector<char> seen(1u<<NP,0); vector<u32> cur,nxt; u32 all=(1u<<NP)-1; if(NP==0)return 1; cur.push_back(all);seen[all]=1; int T=1;
  while(!cur.empty()){ nxt.clear();
    for(u32 P:cur)for(int x=0;x<2;x++){ bool allm=true; u32 P2=0; for(int i=0;i<NP;i++)if(P>>i&1){ if(!mg[x][i])allm=false; P2|=nx[x][i]; }
      if(allm)return T; if(!seen[P2]){seen[P2]=1;nxt.push_back(P2);} }
    cur.swap(nxt); T++; }
  return 0;
}
void decode(long long code){for(int x=0;x<2;x++)for(int q=0;q<n;q++){t[x][q]=code%n;code/=n;}}
int main(int argc,char**argv){
  string mode=argv[1]; n=atoi(argv[2]); int dmax=atoi(argv[3]);
  long long N=1; for(int i=0;i<2*n;i++)N*=n;
  if(mode=="one"){ for(int x=0;x<2;x++)for(int q=0;q<n;q++)t[x][q]=atoi(argv[4+x*n+q])-1; setup();
    printf("blindT=%d\n",blindT()); for(int d=0;d<=dmax;d++){bool w=dwin(d); printf("d=%d win=%d",d,(int)w); if(d<=10)printf(" Tmin=%d",tmin(d)); printf("\n"); fflush(stdout);} return 0;}
  if(mode=="dump"){ for(long long c=0;c<N;c++){decode(c);setup();printf("%lld %d",c,blindT());for(int d=0;d<=dmax;d++)printf(" %d",tmin(d));printf("\n");} return 0;}
  if(mode=="full"){ vector<long long> cnt(dmax+1,0); long long cb=0, viol=0;
    for(long long c=0;c<N;c++){decode(c);setup(); bool b=blindT()>0; cb+=b; bool prev=true;
      for(int d=0;d<=dmax;d++){bool w=dwin(d); cnt[d]+=w; if(w&&!prev)viol++; if(b&&!w)viol++; prev=w;}}
    printf("n=%d total=%lld\n",n,N); for(int d=0;d<=dmax;d++)printf("|W_%d|=%lld\n",d,cnt[d]); printf("|W_inf|=%lld\nmonotonicity_violations=%lld\n",cb,viol); return 0;}
  if(mode=="sym"){ bool list=argc>4;
    vector<int> pm(n); iota(pm.begin(),pm.end(),0); vector<vector<int>> perms; do perms.push_back(pm); while(next_permutation(pm.begin(),pm.end()));
    vector<long long> cnt(dmax+1,0); long long cb=0, classes=0; vector<long long> lastd(dmax+2,0); // lastd[k]: non-blind, largest winnable d = k-1 (k=0: not 0-winnable)
    int cur[16],tr[16];
    for(long long c=0;c<N;c++){decode(c); for(int q=0;q<n;q++){cur[q]=t[0][q];cur[n+q]=t[1][q];}
      // canonical test: cur must be lexicographically minimal (compare from highest index down = code order irrelevant; any total order works)
      int stab=0; bool minimal=true;
      for(auto&p:perms){ for(int s=0;s<2&&minimal;s++){ for(int x=0;x<2;x++)for(int q=0;q<n;q++)tr[x*n+p[q]]=p[t[x^s][q]];
          int cmp=0; for(int i=0;i<2*n;i++){ if(tr[i]!=cur[i]){cmp=tr[i]<cur[i]?-1:1;break;} }
          if(cmp<0)minimal=false; else if(cmp==0)stab++; } if(!minimal)break; }
      if(!minimal)continue; classes++; long long wt=2*(long long)perms.size()/stab;
      setup(); if(!dwin(0)){lastd[0]+=wt;continue;}
      if(blindT()>0){cb+=wt; for(int d=0;d<=dmax;d++)cnt[d]+=wt; continue;}
      int d=0; cnt[0]+=wt; for(d=1;d<=dmax;d++){ if(dwin(d))cnt[d]+=wt; else break; }
      lastd[d]+=wt;
      if(list&&d>=atoi(argv[4])+1){ printf("CAND lastwin=%d a=",d-1); for(int q=0;q<n;q++)printf("%d",t[0][q]+1); printf(" b="); for(int q=0;q<n;q++)printf("%d",t[1][q]+1); printf(" orbit=%lld\n",wt);}
    }
    printf("n=%d total=%lld classes=%lld\n",n,N,classes); for(int d=0;d<=dmax;d++)printf("|W_%d|=%lld\n",d,cnt[d]); printf("|W_inf|=%lld\n",cb);
    printf("not 0-winnable: %lld\n",lastd[0]); for(int k=1;k<=dmax+1;k++)printf("non-blind, largest winnable d (<=%d) = %d : %lld\n",dmax,k-1,lastd[k]);
    return 0;}
}
