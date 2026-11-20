// Referee raw brute force, straight from the definition. No reductions at all.
// Each position: owner in {H,1..n}; H: card 0/1; player: (u0,u1) in {0,1}^2. All non-empty R.
// Prints number of valid (protocol,R) pairs, weak pairs (private, M(1)!=M(0)), min|R|, whether R=Z_m occurs.
#include <bits/stdc++.h>
using namespace std;
int n,m;
int main(int argc,char**argv){
  n=atoi(argv[1]); m=atoi(argv[2]);
  int S=2+4*n; // states per position
  vector<int> st(m,0);
  unsigned long long valid=0,weak=0,priv=0; int minR=99; bool full=false;
  int NX=1<<n;
  vector<unsigned> row(NX);
  vector<vector<unsigned>> win(NX, vector<unsigned>(m));
  while(true){
    // decode
    bool ok=true; int seen=0;
    for(int p=0;p<m;p++) if(st[p]>=2) seen|=1<<((st[p]-2)/4);
    if(seen!=NX-1) ok=false;
    if(ok){
      for(int x=0;x<NX;x++){ unsigned w=0;
        for(int p=0;p<m;p++){ int s=st[p]; int card;
          if(s<2) card=s; else { int i=(s-2)/4; int uu=(s-2)%4; card = ((x>>i)&1)? (uu>>1)&1 : uu&1; }
          w|=card<<p; }
        row[x]=w; }
      for(unsigned R=1;R<(1u<<m);R++){
        for(int x=0;x<NX;x++){
          for(int j=0;j<m;j++){ unsigned t=0;
            for(int q=0;q<m;q++) if(R>>q&1){ int p=((q-j)%m+m)%m; t|=((row[x]>>p)&1)<<q; }
            win[x][j]=t; }
          sort(win[x].begin(),win[x].end()); }
        bool pr=true;
        for(int x=1;x<NX-1&&pr;x++) if(win[x]!=win[0]) pr=false;
        if(!pr) continue;
        priv++;
        if(win[NX-1]!=win[0]) weak++;
        vector<unsigned> common;
        set_intersection(win[0].begin(),win[0].end(),win[NX-1].begin(),win[NX-1].end(),back_inserter(common));
        if(common.empty()){ valid++; int c=__builtin_popcount(R); minR=min(minR,c); if(c==m) full=true; }
      }
    }
    int p=0; while(p<m && ++st[p]==S){ st[p]=0; p++; }
    if(p==m) break;
  }
  printf("RAW n=%d m=%d private=%llu weak=%llu valid=%llu minR=%d fullR=%d\n",n,m,priv,weak,valid,minR==99?0:minR,(int)full);
}
