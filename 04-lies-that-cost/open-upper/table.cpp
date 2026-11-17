// open-upper: own solver.  Q_k game on obligation multisets (Lemma 1 of the solvers).
// usage: table K JMAX EXTRA   -> for every j<=JMAX and every NICE state (a free, b blocked for good),
//   b >= a-1, a(j+1)+b <= 2^j, 1 <= a+b <= 2j-3+EXTRA : is the Q_K game winnable within j?
// usage: root K n q           -> is Q_K(n) <= q ?
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
using namespace std;
typedef vector<int> St;           // c[r], r=0..j ; r=j means "cannot lie in the remaining j questions"
int K; bool PRUNE=true;
map<pair<int,St>,bool> memo;
bool win(const St&c,int j);
bool tryq(const St&c,int j,St&x,int r,int m){
  if(r<=j){ for(int v=0;v<=c[r];v++){x[r]=v; if(tryq(c,j,x,r+1,m))return true;} return false; }
  int in=0; for(int v:x)in+=v;
  if(2*in>m) return false;                      // complement symmetry
  int ell=min(in,m-in);
  for(int yes=0;yes<2;yes++){
    St d(j,0); int tot=0;
    for(int q=0;q<=j;q++){
      int tr= yes? x[q] : c[q]-x[q]; int li=c[q]-tr;
      int nr=max(q-1,0); if(nr>j-1)nr=j-1;
      d[nr]+=tr; tot+=tr;
      if(q==0){ int o=min(K*ell,j-1); d[o]+=li; tot+=li; }   // liars with obligation 0 survive
    }
    if(tot<=1) continue;                        // illegal answer (0) or win (1)
    if(!win(d,j-1)) return false;
  }
  return true;
}
bool win(const St&c,int j){
  int m=0; for(int v:c)m+=v;
  if(m<=1) return true;
  if(j==0) return false;
  if(PRUNE){ long long w=0; for(int r=0;r<=j;r++) w+=(long long)c[r]*(r==0? j+1 : 1+max(0,j-r)); if(w>(1LL<<j)) return false; }
  auto key=make_pair(j,c); auto it=memo.find(key); if(it!=memo.end())return it->second;
  St x(j+1,0); bool r=tryq(c,j,x,0,m);
  memo[key]=r; return r;
}

// ---- "proof" mode: questioner is FORCED to ask the question prescribed in proofs.txt whenever the state
// is (a free, b blocked for good) with a+b >= 2j-2; real obligation dynamics (min(K*ell, j-1)).
static int Pn(int n){ for(int q=1;;q++){ long long l=(long long)n*(q+1)+((n&1)?(q-1):0); if(l<=(1LL<<q)) return q; } }
map<pair<int,St>,bool> pmemo; long long forced=0, freeS=0;
bool pwin(const St&c,int j){
  int m=0; for(int v:c)m+=v;
  if(m<=1) return true;
  if(j==0) return false;
  int a=c[0], b=c[j]; bool ab=(a+b==m);
  if(!ab || m<2*j-2){ freeS++; return win(c,j); }
  auto key=make_pair(j,c); auto it=pmemo.find(key); if(it!=pmemo.end())return it->second;
  forced++;
  int a1,b1;
  if(a%2==0){ a1=a/2; b1=(b+1)/2; }
  else if(b==0){ a1=(a+1)/2; b1=0; }   // root (n,0), n odd: the root question of Theorem 1
  else { a1=(a+1)/2; int d=((b-(j-1))%2==0)? j-1 : j-2; if(b<d){ pmemo[key]=false; return false; } b1=(b-d)/2; }
  St x(j+1,0); x[0]=a1; x[j]+=b1; if(j==0) x[0]=a1+b1;
  int in=a1+b1; int ell=min(in,m-in); bool ok=true;
  for(int yes=0;yes<2&&ok;yes++){
    St d(j,0); int tot=0;
    for(int q=0;q<=j;q++){
      int tr= yes? x[q] : c[q]-x[q]; int li=c[q]-tr;
      int nr=max(q-1,0); if(nr>j-1)nr=j-1;
      d[nr]+=tr; tot+=tr;
      if(q==0){ int o=min(K*ell,j-1); d[o]+=li; tot+=li; }
    }
    if(tot<=1) continue;
    if(!pwin(d,j-1)) ok=false;
  }
  pmemo[key]=ok; return ok;
}
int main(int argc,char**argv){
  if(string(argv[1])=="proof"){ K=atoi(argv[2]); int n1=atoi(argv[3]),n2=atoi(argv[4]); int bad=0;
    for(int n=n1;n<=n2;n++){ int q=Pn(n); St c(q+1,0); c[0]=n; bool w;
      if((K==2&&(n==3||n==5))||(K==3&&n==3)) w=win(c,q);
      else { St r(q+1,0); r[0]=n; r[q]=0; /* root is (n free, 0 blocked): forced iff n>=2q-2 */ w=pwin(r,q); }
      if(!w){bad++; printf("PROOF-STRATEGY FAILS k=%d n=%d q=%d\n",K,n,q);} if(memo.size()>4000000)memo.clear(); }
    printf("proof k=%d n=%d..%d failures=%d forced-question states=%lld free-search states=%lld\n",K,n1,n2,bad,forced,freeS); return 0; }
  string mode=argv[1];
  if(mode=="root"){ K=atoi(argv[2]); int n=atoi(argv[3]),q=atoi(argv[4]); St c(q+1,0); c[0]=n;
    printf("root k=%d n=%d q=%d win=%d\n",K,n,q,(int)win(c,q)); return 0; }
  K=atoi(argv[2]); int J=atoi(argv[3]), EX=atoi(argv[4]); if(argc>5) PRUNE=false;
  long long tested=0,lost=0;
  for(int j=1;j<=J;j++){ long long tj=0,lj=0;
    for(int a=0;a<=2*j+EX;a++) for(int b=max(a-1,0); a+b<=2*j-3+EX; b++){
      if(a+b<1) continue;
      if((long long)a*(j+1)+b > (1LL<<j)) continue;
      St c(j+1,0); c[0]=a; c[j]+=b;
      bool w=win(c,j); tj++; if(!w){lj++; if(lost+lj<=30) printf("LOST k=%d j=%d a=%d b=%d\n",K,j,a,b);}
    }
    tested+=tj; lost+=lj;
    printf("k=%d j=%d nice small states=%lld lost=%lld memo=%zu\n",K,j,tj,lj,memo.size()); fflush(stdout);
  }
  printf("TOTAL k=%d J=%d extra=%d states=%lld lost=%lld\n",K,J,EX,tested,lost);
}
