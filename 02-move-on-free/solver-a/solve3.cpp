// Exhaustive safety-game solver. usage: solve G M H [sizes-mask=7] [dump]
// G in N,F,O. State: 3 bits/cell: 0 free,1 single,2 start-of-2,3 start-of-3,4 continuation.
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int H,M,SM=7; char G;
vector<u64> st; vector<unsigned char> safe_; 
static inline u64 blk(int s,int i){ u64 k=(u64)(s==1?1:2)<<(2*i); for(int j=1;j<s;j++) k|=(u64)3<<(2*(i+j)); return k;}
static inline int szat(u64 k,int i){ int c=(k>>(2*i))&3; if(c==0||c==3) return 0; if(c==1) return 1; return (i+2<H && ((k>>(2*(i+2)))&3)==3)?3:2; }
static inline u64 msk(int s,int i){ u64 k=0; for(int j=0;j<s;j++) k|=(u64)3<<(2*(i+j)); return k;}
void en(int pos,int live,u64 key){
  if(pos==H){st.push_back(key);return;}
  en(pos+1,live,key);
  for(int s=1;s<=3;s++) if((SM>>(s-1)&1) && pos+s<=H && live+s<=M) en(pos+s,live+s,key|blk(s,pos));
}
vector<unsigned> pre; int SH;
static inline long idx(u64 k){ u64 b=k>>SH; return lower_bound(st.begin()+pre[b],st.begin()+pre[b+1],k)-st.begin(); }
static inline bool isSafe(u64 k){ return safe_[idx(k)]; }
static inline bool freeAt(u64 k,int s,int i){ return i>=0 && i+s<=H && (k&msk(s,i))==0; }
// manager moves with budget
bool anySafe(u64 k,int budget){
  if(isSafe(k)) return true;
  if(budget<=0) return false;
  for(int i=0;i<H;i++){ int c=szat(k,i); if(c==0) continue; int t=c; if(t>budget||t>2) continue;
    u64 k2=k&~msk(t,i);
    for(int j=0;j+t<=H;j++){ if(j==i) continue; if(!freeAt(k2,t,j)) continue;
      if(anySafe(k2|blk(t,j),budget-t)) return true; } }
  return false;
}
bool check(u64 k){
  int live=0; for(int i=0;i<H;i++) if((k>>(2*i))&3) live++;
  for(int s=1;s<=3;s++) if((SM>>(s-1)&1) && live+s<=M){
    bool ok=false; for(int i=0;i+s<=H&&!ok;i++) if(freeAt(k,s,i) && isSafe(k|blk(s,i))) ok=true;
    if(!ok) return false; }
  for(int i=0;i<H;i++){ int c=szat(k,i); if(c==0) continue; int s=c;
    if(G=='O'&&s!=1) continue;
    u64 k2=k&~msk(s,i);
    if(G=='F'){ if(!anySafe(k2,s-1)) return false; } else if(!isSafe(k2)) return false; }
  return true;
}
int main(int argc,char**argv){
  G=argv[1][0]; M=atoi(argv[2]); H=atoi(argv[3]); if(argc>4) SM=atoi(argv[4]); bool dump=argc>5;
  en(0,0,0); sort(st.begin(),st.end()); safe_.assign(st.size(),1); 
  SH=max(0,2*H-24); pre.assign((1u<<24)+2,0);
  for(size_t x=0;x<st.size();x++) pre[(st[x]>>SH)+1]++;
  for(size_t b=1;b<pre.size();b++) pre[b]+=pre[b-1];
  bool ch=true; int sweeps=0;
  while(ch && safe_[0]){ ch=false; sweeps++;
    #pragma omp parallel for schedule(dynamic,8192) num_threads(2) reduction(||:ch)
    for(size_t x=0;x<st.size();x++) if(safe_[x] && !check(st[x])){ safe_[x]=0; ch=true; } }
  size_t ns=0; for(auto b:safe_) ns+=b;
  printf("G=%c M=%d H=%d sizes=%d states=%zu sweeps=%d safe=%zu verdict=%s\n",G,M,H,SM,st.size(),sweeps,ns,safe_[0]?"MANAGER_WINS":"ADVERSARY_WINS");

}
