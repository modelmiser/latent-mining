// wd.cpp : exhaustive |W_d(n)|, d=0..dmax, and |W_inf(n)|.
// usage: wd n dmax sym(0/1) [dumpfile]
// Delay game solved as reachability game on states (S, queue of pending Synchronizer letters).
// Blind game solved by BFS over antichains (maximal elements) of possible images.
#include <bits/stdc++.h>
using namespace std;
int n, dmax, NS;
int A[2][8];
int img[2][64];
static unsigned char imm[13][1<<12][64][2]; // [len][q][S][x]
static unsigned char win[1<<12][64], win2[1<<12][64];
void buildimg(){ for(int x=0;x<2;x++){ img[x][0]=0; for(int S=1;S<NS;S++){int r=0; for(int i=0;i<n;i++) if(S>>i&1) r|=1<<A[x][i]; img[x][S]=r;} } }
void buildimm(){
  for(int S=1;S<NS;S++) for(int x=0;x<2;x++) imm[0][0][S][x] = __builtin_popcount(img[x][S])==1;
  for(int len=1;len<=dmax;len++) for(int q=0;q<(1<<len);q++){ int q1=(q>>(len-1))&1, rest=q&((1<<(len-1))-1);
    for(int S=1;S<NS;S++){ int S1=img[q1][S]; for(int x=0;x<2;x++) imm[len][q][S][x]= imm[len-1][rest][img[0][S1]][x] && imm[len-1][rest][img[1][S1]][x]; } }
}
// returns min T (number of Synchronizer letters) or 0 if not winnable
int solve(int d){
  int NQ=1<<d, mask=(1<<(d>0?d-1:0))-1;
  static int rk[1<<12][64];
  for(int q=0;q<NQ;q++) for(int S=1;S<NS;S++) rk[q][S]= (imm[d][q][S][0]||imm[d][q][S][1])?1:0;
  for(int it=2;;it++){ bool ch=false; vector<pair<int,int>> nw;
    for(int q=0;q<NQ;q++){ int q1= d? (q>>(d-1))&1:0, rest= d? (q&mask):0;
      for(int S=1;S<NS;S++) if(!rk[q][S]){ bool w=false;
        for(int x=0;x<2&&!w;x++){ int nq= d? ((rest<<1)|x):0; bool all=true;
          for(int y=0;y<2;y++){ int S2 = d? img[y][img[q1][S]] : img[y][img[x][S]]; if(!rk[nq][S2]) {all=false;break;} }
          if(all) w=true; }
        if(w) nw.push_back({q,S}); } }
    if(nw.empty()) break; for(auto&p:nw) rk[p.first][p.second]=it; }
  // prelude: len<d, S=Q
  int Qs=NS-1;
  if(d==0) return rk[0][Qs];
  vector<int> cur(NQ); for(int q=0;q<NQ;q++) cur[q]=rk[q][Qs];
  for(int len=d-1;len>=0;len--){ vector<int> nx(1<<len);
    for(int q=0;q<(1<<len);q++){ int best=0;
      if(imm[len][q][Qs][0]||imm[len][q][Qs][1]) best=1;
      else for(int x=0;x<2;x++){ int r=cur[(q<<1)|x]; if(r && (!best|| r+1<best)) best=r+1; }
      nx[q]=best; }
    cur=nx; }
  return cur[0];
}
typedef unsigned long long u64;
u64 normfam(u64 F){ u64 R=0; for(int S=1;S<NS;S++) if(F>>S&1){ bool dom=false; for(int T=S+1;T<NS&&!dom;T++) if((F>>T&1)&&((S&T)==S)) dom=true; if(!dom) R|=1ULL<<S; } return R; }
int blind(){ // min T or 0
  unordered_set<u64> seen; vector<u64> cur; u64 F0=1ULL<<(NS-1); cur.push_back(F0); seen.insert(F0);
  for(int T=1;!cur.empty();T++){ vector<u64> nx;
    for(u64 F:cur) for(int x=0;x<2;x++){ bool allsing=true; u64 G=0;
      for(int S=1;S<NS;S++) if(F>>S&1){ int S1=img[x][S]; if(__builtin_popcount(S1)!=1) allsing=false; G|=1ULL<<img[0][S1]; G|=1ULL<<img[1][S1]; }
      if(allsing) return T; G=normfam(G); if(seen.insert(G).second) nx.push_back(G); }
    cur=nx; }
  return 0;
}
int main(int argc,char**argv){ n=atoi(argv[1]); dmax=atoi(argv[2]); int sym=atoi(argv[3]); FILE*df= argc>4? fopen(argv[4],"w"):0; NS=1<<n;
  vector<vector<int>> perms; vector<int> p(n); iota(p.begin(),p.end(),0); do perms.push_back(p); while(next_permutation(p.begin(),p.end()));
  long long total=1; for(int i=0;i<2*n;i++) total*=n;
  vector<long long> cnt(dmax+1,0), maxT(dmax+1,0); long long cinf=0, maxTinf=0, classes=0, tot=0; 
  for(long long code=0;code<total;code++){ long long c=code; for(int x=0;x<2;x++) for(int i=0;i<n;i++){A[x][i]=c%n;c/=n;}
    long long w=1;
    if(sym){ int stab=0; bool canon=true;
      for(auto&pp:perms){ for(int s=0;s<2&&canon;s++){ int B[2][8]; for(int x=0;x<2;x++) for(int i=0;i<n;i++) B[x^s][pp[i]]=pp[A[x][i]];
          long long c2=0; for(int x=1;x>=0;x--) for(int i=n-1;i>=0;i--) c2=c2*n+B[x][i];
          if(c2<code) canon=false; else if(c2==code) stab++; } if(!canon)break; }
      if(!canon) continue; w=(long long)perms.size()*2/stab; }
    classes++; tot+=w; buildimg(); buildimm();
    int bl=blind(); if(bl){cinf+=w; maxTinf=max<long long>(maxTinf,bl);}
    int prof=0; for(int d=0;d<=dmax;d++){ int r=solve(d); if(r){cnt[d]+=w; maxT[d]=max<long long>(maxT[d],r); prof|=1<<d;} }
    if(df){ // dump: automata whose profile differs from blind anywhere
      if(prof != (bl? (1<<(dmax+1))-1:0)){ fprintf(df,"a="); for(int i=0;i<n;i++) fprintf(df,"%d",A[0][i]+1); fprintf(df," b="); for(int i=0;i<n;i++) fprintf(df,"%d",A[1][i]+1);
        fprintf(df," orbit=%lld blindT=%d win_d=",w,bl); for(int d=0;d<=dmax;d++) fprintf(df,"%d",(prof>>d)&1); fprintf(df,"\n"); } }
  }
  printf("n=%d sym=%d classes=%lld total_weight=%lld (expect %lld)\n",n,sym,classes,tot,total);
  for(int d=0;d<=dmax;d++) printf("d=%d |W_d|=%lld maxT=%lld\n",d,cnt[d],maxT[d]);
  printf("|W_inf|=%lld maxT=%lld\n",cinf,maxTinf);
}
