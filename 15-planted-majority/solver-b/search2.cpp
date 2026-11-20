// Lazy DFS v2. Usage: search2 s N mode maxsol [sym=1]
// mode 0 = reduced model (one outcome per unordered pair, no swaps; d[p][q]=id for p>q)
// mode 1 = full ordered d as in the problem text. sym=1: orderly symmetry breaking (w sorted on states>=2; fresh states introduced in order)
#include <bits/stdc++.h>
using namespace std;
int S,NN,MODE,SYM=1; int d[8][8]; int wmask; long long nodes=0, sols=0, maxsol=1;
int np_,nq_;
static int stampArr[2000000]; static int idArr[2000000]; int stamp=0;
int pw[9];
struct G{ vector<int> cf; vector<int> adjStart, adjList; };
int checkInst(int a,int b,int x){
  int maj = a>b?0:1; int n=a+b+1; int B=n+1; pw[0]=1; for(int i=1;i<=S;i++) pw[i]=pw[i-1]*B;
  int init=a+b*B+pw[x];
  static vector<int> cf; static vector<vector<int>> adj; static vector<char> flag; // flag bit0 selfloop bit1 term
  cf.clear(); stamp++; stampArr[init]=stamp; idArr[init]=0; cf.push_back(init);
  bool need=false; size_t na=0;
  for(size_t i=0;i<cf.size();i++){
    int c=cf[i]; if(adj.size()<=i) adj.emplace_back(); adj[i].clear(); if(flag.size()<=i) flag.push_back(0); flag[i]=2;
    int cn[8]; { int t=c; for(int p=0;p<S;p++){cn[p]=t%B;t/=B;} }
    for(int p=0;p<S;p++){ if(!cn[p]) continue;
      for(int q=0;q<S;q++){ if(!cn[q]) continue; if(p==q && cn[p]<2) continue;
        int r=d[p][q];
        if(r<0){ flag[i]&=~2; if(!need){need=true;np_=p;nq_=q;} continue; }
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
  static vector<int> idx,low,st; static vector<char> on; idx.assign(n,-1); low.assign(n,0); on.assign(n,0); st.clear(); int ctr=0;
  static vector<pair<int,int>> cs; cs.clear();
  for(int s0=0;s0<n;s0++){ if(idx[s0]>=0) continue;
    cs.push_back({s0,0}); idx[s0]=low[s0]=ctr++; st.push_back(s0); on[s0]=1;
    while(!cs.empty()){ int v=cs.back().first; int &k=cs.back().second;
      if(k<(int)adj[v].size()){ int u=adj[v][k++];
        if(idx[u]<0){ idx[u]=low[u]=ctr++; st.push_back(u); on[u]=1; cs.push_back({u,0}); }
        else if(on[u]) low[v]=min(low[v],idx[u]);
      } else {
        if(low[v]==idx[v]){ int sz=0; bool bad=false; while(true){int u=st.back(); st.pop_back(); on[u]=0; sz++; if(!good[u]) bad=true; if(u==v)break;} if(sz>1&&bad) return 1; }
        int lv=low[v]; cs.pop_back(); if(!cs.empty()){ int pv=cs.back().first; low[pv]=min(low[pv],lv); }
      }
    }
  }
  return need?2:0;
}
vector<array<int,3>> insts;
void report(){
  sols++;
  if(sols<=maxsol){ printf("SOL s=%d N=%d w=",S,NN); for(int p=0;p<S;p++) putchar(((wmask>>p)&1)?'B':'A'); printf(" d:");
    for(int p=0;p<S;p++)for(int q=0;q<S;q++){ int r=d[p][q]; if(r<0) printf(" %d%d>**",p,q); else printf(" %d%d>%d%d",p,q,r/S,r%S);} printf("\n"); fflush(stdout);}
}
bool dfs(size_t from){
  nodes++;
  for(size_t i=from;i<insts.size();i++){
    int r=checkInst(insts[i][0],insts[i][1],insts[i][2]);
    if(r==1) return false;
    if(r==2){ int p=np_,q=nq_; bool any=false;
      int touched=3|(1<<p)|(1<<q);
      if(SYM){ for(int a=0;a<S;a++)for(int b=0;b<S;b++){ int v=d[a][b]; if(v>=0 && !(MODE==0&&a>b)) touched|=(1<<a)|(1<<b)|(1<<(v/S))|(1<<(v%S)); } }
      else touched=(1<<S)-1;
      // first untouched state of each output class
      int f0=-1,f0b=-1,f1=-1,f1b=-1;
      for(int a=0;a<S;a++) if(!((touched>>a)&1)){ if((wmask>>a)&1){ if(f1<0)f1=a; else if(f1b<0)f1b=a;} else { if(f0<0)f0=a; else if(f0b<0)f0b=a;} }
      for(int p2=0;p2<S;p2++)for(int q2=0;q2<S;q2++){
        if(MODE==0 && p2>q2) continue;
        bool u1=!((touched>>p2)&1), u2=!((touched>>q2)&1);
        if(u1 && p2!=f0 && p2!=f1 && !(u2 && (p2==f0b||p2==f1b))) continue;
        if(u2 && q2!=f0 && q2!=f1 && !(u1 && (q2==f0b||q2==f1b))) continue;
        if(u1&&u2&&p2!=q2){ // two distinct fresh states: allow (f,fb) same class in order, or (f0,f1)/(f1,f0) across classes
          bool same=(((wmask>>p2)&1)==((wmask>>q2)&1));
          if(same){ int f=((wmask>>p2)&1)?f1:f0, fb=((wmask>>p2)&1)?f1b:f0b; if(MODE==0){ if(!(p2==f&&q2==fb)) continue; } else { if(!((p2==f&&q2==fb)||(p2==fb&&q2==f))) continue; } }
          else { if(!((p2==f0||p2==f1)&&(q2==f0||q2==f1))) continue; }
        }
        d[p][q]=p2*S+q2; if(dfs(i)){any=true; if(sols>=maxsol){d[p][q]=-1;return true;}}
      }
      d[p][q]=-1; return any; }
  }
  report(); return true;
}
int main(int argc,char**argv){
  S=atoi(argv[1]); NN=atoi(argv[2]); MODE=atoi(argv[3]); if(argc>4) maxsol=atoll(argv[4]); if(argc>5) SYM=atoi(argv[5]);
  for(int n=2;n<=NN;n++) for(int a=0;a<=n;a++){ int b=n-a; if(abs(a-b)<2) continue; for(int x=0;x<S;x++) insts.push_back({a,b,x}); }
  bool found=false;
  for(wmask=0; wmask<(1<<S); wmask++){
    if(SYM){ bool ok=true; for(int p=2;p+1<S;p++) if(((wmask>>p)&1) > ((wmask>>(p+1))&1)) ok=false; if(!ok) continue; }
    for(int p=0;p<S;p++)for(int q=0;q<S;q++) d[p][q]= (MODE==0 && p>q)? p*S+q : -1;
    long long s0=sols,n0=nodes; clock_t t0=clock(); dfs(0); if(sols>s0) found=true;
    fprintf(stderr,"w=%d nodes=%lld sols=%lld t=%.1fs\n",wmask,nodes-n0,sols-s0,(clock()-t0)/(double)CLOCKS_PER_SEC);
    if(sols>=maxsol) break;
  }
  printf("RESULT s=%d N=%d mode=%d sym=%d found=%d sols=%lld nodes=%lld\n",S,NN,MODE,SYM,found?1:0,sols,nodes);
}
