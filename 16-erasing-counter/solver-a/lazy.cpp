// Lazy exhaustive search for D(r): longest cycle of a destructive single-write incrementer.
// usage: lazy r m [target]   (m = number of available indices, default 2^r-1)
// Tree nodes in heap numbering (root=1, b-child of v = 2v+b). Indices are introduced in
// order of first use (canonical naming: safe, since f is equivariant under index renaming).
// The start state is decided lazily too: the initial value of an index is chosen when first read.
// State is stored as diff D = state XOR init on touched indices; cycle closes iff D==0.
// Pruning: a trajectory that revisits a non-start state is abandoned (start not on a cycle).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
typedef unsigned long long u64;
int r, m; 
int kind[1<<12]; // 0 undecided, 1 leaf, 2 internal
int idx[1<<12];  // index (1..m) for internal; R (0=empty) for leaf
int nused=0; unsigned I=0, touched=0, D=0;
std::vector<char> vis;
int bk[1<<12],bi[1<<12]; unsigned bI; int best=0; u64 nodes=0; u64 hist[1<<16];
int target=0;
void dump(int N){
  char fn[64]; sprintf(fn,"witness_r%d_m%d_N%d.txt",r,m,N);
  FILE*f=fopen(fn,"w");
  fprintf(f,"r %d\nN %d\nstart",r,N);
  for(int i=1;i<=(1<<r)-1;i++) fprintf(f," %d",(int)((I>>i)&1));
  fprintf(f,"\n");
  // print decided nodes; undecided nodes reachable from decided internals become leaf 0
  std::vector<int> st; st.push_back(1);
  while(!st.empty()){int v=st.back(); st.pop_back();
    char p[32]; int L=0; for(int b=31-__builtin_clz(v)-1;b>=0;b--) p[L++]='0'+((v>>b)&1); p[L]=0;
    if(kind[v]==2){fprintf(f,"%s I %d\n",L?p:"-",idx[v]); st.push_back(2*v+1); st.push_back(2*v);}
    else fprintf(f,"%s L %d\n",L?p:"-",kind[v]==1?idx[v]:0);
  }
  fclose(f);
}
void walk(int v,int depth,unsigned P,int t);
void leafapply(int v,unsigned P,int t){
  unsigned oldD=D;
  int R=idx[v];
  unsigned newval = R? (1u<<R):0;
  D = (D & ~P) | ((newval ^ I) & P);
  if(D==0){ hist[t+1]++; if(t+1>best){best=t+1; fprintf(stderr,"best %d nodes %llu\n",best,nodes); memcpy(bk,kind,sizeof kind); memcpy(bi,idx,sizeof idx); bI=I;} if(target && t+1>=target){printf("target reached\n"); exit(0);} }
  else if(!vis[D]){ vis[D]=1; walk(1,0,0,t+1); vis[D]=0; }
  D=oldD;
}
void walk(int v,int depth,unsigned P,int t){
  nodes++;
  if(kind[v]==0){
    kind[v]=1;
    idx[v]=0; leafapply(v,P,t);
    for(int i=1;i<=nused;i++) if(P>>i&1){ idx[v]=i; leafapply(v,P,t);}
    if(depth<r){
      kind[v]=2;
      for(int i=1;i<=nused;i++) if(!(P>>i&1)){ idx[v]=i; walk(v,depth,P,t);}
      if(nused<m){ nused++; idx[v]=nused; walk(v,depth,P,t); nused--; }
    }
    kind[v]=0; return;
  }
  if(kind[v]==1){ leafapply(v,P,t); return; }
  int i=idx[v];
  if(!(touched>>i&1)){
    touched|=1u<<i;
    walk(2*v,depth+1,P|1u<<i,t);           // init 0
    I|=1u<<i; walk(2*v+1,depth+1,P|1u<<i,t); I&=~(1u<<i);
    touched&=~(1u<<i);
  } else {
    int val=((I^D)>>i)&1;
    walk(2*v+val,depth+1,P|1u<<i,t);
  }
}
int main(int argc,char**argv){
  r=atoi(argv[1]); m=argc>2?atoi(argv[2]):(1<<r)-1; if(argc>3) target=atoi(argv[3]);
  vis.assign(1u<<(m+1),0);
  walk(1,0,0,0);
  memcpy(kind,bk,sizeof kind); memcpy(idx,bi,sizeof idx); I=bI; dump(best);
  printf("r=%d m=%d D=%d nodes=%llu\n",r,m,best,nodes);
  printf("cycle-length histogram (count of (tree-part,start) closings):");
  for(int i=1;i<=best;i++) if(hist[i]) printf(" %d:%llu",i,hist[i]);
  printf("\n");
}
