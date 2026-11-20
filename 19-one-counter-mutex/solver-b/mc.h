// Shared model checker. Unified state representation:
//   every state has add[s] in Z_b and succ[s][v] for v in Z_b.
//   read  : add = 0, succ arbitrary
//   add c : succ[s][v] the same for all v
// state 0 = rem, state 1 = crit.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#define MAXS 12
#ifndef MAXB
#define MAXB 8
#endif
#define MAXN 5
struct Aut { int ns; int add[MAXS]; int succ[MAXS][MAXB]; };

// structural legality; returns 0 if fine
static int legal(const Aut&a,int b){
  if(a.ns<2) return 1;
  for(int s=0;s<a.ns;s++){
    if(a.add[s]<0||a.add[s]>=b) return 2;
    for(int v=0;v<b;v++){
      int q=a.succ[s][v];
      if(q<0||q>=a.ns) return 3;
      if(a.add[s]!=0 && q!=a.succ[s][0]) return 4; // add must be blind
      if(s==0 && q==0) return 5;      // successors of rem differ from rem
      if(s==1 && q!=0) return 6;      // successors of crit are rem
      if(s!=1 && q==0) return 7;      // nobody else goes to rem
    }
  }
  return 0;
}

struct MC {
  int n,b; const Aut* A[MAXN]; long mul[MAXN+1]; long N;
  std::vector<char> reach; std::vector<int> order;
  long enc(const int*st,int r){ long x=0; for(int i=0;i<n;i++) x=x*MAXS+st[i]; return x*b+r; }
  void dec(long x,int*st,int&r){ r=x%b; x/=b; for(int i=n-1;i>=0;i--){ st[i]=x%MAXS; x/=MAXS; } }
  long step(long x,int p){ int st[MAXN],r; dec(x,st,r); const Aut&a=*A[p]; int s=st[p]; st[p]=a.succ[s][r]; r=(r+a.add[s])%b; return enc(st,r); }
  int ncrit(long x){ int st[MAXN],r; dec(x,st,r); int c=0; for(int i=0;i<n;i++) c+=(st[i]==1); return c; }
  // 0 ok, 1 safety violated, 2 progress violated. witness config in wit, subset in witM
  long wit; int witM;
  int check(){
    N=1; for(int i=0;i<n;i++) N*=MAXS; N*=b;
    reach.assign(N,0); order.clear();
    int st[MAXN]={0}; long x0=enc(st,0); reach[x0]=1; order.push_back(x0);
    for(size_t h=0;h<order.size();h++){
      long x=order[h];
      if(ncrit(x)>=2){ wit=x; return 1; }
      for(int p=0;p<n;p++){ long y=step(x,p); if(!reach[y]){ reach[y]=1; order.push_back(y);} }
    }
    // progress: for each nonempty M, graph of reachable no-crit configs with non-M in rem,
    // edges = moves of M to no-crit configs. Violation iff some SCC has internal edges of all of M.
    std::vector<int> idx(N,-1),low(N),comp(N,-1);
    for(int M=1;M<(1<<n);M++){
      std::vector<long> nodes;
      for(long x:order){ if(ncrit(x)) continue; int s2[MAXN],r; dec(x,s2,r); bool ok=true; for(int i=0;i<n;i++) if(!((M>>i)&1) && s2[i]!=0) ok=false; if(ok) nodes.push_back(x); }
      for(long x:nodes){ idx[x]=-1; comp[x]=-1; }
      // iterative Tarjan
      int counter=0,ncomp=0; std::vector<long> stk; std::vector<std::pair<long,int>> cs;
      std::vector<char> inM(N>0?0:0);
      for(long root:nodes){ if(idx[root]!=-1) continue;
        cs.push_back({root,0}); idx[root]=low[root]=counter++; stk.push_back(root);
        while(!cs.empty()){
          long x=cs.back().first; int &pi=cs.back().second;
          bool pushed=false;
          while(pi<n){ int p=pi++; if(!((M>>p)&1)) continue; long y=step(x,p); if(ncrit(y)) continue;
            if(idx[y]==-1){ idx[y]=low[y]=counter++; stk.push_back(y); cs.push_back({y,0}); pushed=true; break; }
            else if(comp[y]==-1) low[x]=std::min(low[x],idx[y]); }
          if(pushed) continue;
          if(low[x]==idx[x]){ while(true){ long y=stk.back(); stk.pop_back(); comp[y]=ncomp; if(y==x) break; } ncomp++; }
          cs.pop_back(); if(!cs.empty()){ long par=cs.back().first; low[par]=std::min(low[par],low[x]); }
        }
      }
      std::vector<int> movers(ncomp,0);
      for(long x:nodes) for(int p=0;p<n;p++) if((M>>p)&1){ long y=step(x,p); if(ncrit(y)) continue; if(comp[y]==comp[x]) movers[comp[x]]|=1<<p; }
      for(long x:nodes) if(movers[comp[x]]==M){ wit=x; witM=M; return 2; }
    }
    return 0;
  }
};

// read a system file: "n b" then per process "ns" and ns lines "R q0..q_{b-1}" or "A c q"
static bool readsys(const char*fn,int&n,int&b,Aut*a){
  FILE*f=fopen(fn,"r"); if(!f) return false;
  if(fscanf(f,"%d %d",&n,&b)!=2) return false;
  for(int i=0;i<n;i++){
    if(fscanf(f,"%d",&a[i].ns)!=1) return false;
    for(int s=0;s<a[i].ns;s++){ char t[8]; if(fscanf(f,"%7s",t)!=1) return false;
      if(t[0]=='R'){ a[i].add[s]=0; for(int v=0;v<b;v++) if(fscanf(f,"%d",&a[i].succ[s][v])!=1) return false; }
      else { int c,q; if(fscanf(f,"%d %d",&c,&q)!=2) return false; a[i].add[s]=((c%b)+b)%b; for(int v=0;v<b;v++) a[i].succ[s][v]=q; }
    }
  }
  fclose(f); return true;
}
static void printaut(FILE*f,const Aut&a,int b){
  fprintf(f,"%d\n",a.ns);
  for(int s=0;s<a.ns;s++){
    bool cst=true; for(int v=1;v<b;v++) if(a.succ[s][v]!=a.succ[s][0]) cst=false;
    if(a.add[s]!=0 || cst) fprintf(f,"A %d %d\n",a.add[s],a.succ[s][0]);
    else { fprintf(f,"R"); for(int v=0;v<b;v++) fprintf(f," %d",a.succ[s][v]); fprintf(f,"\n"); }
  }
}
