// Referee exhaustive lazy search. usage: srch S N mode wmaskLo wmaskHi [stopAtFirst=1]
// mode 0: one outcome multiset per unordered pair, no swaps (needs the reduction lemma)
// mode 1: literal ordered model at multiset level: each ordered pair (p,q) has ID, SWAP (p!=q) or another outcome multiset.
// qA=0,qB=1 (WLOG by renaming). Output map: bit i of mask set => state i outputs B.
#include <bits/stdc++.h>
using namespace std;
int S,N,MODE,STOP; int wm; int LASTN=0;
int nslot,sp[40],sq[40],slotOf[6][6],rule[40]; // rule: -1 undef, -2 ID, -3 SWAP, else r*8+t (r<=t)
vector<int> opts[40];
struct In{int a,b,x;}; vector<In> ins;
unsigned long long nodes=0; double sols=0; bool found=false;
int idOf[1<<18],stampA[1<<18],stamp=0;
int eval(const In&I,int&need){
  int badmask = (I.a>I.b)? wm : (~wm)&((1<<S)-1); // states whose output != majority
  stamp++; int keys[512],n=0; vector<int> adj[512]; bool selfl[512],open_[512];
  int c[6]={0};c[0]+=I.a;c[1]+=I.b;c[I.x]++;
  auto pk=[&](int*cc){int k=0;for(int i=S-1;i>=0;i--)k=(k<<3)|cc[i];return k;};
  auto get=[&](int k){if(stampA[k]==stamp)return idOf[k];stampA[k]=stamp;idOf[k]=n;keys[n]=k;return n++;};
  get(pk(c)); need=-1;
  for(int v=0;v<n;v++){int k=keys[v];for(int i=0;i<S;i++){c[i]=k&7;k>>=3;}
    selfl[v]=false;open_[v]=false;adj[v].clear();
    for(int p=0;p<S;p++)if(c[p])for(int q=0;q<S;q++)if(c[q]&&(p!=q||c[p]>=2)){
      int sl=slotOf[p][q]; if(sl<0)continue; int r=rule[sl];
      if(r==-1){open_[v]=true;if(need<0||LASTN)need=sl;}
      else if(r==-2){}
      else if(r==-3){selfl[v]=true;}
      else{int a=r>>3,b=r&7;c[p]--;c[q]--;c[a]++;c[b]++;int u=get(pk(c));c[a]--;c[b]--;c[p]++;c[q]++;adj[v].push_back(u);}
    }}
  bool bad[512];bool anybad=false;
  for(int v=0;v<n;v++){int k=keys[v];bad[v]=false;for(int i=0;i<S;i++){if((k&7)&&((badmask>>i)&1))bad[v]=true;k>>=3;}
    if(bad[v]){anybad=true;if(selfl[v])return 1;if(!open_[v]&&adj[v].empty())return 1;}}
  if(anybad){ // bad node on a cycle of defined edges?  reachability from each bad node back to itself
    for(int v=0;v<n;v++)if(bad[v]){
      static int seen[512];static int st[4096];for(int i=0;i<n;i++)seen[i]=0;int top=0;
      for(int u:adj[v])if(!seen[u]){seen[u]=1;st[top++]=u;}
      while(top){int u=st[--top];if(u==v)return 1;for(int x:adj[u])if(!seen[x]){seen[x]=1;st[top++]=x;}}
    }}
  return need>=0?2:0;
}
void dfs(int i0){
  nodes++; if(found&&STOP)return;
  for(int i=i0;i<(int)ins.size();i++){int need;int r=eval(ins[i],need);
    if(r==1)return;
    if(r==2){for(int o:opts[need]){rule[need]=o;dfs(i);if(found&&STOP){return;}}rule[need]=-1;return;}
  }
  double m=1;for(int s=0;s<nslot;s++)if(rule[s]==-1)m*=opts[s].size();
  sols+=m;
  if(!found){found=true;printf("SOLUTION wmask=%d:",wm);for(int s=0;s<nslot;s++){printf(" (%d,%d)->",sp[s],sq[s]);if(rule[s]==-1)printf("*");else if(rule[s]==-2)printf("id");else if(rule[s]==-3)printf("swap");else printf("{%d,%d}",rule[s]>>3,rule[s]&7);}puts("");}
}
int main(int argc,char**argv){
  S=atoi(argv[1]);N=atoi(argv[2]);MODE=atoi(argv[3]);int lo=atoi(argv[4]),hi=atoi(argv[5]);STOP=argc>6?atoi(argv[6]):1;
  memset(slotOf,-1,sizeof slotOf);
  for(int p=0;p<S;p++)for(int q=0;q<S;q++){
    if(MODE==0){ if(p<=q){sp[nslot]=p;sq[nslot]=q;slotOf[p][q]=nslot;nslot++;} }
    else {sp[nslot]=p;sq[nslot]=q;slotOf[p][q]=nslot;nslot++;}
  }
  // in mode 0 a config holding p,q (p<q) is visited with (p,q) and (q,p); only (p,q) has a slot.
  for(int s=0;s<nslot;s++){int p=sp[s],q=sq[s];opts[s].push_back(-2);if(MODE==1&&p!=q)opts[s].push_back(-3);
    for(int a=0;a<S;a++)for(int b=a;b<S;b++){if(a==min(p,q)&&b==max(p,q))continue;opts[s].push_back(a*8+b);}}
  for(int n=2;n<=N;n++)for(int a=0;a<=n;a++){int b=n-a;if(abs(a-b)<2)continue;for(int x=0;x<S;x++)ins.push_back({a,b,x});}
  LASTN=getenv("LASTN")?1:0;
  for(wm=lo;wm<=hi;wm++){for(int s=0;s<nslot;s++)rule[s]=-1;nodes=0;sols=0;found=false;dfs(0);
    printf("S=%d N=%d mode=%d wmask=%d: %s nodes=%llu solutions=%.0f\n",S,N,MODE,wm,found?"FOUND":"NONE",nodes,sols);fflush(stdout);}
}
