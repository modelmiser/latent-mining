// Referee reduced search. Uses only: (1) complement form (non-differing cards are house cards),
// (2) balance of each D_i (needed for privacy with any nonempty R), (3) privacy is inherited by subsets of R.
// mode sym=0: no symmetry, all R: prints private/weak/valid pair counts and the raw-equivalent weighted counts
//             sum (n+1)^{|H|}  (each house card of the complement form may be a constant card of any owner).
// mode sym=1: position 0 = player 1 with club, players numbered by first appearance, 0 in R.
// usage: red n m sym [part nparts]
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
int n,m,sym,part=0,nparts=1; int NX;
int own[40],crd[40];
ull nconf=0,npriv=0,nweak=0,nvalid=0; long double wpriv=0,wweak=0,wvalid=0; int minR=99,maxR=0; bool printed=false;
unsigned rows_[16]; unsigned FULL;
static inline unsigned rot(unsigned w,int d){ return ((w<<d)|(w>>(m-d)))&FULL; }
int Rl[40],Rn; unsigned Dok; // Dok bit d: pair {0,d} private
vector<unsigned> win[16];
double wt;
void evalR(){
  // exact privacy on R
  for(int x=0;x<NX;x++){
    for(int j=0;j<m;j++){ unsigned t=0;
      for(int k=0;k<Rn;k++){ int p=Rl[k]-j; if(p<0)p+=m; t|=((rows_[x]>>p)&1)<<k; }
      win[x][j]=t; }
    sort(win[x].begin(),win[x].end()); }
}
void growR(int last){
  evalR();
  for(int x=1;x<NX-1;x++) if(win[x]!=win[0]) return;
  npriv++; wpriv+=wt;
  if(win[NX-1]!=win[0]){ nweak++; wweak+=wt; }
  size_t a=0,b=0; bool common=false; auto&A=win[0]; auto&B=win[NX-1];
  while(a<A.size()&&b<B.size()){ if(A[a]==B[b]){common=true;break;} if(A[a]<B[b])a++; else b++; }
  if(!common){ nvalid++; wvalid+=wt; minR=min(minR,Rn); maxR=max(maxR,Rn);
    if(!printed){ printed=true; printf("FIRST VALID n=%d m=%d owner=",n,m); for(int p=0;p<m;p++)printf("%d",own[p]);
      printf(" c=");for(int p=0;p<m;p++)printf("%d",crd[p]); printf(" R=");for(int k=0;k<Rn;k++)printf("%d,",Rl[k]); printf("\n"); } }
  for(int q=last+1;q<m;q++){
    bool ok=true;
    for(int k=0;k<Rn&&ok;k++) if(!(Dok>>(q-Rl[k])&1)) ok=false;
    if(!ok) continue;
    Rl[Rn++]=q; growR(q); Rn--; }
}
void leaf(){
  // balance
  int cnt[8][2]; memset(cnt,0,sizeof cnt); int h=0;
  for(int p=0;p<m;p++){ if(own[p]) cnt[own[p]][crd[p]]++; else h++; }
  for(int i=1;i<=n;i++) if(cnt[i][0]==0||cnt[i][0]!=cnt[i][1]) return;
  nconf++;
  unsigned c=0,msk[8]={0};
  for(int p=0;p<m;p++){ c|=crd[p]<<p; if(own[p]) msk[own[p]]|=1u<<p; }
  for(int x=0;x<NX;x++){ unsigned w=c; for(int i=0;i<n;i++) if(x>>i&1) w^=msk[i+1]; rows_[x]=w; }
  Dok=0;
  for(int d=1;d<m;d++){ int v=__builtin_popcount(rows_[0]&rot(rows_[0],d)); bool ok=true;
    for(int x=1;x<NX-1;x++) if(__builtin_popcount(rows_[x]&rot(rows_[x],d))!=v){ok=false;break;}
    if(ok) Dok|=1u<<d; }
  wt=pow((double)(n+1),h);
  if(sym){ Rl[0]=0; Rn=1; growR(0); }
  else for(int q0=0;q0<m;q0++){ Rl[0]=q0; Rn=1; growR(q0); }
}
ull branchctr=0;
void rec(int p,int maxpl){
  if(p==m){ if(maxpl==n) leaf(); return; }
  if(p==3 && nparts>1){ if((branchctr++)%nparts!=(ull)part) return; }
  if(sym){
    if(p==0){ own[0]=1; crd[0]=0; rec(1,1); return; }
    for(int o=0;o<=min(n,maxpl+1);o++) for(int cc=0;cc<2;cc++){ own[p]=o; crd[p]=cc; rec(p+1,max(maxpl,o)); }
  } else {
    for(int o=0;o<=n;o++) for(int cc=0;cc<2;cc++){ own[p]=o; crd[p]=cc; rec(p+1,n); }
  }
}
int main(int argc,char**argv){
  n=atoi(argv[1]); m=atoi(argv[2]); sym=atoi(argv[3]); if(argc>5){part=atoi(argv[4]);nparts=atoi(argv[5]);}
  NX=1<<n; FULL=(m==32)?~0u:((1u<<m)-1); for(int x=0;x<NX;x++) win[x].resize(m);
  rec(0,0);
  printf("RED n=%d m=%d sym=%d part=%d/%d configs=%llu private=%llu weak=%llu valid=%llu minR=%d maxR=%d",n,m,sym,part,nparts,nconf,npriv,nweak,nvalid,minR==99?0:minR,maxR);
  if(!sym) printf(" rawequiv: private=%.0Lf weak=%.0Lf valid=%.0Lf",wpriv,wweak,wvalid);
  printf("\n");
}
