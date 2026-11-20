// fast3.cpp = fast2.cpp + also counts WEAK validity (privacy and M(1^n) != M(0^n) as multisets, supports may overlap).
// fast2.cpp: fast.cpp + clique pruning on R, all-players-triple filter, chunking. usage: fast2 n m [part K]
// Reductions (see proofs.txt): u_i^1 = complement of u_i^0 on P_i; |P_i| even and u_i^0 half hearts;
// position 0 owned by player 0 with card 0 (rotation, relabel, colour swap); players numbered by first occurrence;
// R contains 0 (translation); R subset of {0} u D where D = distances d at which pair privacy holds.
#include <bits/stdc++.h>
using namespace std;
int n,m,X; int owner[32],c0[32]; int cnt[8],hearts[8];
long long nleaf=0,nvalid=0,ncfg=0,ncfgfull=0; int minR=99,maxR=0; unsigned W[64]; unsigned FULL;
long long byR[33];
string firstW,minW,fullW,weakW; long long nweak=0,t5viol=0;
static inline unsigned rot(unsigned w,int j){j%=m; if(!j) return w; return ((w<<j)|(w>>(m-j)))&FULL;}
string wit(unsigned R){ char b[400]; string s; sprintf(b,"n=%d m=%d owner=",n,m); s=b;
  for(int p=0;p<m;p++) s+=(owner[p]<0?'H':char('0'+owner[p])); s+=" c0="; for(int p=0;p<m;p++) s+=char('0'+c0[p]);
  s+=" c1="; for(int p=0;p<m;p++) s+=char('0'+(owner[p]<0?c0[p]:1-c0[p])); s+=" R="; for(int p=0;p<m;p++) s+=((R>>p&1)?'1':'0'); return s;}
unsigned M0[32]; bool anyv,anyfull; unsigned Pm[8];
bool privacy(unsigned R){
  unsigned Mx[32];
  for(int j=0;j<m;j++) M0[j]=rot(W[0],j)&R; sort(M0,M0+m);
  for(int x=1;x<X-1;x++){ for(int j=0;j<m;j++) Mx[j]=rot(W[x],j)&R; sort(Mx,Mx+m); for(int j=0;j<m;j++) if(Mx[j]!=M0[j]) return false; }
  return true;
}
bool correct(unsigned R){ // M0 holds sorted words of row 0 for R
  for(int j=0;j<m;j++){unsigned t=rot(W[X-1],j)&R; if(binary_search(M0,M0+m,t)) return false;} return true;}
void dfsR(unsigned R,int last,unsigned D){
  // R is privacy-OK here; M0 valid for R
  { unsigned M1[32]; for(int j=0;j<m;j++) M1[j]=rot(W[X-1],j)&R; sort(M1,M1+m); bool df=false; for(int j=0;j<m;j++) if(M1[j]!=M0[j]) df=true; if(df){ nweak++; if(weakW.empty()) weakW=wit(R);} }
  if(correct(R)){ nvalid++; for(int j=0;j<m;j++) for(int i=0;i<n;i++) if(!(rot(R,j)&Pm[i])) {t5viol++; goto out;} out:; int k=__builtin_popcount(R); byR[k]++; anyv=true; if(firstW.empty()) firstW=wit(R);
    if(k<minR){minR=k;minW=wit(R);} maxR=max(maxR,k); if(R==FULL){anyfull=true; if(fullW.empty()) fullW=wit(R);} }
  for(int d=last+1;d<m;d++) if(D>>d&1){ bool cl=true; for(int r=1;r<d&&cl;r++) if((R>>r&1)&&!(D>>(d-r)&1)) cl=false; if(!cl) continue; unsigned R2=R|(1u<<d); if(privacy(R2)) dfsR(R2,d,D); }
}
void leaf(){
  for(int i=0;i<n;i++) if(cnt[i]==0||2*hearts[i]!=cnt[i]) return;
  nleaf++;
  unsigned base=0; for(int i=0;i<n;i++)Pm[i]=0;
  for(int p=0;p<m;p++){ if(c0[p]) base|=1u<<p; if(owner[p]>=0) Pm[owner[p]]|=1u<<p; }
  for(int x=0;x<X;x++){unsigned v=base; for(int i=0;i<n;i++) if(x>>i&1) v^=Pm[i]; W[x]=v;}
  unsigned D=0;
  for(int d=1;d<=m/2;d++){ int a=__builtin_popcount(W[0]&rot(W[0],d)); bool ok=true;
    for(int x=1;x<X-1;x++) if(__builtin_popcount(W[x]&rot(W[x],d))!=a){ok=false;break;}
    if(ok){D|=1u<<d; D|=1u<<(m-d);} }
  if(__builtin_popcount(D)<n-1) return;   // need |R|>=n (proofs.txt L3)
  { // need positions p_i in P_i (one per player) with all pairwise differences in D (L2 + pair privacy)
    unsigned D0=D|1u; // difference 0 impossible between distinct players anyway
    // cand = positions q such that q - p in D for all chosen p ; greedy DFS over players
    struct F{ static bool go(int i,unsigned cand,unsigned D0){ if(i==n) return true; unsigned c=cand&Pm[i];
        while(c){int p=__builtin_ctz(c); c&=c-1; if(go(i+1,cand&rot(D0,p),D0)) return true;} return false; } };
    if(!F::go(0,FULL,D0)) return; }
  anyv=false; anyfull=false;
  privacy(1u); dfsR(1u,0,D);
  if(anyv) ncfg++; if(anyfull) ncfgfull++;
}
int part=0,K=1; long long ctr=0;
void rec(int p,int nused){
  if(p==m){leaf();return;}
  if(p==5&&K>1){ if((ctr++)%K!=part) return; }
  int need=0; for(int i=0;i<nused;i++) need+=abs(2*hearts[i]-cnt[i]); need+=2*(n-nused);
  if(need>m-p) return;
  for(int c=0;c<2;c++){ if(p==0) break; owner[p]=-1;c0[p]=c;rec(p+1,nused);}
  for(int i=0;i<=nused&&i<n;i++)for(int a=0;a<2;a++){ if(p==0&&(i!=0||a!=0)) continue;
    owner[p]=i;c0[p]=a;cnt[i]++;hearts[i]+=a; rec(p+1,max(nused,i+1)); cnt[i]--;hearts[i]-=a;}
}
int main(int argc,char**argv){n=atoi(argv[1]);m=atoi(argv[2]);X=1<<n;FULL=(1u<<m)-1; if(argc>4){part=atoi(argv[3]);K=atoi(argv[4]);} rec(0,0);
  printf("FAST4 part=%d/%d n=%d m=%d E=%d F=%d leaves=%lld valid_pairs=%lld valid_cfgs=%lld cfgs_fullR=%lld minR=%d maxR=%d\n",part,K,n,m,nvalid>0,ncfgfull>0,nleaf,nvalid,ncfg,ncfgfull,nvalid?minR:0,maxR);
  printf("  T5_violations_among_valid=%lld\n",t5viol); printf("  WEAK valid_pairs=%lld %s\n",nweak,weakW.c_str()); printf("  by|R|:"); for(int k=1;k<=m;k++) if(byR[k]) printf(" %d:%lld",k,byR[k]); puts("");
  if(nvalid){printf("  FIRST %s\n  MINR  %s\n",firstW.c_str(),minW.c_str()); if(!fullW.empty()) printf("  FULLR %s\n",fullW.c_str());}
}
