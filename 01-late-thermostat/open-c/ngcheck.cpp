// ngcheck (C++): same checks as ngcheck.py, for certificates too large for Python.
// Written from the checker's specification (not from ngclock.cpp): states are decoded to
// explicit (x, window[], rest) records and re-encoded.
// Certificate: 'd D B BAND', then nogoods in learned order 'k s1..sk'.
// For EVERY nogood K_i and EVERY map f in {-1,0,1}^3: the closed one-step image of K_i under f
// leaves the band or contains a nogood K_j, j<i.  Finally the initial set contains a nogood.
// usage: ngcheck certfile [override_band]
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;
int d,B,Bchk; long long P3;
struct Rec{int x; int win[40]; int r;};
static int sgn(int v){return v>0?1:(v<0?-1:0);}
Rec dec(long long s){Rec R; R.r=(int)(s&1); long long q=s>>1; long long sc=q%P3; R.x=(int)(q/P3)-B;
  for(int i=d-1;i>=0;i--){R.win[i]=(int)(sc%3)-1; sc/=3;} return R;}
long long encr(const Rec&R){long long sc=0; for(int i=0;i<d;i++)sc=sc*3+(R.win[i]+1); return (((long long)(R.x+B))*P3+sc)*2+R.r;}
int main(int argc,char**argv){
  FILE*f=fopen(argv[1],"r"); if(!f){printf("FAIL: cannot open\n");return 1;}
  char a[8],b[8]; if(fscanf(f,"%7s %d %7s %d",a,&d,b,&B)!=4){printf("FAIL: header\n");return 1;}
  Bchk=argc>2?atoi(argv[2]):B; P3=1; for(int i=0;i<d;i++)P3*=3; long long NS=(2LL*B+1)*P3*2;
  vector<vector<int> > ng; vector<vector<int> > bucket(NS); vector<int> mark(NS,0); int mc=0;
  long long count=0; int k;
  vector<int> K,T;
  while(fscanf(f,"%d",&k)==1){
    K.resize(k); for(int i=0;i<k;i++){ if(fscanf(f,"%d",&K[i])!=1){printf("FAIL: truncated certificate at nogood %lld\n",count);return 1;} if(K[i]<0||K[i]>=NS){printf("FAIL: bad state id\n");return 1;} }
    if(k==0){printf("FAIL: empty nogood %lld\n",count);return 1;}
    vector<Rec> recs(k); for(int i=0;i<k;i++)recs[i]=dec(K[i]);
    for(int fm=0;fm<3;fm++)for(int fz=0;fz<3;fz++)for(int fp=0;fp<3;fp++){
      int fv[3]={fm-1,fz-1,fp-1}; bool viol=false; T.clear(); ++mc;
      for(int i=0;i<k&&!viol;i++){const Rec&R=recs[i]; int u=fv[R.win[0]+1];
        Rec N; for(int j=0;j+1<d;j++)N.win[j]=R.win[j+1]; N.win[d-1]=sgn(R.x);
        for(int w=-1;w<=1;w++){ if(R.r&&w!=0)continue; int nx=R.x+u+w; if(nx>Bchk||nx<-Bchk){viol=true;break;}
          if(nx>B||nx<-B)continue; /* only when override_band>B: state outside the certificate's universe, cannot belong to any nogood */
          N.x=nx; N.r=(w!=0); long long id=encr(N); if(mark[id]!=mc){mark[id]=mc;T.push_back((int)id);}
          if(w==0){N.r=1; long long id2=encr(N); if(mark[id2]!=mc){mark[id2]=mc;T.push_back((int)id2);}}
        }}
      if(viol)continue;
      bool found=false;
      for(size_t ti=0;ti<T.size()&&!found;ti++){ const vector<int>&bk=bucket[T[ti]];
        for(size_t bi=0;bi<bk.size()&&!found;bi++){ const vector<int>&E=ng[bk[bi]]; bool ok=true;
          for(size_t e=0;e<E.size();e++) if(mark[E[e]]!=mc){ok=false;break;}
          if(ok)found=true; } }
      if(!found){printf("FAIL: nogood #%lld (size %d) not refuted for f=(%d,%d,%d)\n",count,k,fv[0],fv[1],fv[2]);return 1;}
    }
    // index under its element with the currently shortest bucket (any choice is fine for correctness)
    int best=K[0]; for(int i=1;i<k;i++) if(bucket[K[i]].size()<bucket[best].size())best=K[i];
    bucket[best].push_back((int)ng.size()); ng.push_back(K); count++;
    if(count%100000==0){fprintf(stderr,"  checked %lld\n",count);}
  }
  Rec R0; R0.x=0; for(int i=0;i<d;i++)R0.win[i]=0; R0.r=0; long long s0=encr(R0); R0.r=1; long long s1=encr(R0);
  ++mc; mark[s0]=mc; mark[s1]=mc; bool found=false; long long two[2]={s0,s1};
  for(int t=0;t<2&&!found;t++){ const vector<int>&bk=bucket[two[t]];
    for(size_t bi=0;bi<bk.size()&&!found;bi++){ const vector<int>&E=ng[bk[bi]]; bool ok=true; for(size_t e=0;e<E.size();e++) if(mark[E[e]]!=mc){ok=false;break;} if(ok)found=true; } }
  if(!found){printf("FAIL: initial set contains no nogood\n");return 1;}
  printf("OK: d=%d band=%d: %lld nogoods verified (27 maps each); initial set is dead => no clock holds |x|<=%d\n",d,Bchk,count,Bchk);
  return 0;
}
