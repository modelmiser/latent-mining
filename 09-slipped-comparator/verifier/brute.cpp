// referee brute force: n L. Enumerates all words of size L whose image of the reversed input is sorted
// (necessary for sorting), classifies each from the definition (0-1 inputs, then all n! for accepted).
#include <bits/stdc++.h>
using namespace std;
int n,L; vector<int> w; 
long long cntRev=0,cntSort=0,cSlip=0,cDel=0,cUp=0,slipNotDel=0,delNotSlip=0,permMismatch=0;
static inline unsigned app01(const int*a,int m,unsigned x){ for(int k=0;k<m;k++){int i=a[k]; if(i==0)continue; // 0 = skip (deleted)
    if(((x>>(i-1))&1) && !((x>>i)&1)) x^=(3u<<(i-1));} return x;}
bool sorted01(unsigned x){ for(int i=0;i+1<n;i++) if(((x>>i)&1)&&!((x>>(i+1))&1)) return false; return true;}
bool sorts01(const int*a,int m){ for(unsigned x=0;x<(1u<<n);x++) if(!sorted01(app01(a,m,x))) return false; return true;}
bool sortsPerm(const int*a,int m){ vector<int> p(n); iota(p.begin(),p.end(),1);
  do{ vector<int> v=p; for(int k=0;k<m;k++){int i=a[k]; if(!i)continue; if(v[i-1]>v[i]) swap(v[i-1],v[i]);}
      for(int i=0;i+1<n;i++) if(v[i]>v[i+1]) return false; }while(next_permutation(p.begin(),p.end())); return true;}
void leaf(){ cntRev++; int a[64]; for(int k=0;k<L;k++)a[k]=w[k];
  bool s=sorts01(a,L); if(!s) return; cntSort++;
  bool slip=true,del=true,up=true;
  for(int k=0;k<L;k++){ int i=a[k];
    a[k]=0; if(!sorts01(a,L)) del=false;
    if(i+1<=n-1){ a[k]=i+1; if(!sorts01(a,L)){slip=false;up=false;} }
    if(i-1>=1){ a[k]=i-1; if(!sorts01(a,L)) slip=false; }
    a[k]=i; }
  if(slip){ // re-test on all permutations, literally the definition
    bool ok=sortsPerm(a,L);
    for(int k=0;k<L&&ok;k++){int i=a[k]; if(i+1<=n-1){a[k]=i+1; ok=ok&&sortsPerm(a,L);} if(i-1>=1){a[k]=i-1; ok=ok&&sortsPerm(a,L);} a[k]=i;}
    if(!ok) permMismatch++; cSlip++; if(cSlip<=2){printf("  example slip-proof:");for(int k=0;k<L;k++)printf(" %d",a[k]);puts("");} }
  if(del) cDel++; if(up){cUp++; if(cUp<=1){printf("  example up-proof:");for(int k=0;k<L;k++)printf(" %d",a[k]);puts("");}}
  if(slip&&!del) slipNotDel++; if(del&&!slip) delNotSlip++; }
void dfs(vector<int>&p,int inv){ int rem=L-(int)w.size(); if(inv>rem) return; if(rem==0){ leaf(); return; }
  for(int i=1;i<n;i++){ w.push_back(i); if(p[i-1]>p[i]){ swap(p[i-1],p[i]); dfs(p,inv-1); swap(p[i-1],p[i]); } else dfs(p,inv); w.pop_back(); } }
int main(int c,char**v){ n=atoi(v[1]); L=atoi(v[2]); vector<int> p(n); for(int i=0;i<n;i++)p[i]=n-i;
  dfs(p,n*(n-1)/2);
  printf("n=%d L=%d sortRev=%lld sortAll=%lld slip=%lld del=%lld up=%lld slip&!del=%lld del&!slip=%lld permMismatch=%lld\n",n,L,cntRev,cntSort,cSlip,cDel,cUp,slipNotDel,delNotSlip,permMismatch); }
