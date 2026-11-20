// Fast checker for explicit protocols (same file format as slowcheck.py). s<=16, agents<=15.
// Usage: fastcheck file N [all]  -> first failing instance, or PASS. With 'all', lists every failing instance at the first failing size.
#include <bits/stdc++.h>
using namespace std; typedef unsigned long long u64;
int S,qA,qB; string w; int d[16][16];
static inline int cnt(u64 c,int i){return (c>>(4*i))&15;}
int check(int a,int b,int x){
  int maj=a>b?'A':'B'; u64 init=0; init+=((u64)a)<<(4*qA); init+=((u64)b)<<(4*qB); init+=1ULL<<(4*x);
  unordered_map<u64,int> id; vector<u64> cf; vector<vector<int>> adj; vector<char> fl;
  id.reserve(1<<16); id[init]=0; cf.push_back(init);
  for(size_t i=0;i<cf.size();i++){ u64 c=cf[i]; adj.emplace_back(); fl.push_back(2);
    for(int p=0;p<S;p++){int cp=cnt(c,p); if(!cp)continue; for(int q=0;q<S;q++){int cq=cnt(c,q); if(!cq)continue; if(p==q&&cp<2)continue;
      int r=d[p][q]; int p2=r/S,q2=r%S; if(p2==p&&q2==q)continue; fl[i]&=~2;
      u64 c2=c-(1ULL<<(4*p))-(1ULL<<(4*q))+(1ULL<<(4*p2))+(1ULL<<(4*q2));
      if(c2==c){fl[i]|=1;continue;}
      auto it=id.find(c2); int j; if(it==id.end()){j=cf.size(); id[c2]=j; cf.push_back(c2);} else j=it->second; adj[i].push_back(j);}}
  }
  int n=cf.size(); vector<char> good(n);
  for(int i=0;i<n;i++){bool g=true; for(int p=0;p<S;p++) if(cnt(cf[i],p)&&w[p]!=maj) g=false; good[i]=g; if(!g&&(fl[i]&3)) return 1;}
  vector<int> idx(n,-1),low(n,0),st; vector<char> on(n,0); int ctr=0; vector<pair<int,int>> cs;
  for(int s0=0;s0<n;s0++){ if(idx[s0]>=0)continue; cs.push_back({s0,0}); idx[s0]=low[s0]=ctr++; st.push_back(s0); on[s0]=1;
    while(!cs.empty()){int v=cs.back().first; int&k=cs.back().second;
      if(k<(int)adj[v].size()){int u=adj[v][k++]; if(idx[u]<0){idx[u]=low[u]=ctr++; st.push_back(u); on[u]=1; cs.push_back({u,0});} else if(on[u]) low[v]=min(low[v],idx[u]);}
      else{ if(low[v]==idx[v]){int sz=0;bool bad=false; while(true){int u=st.back(); st.pop_back(); on[u]=0; sz++; if(!good[u])bad=true; if(u==v)break;} if(sz>1&&bad) return 2;}
        int lv=low[v]; cs.pop_back(); if(!cs.empty()){int pv=cs.back().first; low[pv]=min(low[pv],lv);} } } }
  return 0;
}
int main(int argc,char**argv){
  ifstream f(argv[1]); string line; vector<string> L; while(getline(f,line)){ size_t h=line.find('#'); if(h!=string::npos) line=line.substr(0,h); bool e=true; for(char c:line) if(!isspace(c)) e=false; if(!e) L.push_back(line);}
  S=atoi(L[0].c_str()); sscanf(L[1].c_str(),"%d %d",&qA,&qB); { stringstream ss(L[2]); ss>>w; }
  for(int p=0;p<S;p++)for(int q=0;q<S;q++) d[p][q]=p*S+q;
  for(size_t i=3;i<L.size();i++){int p,q,p2,q2; sscanf(L[i].c_str(),"%d %d %d %d",&p,&q,&p2,&q2); d[p][q]=p2*S+q2;}
  int N=atoi(argv[2]); bool all=argc>3;
  for(int n=2;n<=N;n++){ bool failed=false; for(int a=0;a<=n;a++){int b=n-a; if(abs(a-b)<2)continue; for(int x=0;x<S;x++){ int r=check(a,b,x); if(r){ printf("FAIL at a+b=%d: (a,b,x)=(%d,%d,%d) %s\n",n,a,b,x,r==1?"bad terminal/self-loop":"bad cycle"); failed=true; if(!all) return 0; } } } if(failed) return 0; }
  printf("PASS all instances with 2<=a+b<=%d\n",N);
}
