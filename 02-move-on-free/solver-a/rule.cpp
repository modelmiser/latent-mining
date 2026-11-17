// Exhaustive adversary vs a fixed rule-based manager (unbounded heap, 32 cells max).
// usage: rule G M ruleid   -> prints max extent ever used = H needed by that rule.
// 2 bits/cell: 0 free, 1 single, 2 start of 2/3-block, 3 continuation.
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
const int W=32; int M,R; char G;
static inline int cell(u64 k,int i){return (k>>(2*i))&3;}
static inline u64 blk(int s,int i){ u64 k=(u64)(s==1?1:2)<<(2*i); for(int j=1;j<s;j++) k|=(u64)3<<(2*(i+j)); return k;}
static inline u64 msk(int s,int i){ u64 k=0; for(int j=0;j<s;j++) k|=(u64)3<<(2*(i+j)); return k;}
static inline bool fr(u64 k,int s,int i){ return i>=0&&i+s<=W&&(k&msk(s,i))==0; }
int xtent(u64 k){ int e=0; for(int i=0;i<W;i++) if(cell(k,i)) e=i+1; return e; }
int place(u64 k,int s){
  int ext=xtent(k);
  if(R==0){ for(int i=0;i+s<=W;i++) if(fr(k,s,i)) return i; return -1; }
  if(R==1||R==2){ // best fit over finite gaps, else at end
    int best=-1,bl=1000; int i=0;
    while(i<ext){ if(cell(k,i)){i++;continue;} int j=i; while(j<ext&&!cell(k,j)) j++; int L=j-i;
      if(L>=s){ if(R==2){ if(L==s) return i; if(best<0) best=i; } else if(L<bl){bl=L;best=i;} } i=j; }
    if(best>=0) return best; return ext; }
  if(R==3){ // aligned triples: 3 -> lowest empty triple; 2 -> triple with exactly cells(0,1) or (1,2) free & third occupied, else lowest empty triple (cells 0,1); 1 -> fullest non-empty triple with a free cell, else lowest empty triple
    if(s==3){ for(int t=0;3*t+3<=W;t++) if(fr(k,3,3*t)) return 3*t; return -1; }
    if(s==2){ for(int t=0;3*t+3<=W;t++){ if(fr(k,3,3*t)) continue; if(fr(k,2,3*t)) return 3*t; if(fr(k,2,3*t+1)) return 3*t+1; }
      for(int t=0;3*t+3<=W;t++) if(fr(k,3,3*t)) return 3*t; return -1; }
    int best=-1,bc=-1; for(int t=0;3*t+3<=W&&3*t<ext;t++){ int c=0,f=-1; for(int j=2;j>=0;j--){ if(cell(k,3*t+j)) c++; else f=3*t+j; }
      if(c>0&&c<3&&c>bc){bc=c;best=f;} }
    if(best>=0) return best; for(int t=0;3*t+3<=W;t++) if(fr(k,3,3*t)) return 3*t; return -1; }
  return -1;
}
int main(int argc,char**argv){ G=argv[1][0]; M=atoi(argv[2]); R=atoi(argv[3]);
  unordered_set<u64> seen; vector<u64> q; q.push_back(0); seen.insert(0); int mx=0; u64 worst=0;
  for(size_t h=0;h<q.size();h++){ u64 k=q[h]; int live=0; for(int i=0;i<W;i++) if(cell(k,i)) live++;
    for(int s=1;s<=3;s++) if(live+s<=M){ int p=place(k,s); if(p<0||!fr(k,s,p)){printf("OVERFLOW/BAD\n");return 1;} u64 n=k|blk(s,p); int e=xtent(n); if(e>mx){mx=e;worst=n;} if(seen.insert(n).second) q.push_back(n); }
    for(int i=0;i<W;i++){ int c=cell(k,i); if(c==0||c==3) continue; int s=1; if(c==2){ s=2; if(i+2<W&&cell(k,i+2)==3) s=3; }
      if(G=='O'&&s!=1) continue; u64 n=k&~msk(s,i); if(seen.insert(n).second) q.push_back(n); } }
  string str; for(int i=0;i<mx;i++){ str+=".1[="[cell(worst,i)]; }
  printf("G=%c M=%d rule=%d reachable=%zu H_needed=%d worst=%s\n",G,M,R,q.size(),mx,str.c_str());
}
