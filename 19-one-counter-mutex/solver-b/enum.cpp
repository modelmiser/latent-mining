// enum <n> <b> <s> [part nparts] : exhaustive search for solutions with n in {2,3},
// every process having at most s states.  Prints counts and up to 5 solutions.
// Automata are enumerated up to renaming of the non-rem/non-crit states (canonical = BFS order
// from rem, all states reachable); "read with constant successor" is identified with "add 0".
// Pruning (all are necessary conditions, so safe):
//   P1: solo run of each process from R=0 must reach crit, with R = e != 0 there.
//   P2: for a pair (p,q): e_p must not be a value from which q, solo from rem, reaches crit.
//   P3 (n=3): every pair of a solution is itself a 2-process solution (third process idle in rem).
#include "mc.h"
#include <set>
#include <string>
int b,S;
std::vector<Aut> L; std::vector<int> Emask, Eval;
static int solo(const Aut&a,int v,int&e){ // returns 1 if reaches crit from rem with R=v
  int s=0,r=v; for(int t=0;t<MAXS*MAXB+2;t++){ int q=a.succ[s][r]; r=(r+a.add[s])%b; s=q; if(s==1){ e=r; return 1; } } return 0; }
static bool canonical(const Aut&a){
  int k=a.ns; int seen[MAXS]={0}; int q[MAXS],h=0,t=0; q[t++]=0; seen[0]=1; int next=2;
  while(h<t){ int s=q[h++]; for(int v=0;v<b;v++){ int y=a.succ[s][v]; if(!seen[y]){ if(y>=2){ if(y!=next) return false; next++; } seen[y]=1; q[t++]=y; } } }
  return t==k;
}
static void gen(int k){
  Aut a; a.ns=k; memset(a.add,0,sizeof a.add); memset(a.succ,0,sizeof a.succ);
  // options for a non-crit state: index over reads ((k-1)^b) then adds c!=0 ((b-1)(k-1))
  long nr=1; for(int v=0;v<b;v++) nr*= (k-1); long nopt=nr+(long)(b-1)*(k-1);
  std::vector<long> ch(k,0);
  auto setst=[&](int s,long o){ if(o<nr){ a.add[s]=0; for(int v=0;v<b;v++){ a.succ[s][v]=1+o%(k-1); o/=(k-1);} } else { o-=nr; a.add[s]=1+o%(b-1); int q=1+o/(b-1); for(int v=0;v<b;v++) a.succ[s][v]=q; } };
  std::vector<int> ncs; for(int s=0;s<k;s++) if(s!=1) ncs.push_back(s);
  long total=1; for(size_t i=0;i<ncs.size();i++) total*=nopt;
  for(long code=0;code<total;code++){
    long c=code; for(size_t i=0;i<ncs.size();i++){ setst(ncs[i],c%nopt); c/=nopt; }
    if(!canonical(a)) continue;
    for(int cc=0;cc<b;cc++){ a.add[1]=cc; for(int v=0;v<b;v++) a.succ[1][v]=0;
      if(legal(a,b)){ fprintf(stderr,"internal: illegal\n"); exit(1);} 
      int e; if(!solo(a,0,e)||e==0) continue;   // P1
      int m=0; for(int v=0;v<b;v++){ int e2; if(solo(a,v,e2)) m|=1<<v; }
      L.push_back(a); Emask.push_back(m); Eval.push_back(e); }
  }
}
int main(int argc,char**argv){
  int n=atoi(argv[1]); b=atoi(argv[2]); S=atoi(argv[3]); int part=0,np=1; if(argc>5){ part=atoi(argv[4]); np=atoi(argv[5]); }
  for(int k=2;k<=S;k++) gen(k);
  int Ln=L.size(); printf("n=%d b=%d s<=%d: %d canonical automata pass P1\n",n,b,S,Ln);
  long checks=0,sol=0,shown=0;
  MC m; m.b=b;
  if(n==2){ m.n=2;
    int small=getenv("SMALL")?atoi(getenv("SMALL")):99; for(int i=0;i<Ln;i++){ if(i%np!=part) continue; if(L[i].ns>small) continue; for(int j=i;j<Ln;j++){
      if((Emask[j]>>Eval[i])&1) continue; if((Emask[i]>>Eval[j])&1) continue;
      m.A[0]=&L[i]; m.A[1]=&L[j]; checks++;
      if(m.check()==0){ sol++; if(shown<5){ shown++; printf("SOLUTION\n2 %d\n",b); printaut(stdout,L[i],b); printaut(stdout,L[j],b); } } } }
  } else {
    std::vector<std::vector<int>> adj(Ln); std::vector<std::vector<char>> ok(Ln,std::vector<char>(Ln,0)); long pairs=0;
    m.n=2;
    for(int i=0;i<Ln;i++) for(int j=i;j<Ln;j++){
      if((Emask[j]>>Eval[i])&1) continue; if((Emask[i]>>Eval[j])&1) continue;
      m.A[0]=&L[i]; m.A[1]=&L[j]; if(m.check()==0){ ok[i][j]=ok[j][i]=1; adj[i].push_back(j); pairs++; } }
    printf("valid unordered pairs (2-process solutions): %ld\n",pairs);
    m.n=3;
    for(int i=0;i<Ln;i++) for(int j:adj[i]) for(int k:adj[j]) if(k>=j && ok[i][k]){
      m.A[0]=&L[i]; m.A[1]=&L[j]; m.A[2]=&L[k]; checks++;
      if(m.check()==0){ sol++; if(shown<5){ shown++; printf("SOLUTION\n3 %d\n",b); printaut(stdout,L[i],b); printaut(stdout,L[j],b); printaut(stdout,L[k],b);} } }
  }
  printf("full model checks: %ld   solutions (unordered, canonical): %ld\n",checks,sol);
}
