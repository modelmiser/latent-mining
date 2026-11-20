// check <system file> : prints OK / ILLEGAL / SAFETY / PROGRESS
#include "mc.h"
int main(int argc,char**argv){
  int n,b; static Aut a[MAXN];
  if(argc<2||!readsys(argv[1],n,b,a)){ printf("cannot read\n"); return 9; }
  for(int i=0;i<n;i++){ int l=legal(a[i],b); if(l){ printf("ILLEGAL process %d rule %d\n",i+1,l); return 3; } }
  MC m; m.n=n; m.b=b; for(int i=0;i<n;i++) m.A[i]=&a[i];
  int r=m.check();
  int mx=0; for(int i=0;i<n;i++) mx=std::max(mx,a[i].ns);
  if(r==0) printf("OK n=%d b=%d max_states=%d reachable_configs=%zu\n",n,b,mx,m.order.size());
  else { int st[MAXN],R; m.dec(m.wit,st,R); printf(r==1?"SAFETY":"PROGRESS"); printf(" violated at config states="); for(int i=0;i<n;i++) printf("%d ",st[i]); printf("R=%d",R); if(r==2) printf(" moverset=%d",m.witM); printf("\n"); }
  return r;
}
