// Exhaustive BFS for charged pebbles. Usage: bfs p k N [witnessfile]
// State: p entries of 12 bits (pos<<4|charge), pos=0 means in hand; sorted descending, packed in uint64.
// Position cap N: P-moves onto nodes > N are suppressed. Result is exact iff maxocc < N (cap never binding).
#include <bits/stdc++.h>
using namespace std;
typedef uint64_t u64; typedef uint32_t u32;
int p,k,N;
vector<u64> states; vector<u32> par;
vector<u32> table; u64 mask;
static inline u64 hsh(u64 x){ x^=x>>33; x*=0xff51afd7ed558ccdULL; x^=x>>33; x*=0xc4ceb9fe1a85ec53ULL; x^=x>>33; return x; }
void grow(){
  size_t ns=table.size()*2; vector<u32>().swap(table); table.assign(ns,0xffffffffu); mask=ns-1;
  for(u32 i=0;i<states.size();i++){ u64 h=hsh(states[i])&mask; while(table[h]!=0xffffffffu) h=(h+1)&mask; table[h]=i; }
}
bool insert(u64 s,u32 parent){
  u64 h=hsh(s)&mask;
  while(table[h]!=0xffffffffu){ if(states[table[h]]==s) return false; h=(h+1)&mask; }
  table[h]=states.size(); states.push_back(s); par.push_back(parent);
  if(states.size()*2>table.size()) grow();
  return true;
}
static inline u64 pack(int*e){ // sort descending
  for(int i=1;i<p;i++){int v=e[i],j=i-1; while(j>=0&&e[j]<v){e[j+1]=e[j];j--;} e[j+1]=v;}
  u64 s=0; for(int i=0;i<p;i++) s=(s<<12)|(u64)e[i]; return s;
}
static inline void unpack(u64 s,int*e){ for(int i=p-1;i>=0;i--){e[i]=s&0xfff; s>>=12;} }
int main(int argc,char**argv){
  p=atoi(argv[1]);k=atoi(argv[2]);N=atoi(argv[3]);
  if(p>5||k>15||N>250){fprintf(stderr,"range\n");return 2;}
  table.assign(1<<20,0xffffffffu); mask=table.size()-1;
  int e[8],f[8]; for(int i=0;i<p;i++) e[i]=k;
  insert(pack(e),0xffffffffu);
  int best=0,maxocc=0; u32 bestidx=0;
  static bool occ[300];
  for(u32 q=0;q<states.size();q++){
    if(states.size()>0xfff00000u){fprintf(stderr,"too many states\n");return 3;}
    u64 s=states[q]; unpack(s,e);
    int nb=0; for(int i=0;i<p;i++) if(e[i]>>4){occ[e[i]>>4]=true;nb++;}
    occ[0]=true;
    if(nb){ int top=e[0]>>4; if(top>maxocc)maxocc=top; if(nb==1&&top>best){best=top;bestidx=q;} }
    // removals
    for(int i=0;i<nb;i++){
      int pos=e[i]>>4,c=e[i]&15;
      if(occ[pos-1]){ memcpy(f,e,sizeof(int)*p); f[i]=c; insert(pack(f),q); }
      else if(c>=1){ memcpy(f,e,sizeof(int)*p); f[i]=c-1; insert(pack(f),q); }
    }
    // placements
    if(nb<p){
      for(int h=nb;h<p;h++){
        if(h>nb&&e[h]==e[h-1]) continue; // interchangeable equal-charge hand pebbles
        int c=e[h];
        for(int j=-1;j<nb;j++){
          int t=(j<0?0:(e[j]>>4))+1;
          if(t>N||occ[t]) continue;
          memcpy(f,e,sizeof(int)*p); f[h]=(t<<4)|c; insert(pack(f),q);
        }
      }
    }
    for(int i=0;i<nb;i++) occ[e[i]>>4]=false;
  }
  printf("p=%d k=%d N=%d C=%d maxocc=%d states=%zu formula=%d %s\n",p,k,N,best,maxocc,states.size(),(p*k+2)*(1<<(p-2)),maxocc<N?"EXACT":"CAP-BINDING");
  if(argc>4){
    vector<u32> path; for(u32 i=bestidx;i!=0xffffffffu;i=par[i]) path.push_back(i);
    reverse(path.begin(),path.end());
    FILE*o=fopen(argv[4],"w");
    fprintf(o,"# p=%d k=%d clean at n=%d ; moves: P node charge | R node | F node ; %zu moves\n",p,k,best,path.size()-1);
    for(size_t t=1;t<path.size();t++){
      int a[8],b[8]; unpack(states[path[t-1]],a); unpack(states[path[t]],b);
      map<int,int> A,B; for(int i=0;i<p;i++){ if(a[i]>>4)A[a[i]>>4]=a[i]&15; if(b[i]>>4)B[b[i]>>4]=b[i]&15; }
      if(B.size()>A.size()){ for(auto&x:B) if(!A.count(x.first)) fprintf(o,"P %d %d\n",x.first,x.second); }
      else { for(auto&x:A) if(!B.count(x.first)) fprintf(o,"%s %d\n",(x.first==1||A.count(x.first-1))?"R":"F",x.first); }
    }
    fclose(o);
  }
}
