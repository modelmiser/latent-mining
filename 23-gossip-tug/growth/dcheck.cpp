// Checker for the Delayer strategy D* (PROOFS.md Theorem 1), written from the game definition.
// Delayer's move is computed by the rule; EVERY legal Hastener reply is explored; the program
// reports the minimum number of calls over all plays (the guarantee of the rule).
// usage: dcheck N [variant]   variant 0 = D* ; 1 = wrong rule (Rule 2 names centre instead of p);
//                             2 = wrong rule (after first expert, name lowest active agent) ; 3 = wrong (star to n-1 leaves, i.e. P1)
#include <bits/stdc++.h>
using namespace std;
int N, VAR=0; typedef unsigned short M;
struct S{ M K[16]; int p; };
static bool legal(const S&s,int a,int b){ return a!=b && ( !((s.K[b]>>a)&1) || !((s.K[a]>>b)&1) ); }
static bool active(const S&s,int a){ for(int b=0;b<N;b++) if(legal(s,a,b)) return true; return false; }
static bool terminal(const S&s){ for(int i=0;i<N;i++) if(s.K[i]!=(M)((1u<<N)-1)) return false; return true; }
// Delayer's rule. s.p = -1 during phase 1.
static int rule(S&s){
  int stop = (VAR==3)? N : N-1;               // phase 1: name agent 0 until it knows N-1 secrets
  if(s.p<0){
    if(__builtin_popcount(s.K[0])<stop) return 0;
    if(VAR==3){ s.p=-2; } else { for(int i=0;i<N;i++) if(!((s.K[0]>>i)&1)) s.p=i; }
  }
  if(s.p==-2){ for(int a=0;a<N;a++) if(active(s,a)) return a; return -1; }
  int p=s.p; M W=((1u<<N)-1)&~(1u<<p), F=(1u<<N)-1;
  bool anyexp=false; for(int i=0;i<N;i++) if(s.K[i]==F) anyexp=true;
  if(!anyexp){
    for(int x=0;x<N;x++) if(x!=p && (s.K[x]&W)!=W && !((s.K[x]>>p)&1)) return x;   // Rule 1: non-top, no flag
    if(VAR==1) return 0;
    return p;                                                                        // Rule 2
  }
  if(VAR==2){ for(int a=0;a<N;a++) if(active(s,a)) return a; }
  if(s.K[p]==F && active(s,p)) return p;                                             // Rule 3
  for(int e=0;e<N;e++) if(s.K[e]==F && active(s,e)) return e;
  fprintf(stderr,"RULE FAILED: no active expert\n"); exit(2);
}
map<vector<int>,int> memo; long nodes=0;
int go(S s){
  if(terminal(s)) return 0;
  int a=rule(s);
  if(a<0||!active(s,a)){ fprintf(stderr,"RULE NAMED INACTIVE AGENT\n"); exit(2); }
  vector<int> k(s.K,s.K+N); k.push_back(s.p);
  auto it=memo.find(k); if(it!=memo.end()) return it->second;
  nodes++;
  int best=INT_MAX;
  bool ph1=(s.p==-1) && !getenv("FULLPH1");   // phase 1: all untouched partners are interchangeable; take the lowest unless FULLPH1 is set
  for(int b=0;b<N;b++) if(legal(s,a,b)){ S t=s; M u=t.K[a]|t.K[b]; t.K[a]=u; t.K[b]=u; best=min(best,1+go(t)); if(ph1) break; }
  return memo[k]=best;
}
int main(int c,char**v){ N=atoi(v[1]); if(c>2) VAR=atoi(v[2]); S s; for(int i=0;i<N;i++) s.K[i]=1u<<i; s.p=-1;
  int g=go(s); printf("N=%d variant=%d guaranteed_calls=%d  3N-6=%d  %s  positions=%ld\n",N,VAR,g,3*N-6,g>=3*N-6?"OK":"BELOW",nodes); }
