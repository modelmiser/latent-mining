// Heuristic (NOT exhaustive) lower-bound search: simulated annealing over trees.
// usage: sa r m seed iters T0 [initfile]
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <random>
int r,m; int NN; // nodes 1..2^(r+1)-1
struct Tree{ unsigned char isInt[64], idx[64], rsel[64]; };
std::mt19937_64 rng;
int depthOf(int v){ return 31-__builtin_clz(v); }
bool valid(const Tree&t){ // no index repeated on a path
  for(int v=1;v<(1<<r);v++){ if(!t.isInt[v]) continue; bool reach=true; for(int u=v>>1;u>=1;u>>=1){ if(!t.isInt[u]){reach=false;break;} if(t.idx[u]==t.idx[v]) return false; } (void)reach; }
  return true; }
std::vector<int> F, mark, pos;
int eval(const Tree&t,int*startOut=0){
  int S=1<<m;
  for(int s=0;s<S;s++){ int v=1; unsigned P=0; int path[8]; int L=0;
    while(v<(1<<r) && t.isInt[v]){ int i=t.idx[v]; path[L++]=i; P|=1u<<i; v=2*v+((s>>i)&1); }
    int ns=s&~P; int rs=t.rsel[v]; if(rs>L) rs=L; if(rs) ns|=1<<path[rs-1]; F[s]=ns; }
  int best=0; std::fill(mark.begin(),mark.end(),0); int stamp=0;
  for(int s=0;s<S;s++){ if(mark[s]) continue; int st=++stamp; int x=s,n=0; while(!mark[x]){mark[x]=st;pos[x]=n++;x=F[x];}
    if(mark[x]==st){int L=n-pos[x]; if(L>best){best=L; if(startOut)*startOut=x;}} }
  return best; }
void dump(const Tree&t,int N,int start,const char*fn){
  FILE*f=fopen(fn,"w"); fprintf(f,"r %d\nN %d\nstart",r,N); for(int i=0;i<(1<<r)-1;i++) fprintf(f," %d",i<m?(start>>i)&1:0); fprintf(f,"\n");
  std::vector<int> st; st.push_back(1);
  while(!st.empty()){int v=st.back(); st.pop_back(); char p[32]; int L=0; for(int b=depthOf(v)-1;b>=0;b--) p[L++]='0'+((v>>b)&1); p[L]=0;
    if(v<(1<<r)&&t.isInt[v]){fprintf(f,"%s I %d\n",L?p:"-",t.idx[v]+1); st.push_back(2*v+1); st.push_back(2*v);}
    else { int path[8]; int k=0; for(int b=depthOf(v);b>=1;b--){ int u=v>>b; path[k++]=t.idx[u]+1; } int rs=t.rsel[v]; if(rs>k) rs=k; fprintf(f,"%s L %d\n",L?p:"-",rs?path[rs-1]:0);} }
  fclose(f); }
int main(int argc,char**argv){
  r=atoi(argv[1]); m=atoi(argv[2]); long seed=atol(argv[3]); long iters=atol(argv[4]); double T0=atof(argv[5]);
  rng.seed(seed); NN=(1<<(r+1)); F.resize(1<<m); mark.resize(1<<m); pos.resize(1<<m);
  Tree t; memset(&t,0,sizeof t);
  if(argc>6){ // init from witness file
    FILE*f=fopen(argv[6],"r"); char a[64],k[8]; int x; char line[256]; 
    while(fgets(line,256,f)){ if(sscanf(line,"%s %s %d",a,k,&x)!=3) continue; if(strcmp(k,"I")&&strcmp(k,"L")) continue;
      int v=1; if(a[0]!='-') for(char*c=a;*c;c++) v=2*v+(*c-'0');
      if(k[0]=='I'){t.isInt[v]=1;t.idx[v]=x-1;} else { t.isInt[v]=0; int rs=0; int d=depthOf(v); for(int b=d;b>=1;b--){ if(t.idx[v>>b]+1==x) rs=d-b+1; } t.rsel[v]=rs; } }
    fclose(f);
  } else { do{ for(int v=1;v<NN;v++){ t.isInt[v]=(v<(1<<r))&&(rng()%8!=0); t.idx[v]=rng()%m; t.rsel[v]=rng()%(r+1);} }while(!valid(t)); }
  int cur=eval(t); int best=cur; Tree bt=t; printf("init %d\n",cur);
  for(long it=0;it<iters;it++){
    Tree u=t; int nm=1+(rng()%3==0);
    for(int q=0;q<nm;q++){ int v=1+rng()%(NN-1); int c=rng()%4;
      if(v>=(1<<r)||c==0) u.rsel[v]=rng()%(r+1);
      else if(c==1) u.isInt[v]^= (rng()%4==0);
      else if(c==2) u.idx[v]=rng()%m;
      else { int w=1+rng()%((1<<r)-1); std::swap(u.idx[v],u.idx[w]); } }
    if(!valid(u)) continue;
    int e=eval(u); double T=T0*(1.0-(double)it/iters)+1e-9;
    if(e>=cur || std::exp((std::log((double)e)-std::log((double)cur))/T) > (rng()%1000000)/1e6){ t=u; cur=e; if(e>best){best=e;bt=t; printf("it %ld best %d\n",it,best); fflush(stdout);} }
  }
  int st=0; eval(bt,&st); char fn[64]; sprintf(fn,"sa_r%d_m%d_seed%ld_N%d.txt",r,m,seed,best); dump(bt,best,st,fn);
  printf("final best %d\n",best);
}
