// Referee checker, written from the problem statement.
// Format: line1 "s qA qB"; line2 w (A/B string); then "p q p2 q2" lines = d(p,q)=(p2,q2); unlisted = identity.
// usage: chk file Nmax [Nmin]   -> per-N result, first failing instance.
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 K;
struct H{size_t operator()(K k)const{uint64_t a=(uint64_t)k,b=(uint64_t)(k>>64);a^=b*0x9E3779B97F4A7C15ULL;a^=a>>29;a*=0xBF58476D1CE4E5B9ULL;a^=a>>32;return a;}};
int S,qA,qB; string w; int d1[32][32],d2[32][32];
size_t maxR=0;
static inline K pk(const int*c){K k=0;for(int i=S-1;i>=0;i--)k=(k<<5)|(K)c[i];return k;}
static inline void up(K k,int*c){for(int i=0;i<S;i++){c[i]=(int)(k&31);k>>=5;}}
// returns "" if won, else reason
string inst(int a,int b,int x){
  char maj=a>b?'A':'B';
  int c0[32]={0};c0[qA]+=a;c0[qB]+=b;c0[x]++;
  unordered_map<K,int,H> id; vector<K> keys; vector<int> idx,low; vector<char> onst,selfl;
  vector<int> tst; // tarjan stack
  struct F{int v;vector<int> su;size_t i;};
  vector<F> st;
  auto good=[&](K k){int c[32];up(k,c);for(int i=0;i<S;i++)if(c[i]&&w[i]!=maj)return false;return true;};
  auto show=[&](K k){int c[32];up(k,c);string s="{";for(int i=0;i<S;i++)if(c[i])s+=to_string(i)+"x"+to_string(c[i])+" ";return s+"}";};
  auto get=[&](K k)->int{auto it=id.find(k);if(it!=id.end())return it->second;int n=keys.size();id.emplace(k,n);keys.push_back(k);idx.push_back(-1);low.push_back(0);onst.push_back(0);selfl.push_back(0);return n;};
  auto succ=[&](int v,vector<int>&out){int c[32];up(keys[v],c);out.clear();
    for(int p=0;p<S;p++)if(c[p])for(int q=0;q<S;q++)if(c[q]&&(p!=q||c[p]>=2)){
      int r=d1[p][q],t=d2[p][q]; if(r==p&&t==q)continue;
      c[p]--;c[q]--;c[r]++;c[t]++;K k=pk(c);c[r]--;c[t]--;c[p]++;c[q]++;
      int u=get(k); if(u==v)selfl[v]=1; else out.push_back(u);} };
  int counter=0; string res="";
  int r0=get(pk(c0));
  st.push_back({r0,{},0}); idx[r0]=low[r0]=counter++; tst.push_back(r0); onst[r0]=1; {vector<int> s;succ(r0,s);st.back().su=s;
    if(s.empty()&&!selfl[r0]&&!good(keys[r0]))return "bad terminal "+show(keys[r0]);}
  while(!st.empty()){
    F&f=st.back();
    if(f.i<f.su.size()){int u=f.su[f.i++];
      if(idx[u]<0){idx[u]=low[u]=counter++;tst.push_back(u);onst[u]=1;vector<int> s;succ(u,s);
        if(s.empty()&&!selfl[u]&&!good(keys[u])){res="bad terminal "+show(keys[u]);break;}
        st.push_back({u,s,0});}
      else if(onst[u]){int v=f.v;low[v]=min(low[v],idx[u]);}
    } else {int v=f.v;
      if(low[v]==idx[v]){vector<int> comp;while(true){int u=tst.back();tst.pop_back();onst[u]=0;comp.push_back(u);if(u==v)break;}
        bool cyc=comp.size()>1; for(int u:comp)if(selfl[u])cyc=true; // any selfloop member alone is on a cycle
        for(int u:comp){bool on=comp.size()>1||selfl[u]; if(on&&!good(keys[u])){res="bad cycle "+show(keys[u]);break;}}
        if(res!="")break;}
      st.pop_back(); if(!st.empty()){int p=st.back().v;low[p]=min(low[p],low[v]);}
    }
  }
  maxR=max(maxR,keys.size());
  return res;
}
int main(int argc,char**argv){
  ifstream f(argv[1]); int Nmax=atoi(argv[2]); int Nmin=argc>3?atoi(argv[3]):2;
  string line; vector<string> L; while(getline(f,line)){size_t h=line.find('#');if(h!=string::npos)line=line.substr(0,h);bool e=true;for(char ch:line)if(!isspace(ch))e=false;if(!e)L.push_back(line);}
  {istringstream is(L[0]);is>>S>>qA>>qB;} {istringstream is(L[1]);is>>w;}
  if((int)w.size()!=S||qA==qB||S>25){puts("BAD FILE");return 2;}
  for(int p=0;p<S;p++)for(int q=0;q<S;q++){d1[p][q]=p;d2[p][q]=q;}
  for(size_t i=2;i<L.size();i++){istringstream is(L[i]);int p,q,r,t;is>>p>>q>>r>>t;if(p<0||q<0||r<0||t<0||p>=S||q>=S||r>=S||t>=S){puts("BAD RULE");return 2;}d1[p][q]=r;d2[p][q]=t;}
  if(Nmax+1>31){puts("N too large for packing");return 2;}
  int wins=1;
  for(int n=2;n<=Nmax;n++){ if(n<Nmin){wins=n;continue;}
    bool ok=true;
    for(int a=0;a<=n&&ok;a++){int b=n-a;if(abs(a-b)<2)continue;
      for(int x=0;x<S&&ok;x++){string r=inst(a,b,x);if(r!=""){ok=false;printf("FAIL n=%d (a=%d,b=%d,x=%d): %s\n",n,a,b,x,r.c_str());}}}
    if(!ok)break; wins=n; fprintf(stderr,"ok n=%d maxR=%zu\n",n,maxR);
  }
  printf("RESULT wins all 2<=a+b<=%d (tested from %d to %d) maxR=%zu\n",wins,Nmin,Nmax,maxR);
}
