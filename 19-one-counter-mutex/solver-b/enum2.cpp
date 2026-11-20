// enum2 pairs <b> <s> <part> <nparts> <outfile> : all 2-process solutions (unordered pairs i<=j of
//        canonical automata with <= s states), written as "i j" lines; prints counts.
// enum2 triples <b> <s> <pairfile>... : 3-process solutions = triangles of the pair graph that pass
//        the full 3-process model check (P3: every pair of a solution is a 2-process solution).
// enum2 dump <b> <s> <i> : print automaton number i.
// Same enumeration/canonical form as enum.cpp.  Extra safe prefilter P4 for pairs:
//   let V_p = values of R at all-in-rem while p alone cycles from R=0, C_p = values of R while p is
//   in crit during those cycles.  A solution needs V_p subset of E_q and C_p disjoint from E_q
//   (q solo from rem must enter iff nobody is in crit).  P1, P2 as in enum.cpp (P2 is implied by P4).
#include "mc.h"
int b,S;
std::vector<Aut> L; std::vector<int> Emask, Vmask, Cmask;
static int solo(const Aut&a,int v,int&e){ int s=0,r=v; for(int t=0;t<MAXS*MAXB+2;t++){ int q=a.succ[s][r]; r=(r+a.add[s])%b; s=q; if(s==1){ e=r; return 1; } } return 0; }
static bool canonical(const Aut&a){
  int k=a.ns; int seen[MAXS]={0}; int q[MAXS],h=0,t=0; q[t++]=0; seen[0]=1; int next=2;
  while(h<t){ int s=q[h++]; for(int v=0;v<b;v++){ int y=a.succ[s][v]; if(!seen[y]){ if(y>=2){ if(y!=next) return false; next++; } seen[y]=1; q[t++]=y; } } }
  return t==k;
}
static void gen(int k){
  Aut a; a.ns=k; memset(a.add,0,sizeof a.add); memset(a.succ,0,sizeof a.succ);
  long nr=1; for(int v=0;v<b;v++) nr*= (k-1); long nopt=nr+(long)(b-1)*(k-1);
  auto setst=[&](int s,long o){ if(o<nr){ a.add[s]=0; for(int v=0;v<b;v++){ a.succ[s][v]=1+o%(k-1); o/=(k-1);} } else { o-=nr; a.add[s]=1+o%(b-1); int q=1+o/(b-1); for(int v=0;v<b;v++) a.succ[s][v]=q; } };
  std::vector<int> ncs; for(int s=0;s<k;s++) if(s!=1) ncs.push_back(s);
  long total=1; for(size_t i=0;i<ncs.size();i++) total*=nopt;
  for(long code=0;code<total;code++){
    long c=code; for(size_t i=0;i<ncs.size();i++){ setst(ncs[i],c%nopt); c/=nopt; }
    if(!canonical(a)) continue;
    for(int cc=0;cc<b;cc++){ a.add[1]=cc; for(int v=0;v<b;v++) a.succ[1][v]=0;
      int e; if(!solo(a,0,e)||e==0) continue;
      int m=0; for(int v=0;v<b;v++){ int e2; if(solo(a,v,e2)) m|=1<<v; }
      // solo cycling from 0
      int V=1,C=0,v=0; bool okc=true;
      for(int t=0;t<=b;t++){ int e2; if(!solo(a,v,e2)){ okc=false; break; } C|=1<<e2; v=(e2+a.add[1])%b; V|=1<<v; }
      if(!okc) continue;           // p alone must be able to cycle forever (progress)
      if(V&C) continue;            // a second process could not tell "free" from "p critical"
      L.push_back(a); Emask.push_back(m); Vmask.push_back(V); Cmask.push_back(C); }
  }
}
int main(int argc,char**argv){
  const char*mode=argv[1]; b=atoi(argv[2]); S=atoi(argv[3]);
  for(int k=2;k<=S;k++) gen(k);
  int Ln=L.size(); printf("b=%d s<=%d: %d canonical automata pass the solo filters\n",b,S,Ln); fflush(stdout);
  MC m; m.b=b;
  if(!strcmp(mode,"dump")){ printaut(stdout,L[atoi(argv[4])],b); return 0; }
  if(!strcmp(mode,"pairs")){ int part=atoi(argv[4]),np=atoi(argv[5]); FILE*o=fopen(argv[6],"w"); m.n=2; long checks=0,sol=0;
    int small=getenv("SMALL")?atoi(getenv("SMALL")):99; for(int i=0;i<Ln;i++){ if(i%np!=part) continue; if(L[i].ns>small) continue; for(int j=i;j<Ln;j++){
      if((Vmask[i]&~Emask[j])||(Cmask[i]&Emask[j])||(Vmask[j]&~Emask[i])||(Cmask[j]&Emask[i])) continue;
      m.A[0]=&L[i]; m.A[1]=&L[j]; checks++; if(m.check()==0){ sol++; fprintf(o,"%d %d\n",i,j); } } }
    fclose(o); printf("part %d/%d: full model checks %ld, 2-process solutions %ld\n",part,np,checks,sol); return 0; }
  std::vector<std::vector<int>> adj(Ln); long pairs=0;
  for(int f=4;f<argc;f++){ FILE*o=fopen(argv[f],"r"); int i,j; while(fscanf(o,"%d %d",&i,&j)==2){ adj[i].push_back(j); if(i!=j) adj[j].push_back(i); pairs++; } fclose(o); }
  for(auto&v:adj) std::sort(v.begin(),v.end());
  auto has=[&](int i,int j){ return std::binary_search(adj[i].begin(),adj[i].end(),j); };
  printf("2-process solutions read: %ld\n",pairs);
  m.n=3; long checks=0,sol=0,shown=0;
  for(int i=0;i<Ln;i++) for(int j:adj[i]) if(j>=i) for(int k:adj[j]) if(k>=j && has(i,k)){
    m.A[0]=&L[i]; m.A[1]=&L[j]; m.A[2]=&L[k]; checks++;
    if(m.check()==0){ sol++; if(shown<5){ shown++; printf("SOLUTION %d %d %d\n3 %d\n",i,j,k,b); printaut(stdout,L[i],b); printaut(stdout,L[j],b); printaut(stdout,L[k],b);} } }
  printf("pairwise-compatible triples model-checked: %ld   3-process solutions: %ld\n",checks,sol);
}
