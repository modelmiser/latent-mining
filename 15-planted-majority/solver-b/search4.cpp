// Lazy DFS v3 with forward checking (min-domain variable choice) and dead-region pruning.
// Usage: search3 s N mode maxsol [sym=1] [wonly=-1]
// mode 0 = reduced model (one outcome per unordered pair p<=q, no swaps; d[p][q]=id for p>q)
// mode 1 = full ordered d as in the problem text.
#include <bits/stdc++.h>
using namespace std;
int S,NN,MODE,SYM=1; int d[8][8]; int wmask; long long nodes=0, sols=0, maxsol=1, checks=0; int WT[8]; int HAVEWT=0;
unsigned long long undefMask; // bit p*8+q
static int stampArr[4000000]; static int idArr[4000000]; int stamp=0;
int pw[9];
// returns 1 fail, 0 otherwise; ORs undefined pairs seen into undefMask
int checkInst(int a,int b,int x){
  checks++;
  int maj = a>b?0:1; int n=a+b+1; int B=n+1; pw[0]=1; for(int i=1;i<=S;i++) pw[i]=pw[i-1]*B;
  int init=a+b*B+pw[x];
  static vector<int> cf; static vector<vector<int>> adj; static vector<char> flag; // bit0 selfloop bit1 term bit2 hasundef
  cf.clear(); stamp++; stampArr[init]=stamp; idArr[init]=0; cf.push_back(init);
  for(size_t i=0;i<cf.size();i++){
    int c=cf[i]; if(adj.size()<=i) adj.emplace_back(); adj[i].clear(); if(flag.size()<=i) flag.push_back(0); flag[i]=2;
    int cn[8]; { int t=c; for(int p=0;p<S;p++){cn[p]=t%B;t/=B;} }
    for(int p=0;p<S;p++){ if(!cn[p]) continue;
      for(int q=0;q<S;q++){ if(!cn[q]) continue; if(p==q && cn[p]<2) continue;
        int r=d[p][q];
        if(r<0){ flag[i]&=~2; flag[i]|=4; undefMask|=1ULL<<(p*8+q); continue; }
        int p2=r/S,q2=r%S; if(p2==p&&q2==q) continue;
        flag[i]&=~2;
        int c2=c-pw[p]-pw[q]+pw[p2]+pw[q2];
        if(c2==c){flag[i]|=1;continue;}
        int j; if(stampArr[c2]!=stamp){ stampArr[c2]=stamp; j=cf.size(); idArr[c2]=j; cf.push_back(c2);} else j=idArr[c2];
        adj[i].push_back(j);
      }}
  }
  n=cf.size(); static vector<char> good; good.assign(n,0);
  for(int i=0;i<n;i++){ bool g=true; int t=cf[i]; for(int p=0;p<S;p++){ if(t%B && ((wmask>>p)&1)!=maj) g=false; t/=B;} good[i]=g;
    if(!g && (flag[i]&3)) return 1; }
  static vector<int> idx,low,st; static vector<char> on,esc; idx.assign(n,-1); low.assign(n,0); on.assign(n,0); st.clear(); int ctr=0;
  esc.assign(n,0);
  static vector<pair<int,int>> cs; cs.clear();
  // Tarjan; SCCs come out in reverse topological order, so escape-reachability can be computed on the fly
  for(int s0=0;s0<n;s0++){ if(idx[s0]>=0) continue;
    cs.push_back({s0,0}); idx[s0]=low[s0]=ctr++; st.push_back(s0); on[s0]=1;
    while(!cs.empty()){ int v=cs.back().first; int &k=cs.back().second;
      if(k<(int)adj[v].size()){ int u=adj[v][k++];
        if(idx[u]<0){ idx[u]=low[u]=ctr++; st.push_back(u); on[u]=1; cs.push_back({u,0}); }
        else if(on[u]) low[v]=min(low[v],idx[u]);
      } else {
        if(low[v]==idx[v]){ int sz=0; bool bad=false, e=false; size_t top=st.size();
          while(true){int u=st[--top]; sz++; if(!good[u]) bad=true; if(good[u]||(flag[u]&4)) e=true; for(int t:adj[u]) if(!on[t] && esc[t]) e=true; if(u==v)break;}
          if(sz>1&&bad) return 1;
          if(!e) return 1; // dead region: no good config and no undefined pair reachable
          while(st.size()>top){ int u=st.back(); st.pop_back(); on[u]=0; esc[u]=1; }
        }
        int lv=low[v]; cs.pop_back(); if(!cs.empty()){ int pv=cs.back().first; low[pv]=min(low[pv],lv); }
      }
    }
  }
  return 0;
}
vector<array<int,3>> insts;
int checkAll(){ undefMask=0; for(auto&I:insts) if(checkInst(I[0],I[1],I[2])) return 1; return 0; }
void report(){
  sols++;
  if(sols<=maxsol){ printf("SOL s=%d N=%d w=",S,NN); for(int p=0;p<S;p++) putchar(((wmask>>p)&1)?'B':'A'); printf(" d:");
    for(int p=0;p<S;p++)for(int q=0;q<S;q++){ int r=d[p][q]; if(r<0) printf(" %d%d>**",p,q); else printf(" %d%d>%d%d",p,q,r/S,r%S);} printf("\n"); fflush(stdout);}
}
void domain(int p,int q,vector<int>&out){
  out.clear();
  int touched=3|(1<<p)|(1<<q);
  if(SYM){ for(int a=0;a<S;a++)for(int b=0;b<S;b++){ int v=d[a][b]; if(v>=0 && !(MODE==0&&a>b)) touched|=(1<<a)|(1<<b)|(1<<(v/S))|(1<<(v%S)); } }
  else touched=(1<<S)-1;
  int f0=-1,f0b=-1,f1=-1,f1b=-1;
  for(int a=0;a<S;a++) if(!((touched>>a)&1)){ if((wmask>>a)&1){ if(f1<0)f1=a; else if(f1b<0)f1b=a;} else { if(f0<0)f0=a; else if(f0b<0)f0b=a;} }
  for(int p2=0;p2<S;p2++)for(int q2=0;q2<S;q2++){
    if(MODE==0 && p2>q2) continue;
    bool u1=!((touched>>p2)&1), u2=!((touched>>q2)&1);
    if(u1&&u2&&p2!=q2){
      bool same=(((wmask>>p2)&1)==((wmask>>q2)&1));
      if(same){ int f=((wmask>>p2)&1)?f1:f0, fb=((wmask>>p2)&1)?f1b:f0b; if(MODE==0){ if(!(p2==f&&q2==fb)) continue; } else { if(!((p2==f&&q2==fb)||(p2==fb&&q2==f))) continue; } }
      else { if(!((p2==f0||p2==f1)&&(q2==f0||q2==f1))) continue; }
    } else {
      if(u1 && p2!=f0 && p2!=f1) continue;
      if(u2 && q2!=f0 && q2!=f1) continue;
    }
    if(HAVEWT && WT[p2]+WT[q2]!=WT[p]+WT[q]) continue; out.push_back(p2*S+q2);
  }
}
bool dfs(){
  nodes++;
  if(checkAll()) return false;
  unsigned long long um=undefMask;
  if(!um){ report(); return true; }
  int bp=-1,bq=-1; vector<int> best, dom, feas;
  for(int p=0;p<S;p++)for(int q=0;q<S;q++) if((um>>(p*8+q))&1){
    domain(p,q,dom); feas.clear();
    for(int v:dom){ d[p][q]=v; if(!checkAll()) feas.push_back(v); }
    d[p][q]=-1;
    if(feas.empty()) return false;
    if(bp<0 || feas.size()<best.size()){ bp=p;bq=q;best=feas; if(best.size()==1) goto done; }
  }
  done:
  bool any=false;
  for(int v:best){ d[bp][bq]=v; if(dfs()){ any=true; if(sols>=maxsol){ d[bp][bq]=-1; return true; } } }
  d[bp][bq]=-1; return any;
}
int main(int argc,char**argv){
  S=atoi(argv[1]); NN=atoi(argv[2]); MODE=atoi(argv[3]); if(argc>4) maxsol=atoll(argv[4]); if(argc>5) SYM=atoi(argv[5]);
  int wonly= argc>6? atoi(argv[6]) : -1; if(argc>7){ char*t=strtok(argv[7],","); int i=0; while(t){ WT[i++]=atoi(t); t=strtok(NULL,","); } HAVEWT=1; SYM=0; }
  // instances: largest first tends to fail earlier
  for(int n=NN;n>=2;n--) for(int a=0;a<=n;a++){ int b=n-a; if(abs(a-b)<2) continue; for(int x=0;x<S;x++) insts.push_back({a,b,x}); }
  bool found=false;
  for(wmask=0; wmask<(1<<S); wmask++){
    if(wonly>=0 && wmask!=wonly) continue;
    if(SYM){ bool ok=true; for(int p=2;p+1<S;p++) if(((wmask>>p)&1) > ((wmask>>(p+1))&1)) ok=false; if(!ok) continue; }
    for(int p=0;p<S;p++)for(int q=0;q<S;q++) d[p][q]= (MODE==0 && p>q)? p*S+q : -1;
    long long s0=sols,n0=nodes; clock_t t0=clock(); dfs(); if(sols>s0) found=true;
    fprintf(stderr,"w=%d nodes=%lld sols=%lld t=%.1fs\n",wmask,nodes-n0,sols-s0,(clock()-t0)/(double)CLOCKS_PER_SEC);
    if(sols>=maxsol) break;
  }
  printf("RESULT s=%d N=%d mode=%d sym=%d found=%d sols=%lld nodes=%lld checks=%lld\n",S,NN,MODE,SYM,found?1:0,sols,nodes,checks);
}
