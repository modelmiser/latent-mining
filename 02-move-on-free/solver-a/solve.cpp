// Exhaustive safety-game solver. usage: solve G M H [sizes-mask=7] [dump]
// G in N,F,O. State: 3 bits/cell: 0 free,1 single,2 start-of-2,3 start-of-3,4 continuation.
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int H,M,SM=7; char G;
vector<u64> st; vector<unsigned char> safe_; vector<int> rnk;
static inline u64 blk(int s,int i){ u64 k=(u64)(s==1?1:s)<<(3*i); for(int j=1;j<s;j++) k|=(u64)4<<(3*(i+j)); return k;}
static inline u64 msk(int s,int i){ u64 k=0; for(int j=0;j<s;j++) k|=(u64)7<<(3*(i+j)); return k;}
void en(int pos,int live,u64 key){
  if(pos==H){st.push_back(key);return;}
  en(pos+1,live,key);
  for(int s=1;s<=3;s++) if((SM>>(s-1)&1) && pos+s<=H && live+s<=M) en(pos+s,live+s,key|blk(s,pos));
}
static inline long idx(u64 k){ return lower_bound(st.begin(),st.end(),k)-st.begin(); }
static inline bool isSafe(u64 k){ return safe_[idx(k)]; }
static inline bool freeAt(u64 k,int s,int i){ return i>=0 && i+s<=H && (k&msk(s,i))==0; }
// manager moves with budget
bool anySafe(u64 k,int budget){
  if(isSafe(k)) return true;
  if(budget<=0) return false;
  for(int i=0;i<H;i++){ int c=(k>>(3*i))&7; if(c==0||c==4) continue; int t=c; if(t>budget||t>2) continue;
    u64 k2=k&~msk(t,i);
    for(int j=0;j+t<=H;j++){ if(j==i) continue; if(!freeAt(k2,t,j)) continue;
      if(anySafe(k2|blk(t,j),budget-t)) return true; } }
  return false;
}
bool check(u64 k){
  int live=0; for(int i=0;i<H;i++) if((k>>(3*i))&7) live++;
  for(int s=1;s<=3;s++) if((SM>>(s-1)&1) && live+s<=M){
    bool ok=false; for(int i=0;i+s<=H&&!ok;i++) if(freeAt(k,s,i) && isSafe(k|blk(s,i))) ok=true;
    if(!ok) return false; }
  for(int i=0;i<H;i++){ int c=(k>>(3*i))&7; if(c==0||c==4) continue; int s=c;
    if(G=='O'&&s!=1) continue;
    u64 k2=k&~msk(s,i);
    if(G=='F'){ if(!anySafe(k2,s-1)) return false; } else if(!isSafe(k2)) return false; }
  return true;
}
int main(int argc,char**argv){
  G=argv[1][0]; M=atoi(argv[2]); H=atoi(argv[3]); if(argc>4) SM=atoi(argv[4]); bool dump=argc>5;
  en(0,0,0); sort(st.begin(),st.end()); safe_.assign(st.size(),1); rnk.assign(st.size(),0);
  bool ch=true; int sweeps=0;
  while(ch && safe_[0]){ ch=false; sweeps++;
    for(size_t x=0;x<st.size();x++) if(safe_[x] && !check(st[x])){ safe_[x]=0; rnk[x]=sweeps; ch=true; } }
  size_t ns=0; for(auto b:safe_) ns+=b;
  printf("G=%c M=%d H=%d sizes=%d states=%zu sweeps=%d safe=%zu verdict=%s\n",G,M,H,SM,st.size(),sweeps,ns,safe_[0]?"MANAGER_WINS":"ADVERSARY_WINS");
  if(dump){ for(size_t x=0;x<st.size();x++){ if(!safe_[x]&&!rnk[x]) continue; string s; for(int i=0;i<H;i++){int c=(st[x]>>(3*i))&7; s+=".123-"[c];} printf("%s %d %d\n",s.c_str(),(int)safe_[x],rnk[x]); } }
}
