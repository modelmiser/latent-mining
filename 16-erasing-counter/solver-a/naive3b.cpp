// Independent exhaustive check of D(3): enumerate every depth-<=3 tree on 7 indices (root index 1 WLOG;
// 0-child index 2 WLOG if internal; 1-child index in {2,3} WLOG if internal), build f on all 128
// states by direct simulation, and find the longest cycle of the functional graph.
#include <cstdio>
#include <vector>
#include <cstring>
struct Node{int kind; int idx; int ch[2];}; // kind 1 leaf (idx=R, 0 empty), 2 internal
// simulate subtree described by option; state s is 7-bit mask, bit (i-1) = position i
// A depth-1 child option: leaf R in {0,1}; or internal c with two grandchild options
struct G{int leaf; int R; int e; int R0,R1;};
std::vector<G> gopts(int c){ std::vector<G> v;
  int Rs[3]={0,1,c}; for(int k=0;k<3;k++) v.push_back({1,Rs[k],0,0,0});
  for(int e=2;e<=7;e++) if(e!=c){ int Q[4]={0,1,c,e}; for(int a=0;a<4;a++)for(int b=0;b<4;b++) v.push_back({0,0,e,Q[a],Q[b]}); }
  return v; }
static inline int apply(int s,int P,int R){ s&=~P; if(R) s|=1<<(R-1); return s; }
int evalG(const G&g,int s,int P){ if(g.leaf) return apply(s,P,g.R);
  int bit=(s>>(g.e-1))&1; P|=1<<(g.e-1); return apply(s,P,bit?g.R1:g.R0); }
// build all options for the b-child of root: table of f over the 64 states with root bit=b
struct Opt{ unsigned char f[64]; };
void build(int b,const std::vector<int>&cs,std::vector<Opt>&out){
  int Rs[2]={0,1};
  for(int k=0;k<2;k++){ Opt o; for(int t=0;t<64;t++){int s=(t<<1)|b; o.f[t]=apply(s,1,Rs[k]);} out.push_back(o);}
  for(int c:cs){ auto gs=gopts(c);
    for(auto&g0:gs)for(auto&g1:gs){ Opt o; for(int t=0;t<64;t++){int s=(t<<1)|b; int bit=(s>>(c-1))&1; o.f[t]=evalG(bit?g1:g0,s,1|(1<<(c-1)));} out.push_back(o);} }
}
int main(){
  std::vector<Opt> A,B; build(0,{2},A); build(1,{2,3},B);
  printf("options %zu x %zu\n",A.size(),B.size());
  int best=0; long long hist[130]; memset(hist,0,sizeof hist); int maxbeta=0,maxrun=0,maxA=0; int maxbetaN=0,maxrunN=0;
  unsigned char f[128]; int mark[128]; memset(mark,0,sizeof mark); int stamp=0; int pos[128];
  for(size_t a=0;a<A.size();a++){ for(int t=0;t<64;t++) f[t<<1]=A[a].f[t];
    for(size_t b=0;b<B.size();b++){ for(int t=0;t<64;t++) f[(t<<1)|1]=B[b].f[t];
      int base=stamp; int loc=0;
      for(int s=0;s<128;s++){ if(mark[s]>base) continue; int st0=++stamp; int x=s; int n=0;
        while(mark[x]<=base){ mark[x]=st0; pos[x]=n++; x=f[x]; }
        if(mark[x]==st0){ int L=n-pos[x]; if(L>loc) loc=L; int beta=0; int y=x; for(int q=0;q<L;q++){beta+=y&1; y=f[y];}
          if(beta>0 && beta<L){ while(!(y&1)) y=f[y]; int run=0,mr=0; for(int q=0;q<L;q++){ y=f[y]; if(y&1){ if(run>mr) mr=run; run=0;} else run++; }
            if(beta>maxbeta){maxbeta=beta;maxbetaN=L;} if(mr>maxrun){maxrun=mr;maxrunN=L;} if(L-beta>maxA) maxA=L-beta; } } }
      if(stamp>2000000000){ memset(mark,0,sizeof mark); stamp=0; }
      hist[loc]++; if(loc>best) best=loc; } }
  printf("D(3)=%d\n",best); printf("mixed cycles: max #root=1 states %d (in a cycle of length %d), max run of root=0 states %d (cycle length %d), max #root=0 states %d\n",maxbeta,maxbetaN,maxrun,maxrunN,maxA); printf("trees by longest cycle:"); for(int i=1;i<=128;i++) if(hist[i]) printf(" %d:%lld",i,hist[i]); printf("\n");
}
