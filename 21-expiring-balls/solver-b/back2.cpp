// back2.cpp  usage: back2 n L     (FIFO game; memory-light version of back.cpp, same attractor A)
// A is closed under "prepend an older ball" (Adversary expires it). So A = { p : some age-suffix of p is in C },
// where C (the "core") = minimal seeds + states that entered A by the offer rule. Only C is stored.
// Every state of A is visited exactly once: it is reached from its one-ball-shorter suffix, and a subtree is
// cut where it meets another core element (which enumerates that subtree itself).
// For each visited p in A with newest ball b: if p with b flipped is terminal or in A, then p-b is in A.
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 K; typedef uint64_t u64;
int n,L;
struct St{int k; int r[12],o[12];};
static K packc(const int*r,const int*o,int k){ int mp[16]; memset(mp,0,sizeof mp); int nx=1; K x=0;
  for(int i=0;i<k;i++){ if(!mp[r[i]])mp[r[i]]=nx++; if(!mp[o[i]])mp[o[i]]=nx++;
    x|=(K)(mp[r[i]]|(mp[o[i]]<<4))<<(8*i);} return x;}
static St unpack(K x){St s; s.k=0; while((int)(x&0xff)){s.r[s.k]=(int)(x&15); s.o[s.k]=(int)((x>>4)&15); s.k++; x>>=8;} return s;}
static int maxload(const St&s){int c[16]={0},m=0; for(int i=0;i<s.k;i++){ if(++c[s.r[i]]>m)m=c[s.r[i]]; } return m;}
struct H{ size_t operator()(K x)const{ u64 a=(u64)x^((u64)(x>>64)*0x9e3779b97f4a7c15ULL); a^=a>>33;a*=0xff51afd7ed558ccdULL;a^=a>>33;a*=0xc4ceb9fe1a85ec53ULL;a^=a>>33; return a;}};
unordered_set<K,H> C; vector<K> W; u64 visited=0, seedsC=0; bool found=false;
static void comps(const St&s,int*c){ for(int i=0;i<16;i++)c[i]=i;
  for(int i=0;i<s.k;i++){int a=c[s.r[i]],b=c[s.o[i]]; if(a!=b) for(int j=0;j<16;j++) if(c[j]==b)c[j]=a;} }
static int used(const St&s){int m=0; for(int i=0;i<s.k;i++) m=max(m,max(s.r[i],s.o[i])); return m;}
static bool inA(const St&s){ for(int j=0;j<s.k;j++) if(C.count(packc(s.r+j,s.o+j,s.k-j))) return true; return C.count((K)0)>0; }
// children: prepend an older ball; calls f(child) for each non-terminal child (each child is a distinct state)
template<class F> static void children(const St&a,F f){
  if(a.k>=n-1) return; int ld[16]={0}; for(int i=0;i<a.k;i++) ld[a.r[i]]++; int c[16]; comps(a,c); int m=used(a); int top=min(n,m+2);
  St p; p.k=a.k+1; for(int i=0;i<a.k;i++){p.r[i+1]=a.r[i];p.o[i+1]=a.o[i];}
  for(int x=1;x<=top;x++)for(int y=1;y<=top;y++){ if(x==y)continue;
    if(x<=m&&y<=m){ if(c[x]==c[y])continue; }
    else if(x<=m){ if(y!=m+1)continue; } else if(y<=m){ if(x!=m+1)continue; }
    else { if(!(x==m+1&&y==m+2))continue; }
    if(ld[x]+1>L-1) continue;
    p.r[0]=x;p.o[0]=y; f(p); } }
static void seedgen(const St&s){
  int ld[16]={0}; for(int i=0;i<s.k;i++) ld[s.r[i]]++; int c[16]; comps(s,c); int m=used(s);
  for(int a=1;a<=m;a++) if(ld[a]==L-1) for(int b=a+1;b<=m;b++) if(ld[b]==L-1&&c[a]!=c[b]){ K x=packc(s.r,s.o,s.k); C.insert(x); W.push_back(x); seedsC++; return; }
  // safe prune: a seed needs two bins of load L-1 in different components; prepending never separates components,
  // so the best pair (x,y) must already be in different components now (fresh bins count as load 0, own component).
  int R=n-2-s.k; /* each prepended ball merges two components and x,y must stay apart: at most n-k-2 more balls */ int best=0; // best = max of ld[x]+ld[y] over admissible pairs
  for(int a=1;a<=m;a++){ if(m<n) best=max(best,ld[a]); for(int b=a+1;b<=m;b++) if(c[a]!=c[b]) best=max(best,ld[a]+ld[b]); }
  if(2*(L-1)-best>R) return;
  children(s,[&](const St&p){ seedgen(p); }); }
static void visit(const St&a){ // a in A, non-terminal
  visited++; if(a.k==0){found=true;return;}
  St f=a; swap(f.r[f.k-1],f.o[f.k-1]);
  if(maxload(f)>=L || inA(f)){ St s=a; s.k--; if(!inA(s)){ K x=packc(s.r,s.o,s.k); C.insert(x); W.push_back(x); if(s.k==0) found=true; } }
  children(a,[&](const St&p){ if(!C.count(packc(p.r,p.o,p.k))) visit(p); }); }
int main(int argc,char**argv){ n=atoi(argv[1]);L=atoi(argv[2]);
  St s; s.k=0; seedgen(s); fprintf(stderr,"core seeds=%llu t=%.1fs\n",(unsigned long long)seedsC,clock()/1e6);
  while(!W.empty()&&!found){ K x=W.back(); W.pop_back(); visit(unpack(x)); }
  printf("n=%d L=%d core_seeds=%llu core=%zu attractor_nonterminal_visited=%llu%s RESULT: Adversary %s force load %d\n",n,L,(unsigned long long)seedsC,C.size(),(unsigned long long)visited,found?"(stopped early)":"",found?"CAN":"CANNOT",L);
  return 0; }
