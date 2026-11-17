// referee layered count: dp n L mode(slip|del|up) dom(0|1)
// state = (image p of reversed input, set Q of images of the reversed input under exactly one fault).
// Counts words of size exactly L with p and all of Q sorted at the end (reversed input only).
#include <bits/stdc++.h>
using namespace std;
int n,L; int NP; vector<array<int8_t,8>> P; map<array<int8_t,8>,int> idx; vector<vector<uint16_t>> ap; vector<uint8_t> inv; vector<vector<uint8_t>> dom;
int main(int c,char**v){ n=atoi(v[1]); L=atoi(v[2]); string mode=v[3]; int DOM=atoi(v[4]);
  array<int8_t,8> a{}; for(int i=0;i<n;i++)a[i]=i+1;
  do{ idx[a]=P.size(); P.push_back(a);}while(next_permutation(a.begin(),a.begin()+n)); NP=P.size();
  ap.assign(n,vector<uint16_t>(NP)); inv.assign(NP,0);
  for(int k=0;k<NP;k++){ for(int i=1;i<n;i++){ auto b=P[k]; if(b[i-1]>b[i]) swap(b[i-1],b[i]); ap[i][k]=idx[b]; }
    for(int i=0;i<n;i++)for(int j=i+1;j<n;j++) if(P[k][i]>P[k][j]) inv[k]++; }
  if(DOM){ // dom[x][y]=1 iff x <= y : for every threshold t and prefix j, #(>=t in first j of x) <= same for y
    vector<array<uint8_t,64>> pre(NP); 
    for(int k=0;k<NP;k++) for(int t=2;t<=n;t++){int s=0; for(int j=0;j<n;j++){ s+=(P[k][j]>=t); pre[k][(t-2)*n+j]=s; }}
    dom.assign(NP,vector<uint8_t>(NP,0)); int m=(n-1)*n;
    for(int x=0;x<NP;x++)for(int y=0;y<NP;y++){ bool ok=true; for(int q=0;q<m&&ok;q++) if(pre[x][q]>pre[y][q]) ok=false; dom[x][y]=ok; } }
  array<int8_t,8> r{}; for(int i=0;i<n;i++) r[i]=n-i; int start=idx[r]; array<int8_t,8> so{}; for(int i=0;i<n;i++) so[i]=i+1; int goal=idx[so];
  typedef basic_string<char16_t> Key; unordered_map<Key,unsigned long long> cur,nxt; 
  struct H{}; cur[Key(1,(char16_t)start)]=1;
  for(int lev=1;lev<=L;lev++){ nxt.clear(); int rem=L-lev;
    for(auto&kv:cur){ const Key&s=kv.first; int p=s[0];
      for(int i=1;i<n;i++){ int p2=ap[i][p]; if(inv[p2]>rem) continue; vector<uint16_t> q; bool bad=false;
        for(size_t k=1;k<s.size();k++){ int z=ap[i][s[k]]; if(inv[z]>rem){bad=true;break;} q.push_back(z);} if(bad) continue;
        if(mode=="del") q.push_back(p);
        else { if(i+1<=n-1) q.push_back(ap[i+1][p]); if(mode=="slip" && i-1>=1) q.push_back(ap[i-1][p]); }
        for(size_t k=0;k<q.size();k++) if(inv[q[k]]>rem){bad=true;break;} if(bad) continue;
        sort(q.begin(),q.end()); q.erase(unique(q.begin(),q.end()),q.end());
        if(DOM){ vector<uint16_t> q2; for(auto z:q){ bool d=dom[z][p2]; for(auto y:q) if(!d && y!=z && dom[z][y]) d=true; if(!d) q2.push_back(z);} q.swap(q2); }
        Key t; t.push_back(p2); for(auto z:q) t.push_back(z); nxt[t]+=kv.second; } }
    cur.swap(nxt); fprintf(stderr,"lev %d states %zu\n",lev,cur.size()); }
  unsigned long long acc=0; for(auto&kv:cur){ bool ok=kv.first[0]==goal; for(size_t k=1;k<kv.first.size();k++) if(kv.first[k]!=goal) ok=false; if(ok) acc+=kv.second; }
  printf("n=%d L=%d mode=%s dom=%d accepted=%llu\n",n,L,mode.c_str(),DOM,acc); }
