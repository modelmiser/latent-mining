// back.cpp  usage: back n L [verify]
// FIFO game only. Backward computation of the Adversary attractor A (non-terminal states, all loads <= L-1,
// from which Adversary forces load L). Seeds: states with two bins of load L-1 in different components.
// Closure: (1) p = e+a (e older than everything) with a in A  => p in A   [Adversary expires]
//          (2) s+ball(u<-), s+ball(v<-) both in A or terminal        => s in A   [Adversary offers {u,v}]
// Every age-ordered oriented forest is reachable from empty, so "empty in A" <=> g(n) >= L.
// With "verify": independent forward pass: Rule answers every offer with a state outside A and terminal;
// asserts this is always possible and that expiry never enters A, over the whole forward closure. That pass
// PROVES g(n) < L on its own (A is then only a hint).
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 K; typedef uint64_t u64;
int n,L;
struct St{int k; int r[12],o[12];};
static K packc(const St&s){ int mp[16]; memset(mp,0,sizeof mp); int nx=1; K x=0;
  for(int i=0;i<s.k;i++){ if(!mp[s.r[i]])mp[s.r[i]]=nx++; if(!mp[s.o[i]])mp[s.o[i]]=nx++;
    x|=(K)(mp[s.r[i]]|(mp[s.o[i]]<<4))<<(8*i);} return x;}
static St unpack(K x){St s; s.k=0; while((int)(x&0xff)){s.r[s.k]=(int)(x&15); s.o[s.k]=(int)((x>>4)&15); s.k++; x>>=8;} return s;}
static int maxload(const St&s){int c[16]={0},m=0; for(int i=0;i<s.k;i++){ if(++c[s.r[i]]>m)m=c[s.r[i]]; } return m;}
struct H{ size_t operator()(K x)const{ u64 a=(u64)x^((u64)(x>>64)*0x9e3779b97f4a7c15ULL); a^=a>>33;a*=0xff51afd7ed558ccdULL;a^=a>>33;a*=0xc4ceb9fe1a85ec53ULL;a^=a>>33; return a;}};
unordered_set<K,H> A; vector<K> Q;
static void add(K x){ if(A.insert(x).second) Q.push_back(x); }
static void comps(const St&s,int*c){ for(int i=0;i<16;i++)c[i]=i;
  for(int i=0;i<s.k;i++){int a=c[s.r[i]],b=c[s.o[i]]; if(a!=b) for(int j=0;j<16;j++) if(c[j]==b)c[j]=a;} }
static int used(const St&s){int m=0; for(int i=0;i<s.k;i++) m=max(m,max(s.r[i],s.o[i])); return m;}
u64 nseeds=0;
static void gen(St&s){ // enumerate canonical states with loads <= L-1; record seeds
  int ld[16]={0}; for(int i=0;i<s.k;i++) ld[s.r[i]]++;
  int c[16]; comps(s,c); int m=used(s);
  bool seed=false; for(int a=1;a<=m&&!seed;a++) if(ld[a]==L-1) for(int b=a+1;b<=m;b++) if(ld[b]==L-1&&c[a]!=c[b]){seed=true;break;}
  if(seed){ nseeds++; add(packc(s)); }
  int R=n-1-s.k; if(R<=0) return;
  int t1=0,t2=0; for(int a=1;a<=m;a++){ if(ld[a]>t1){t2=t1;t1=ld[a];} else if(ld[a]>t2)t2=ld[a]; }
  if(!seed && 2*(L-1)-t1-t2>R) return;
  int top=min(n,m+2);
  for(int a=1;a<=top;a++)for(int b=1;b<=top;b++){ if(a==b)continue;
    if(a<=m&&b<=m){ if(c[a]==c[b])continue; }
    else if(a<=m){ if(b!=m+1)continue; } else if(b<=m){ if(a!=m+1)continue; }
    else { if(!(a==m+1&&b==m+2))continue; }
    if(ld[a]+1>L-1) continue;
    s.r[s.k]=a;s.o[s.k]=b;s.k++; gen(s); s.k--; }
}
int main(int argc,char**argv){ n=atoi(argv[1]);L=atoi(argv[2]); bool verify=argc>3;
  St s; s.k=0; gen(s);
  fprintf(stderr,"seeds=%llu\n",(unsigned long long)nseeds);
  bool found=false;
  for(size_t h=0;h<Q.size()&&!found;h++){ St a=unpack(Q[h]);
    if(a.k==0){found=true;break;}
    // (2) offer predecessor
    { St f=a; swap(f.r[f.k-1],f.o[f.k-1]); bool ok = maxload(f)>=L || A.count(packc(f));
      if(ok){ St p=a; p.k--; add(packc(p)); } }
    // (1) expire predecessors
    if(a.k<n-1){ int ld[16]={0}; for(int i=0;i<a.k;i++) ld[a.r[i]]++; int c[16]; comps(a,c); int m=used(a); int top=min(n,m+2);
      St p; p.k=a.k+1; for(int i=0;i<a.k;i++){p.r[i+1]=a.r[i];p.o[i+1]=a.o[i];}
      for(int x=1;x<=top;x++)for(int y=1;y<=top;y++){ if(x==y)continue;
        if(x<=m&&y<=m){ if(c[x]==c[y])continue; }
        else if(x<=m){ if(y!=m+1)continue; } else if(y<=m){ if(x!=m+1)continue; }
        else { if(!(x==m+1&&y==m+2))continue; }
        if(ld[x]+1>L-1) continue;
        p.r[0]=x;p.o[0]=y; add(packc(p)); } }
    if(Q.size()>400000000ULL){ printf("ABORT too big\n"); return 4; }
  }
  if(A.count((K)0)) found=true;
  printf("n=%d L=%d seeds=%llu attractor_nonterminal=%zu%s RESULT: Adversary %s force load %d\n",n,L,(unsigned long long)nseeds,A.size(),found?"(stopped early)":"",found?"CAN":"CANNOT",L);
  if(verify && !found){
    if(n>9){printf("verify supports n<=9\n");return 6;}
    // open-addressing set of u64 keys (n<=9: 8 balls * 8 bits); empty slot = ~0; FIFO queue of slot indices
    u64 cap=(u64)1<<(n==9?28:24), msk=cap-1; vector<u64> T(cap,~0ULL); vector<uint32_t> F; u64 cnt=0;
    auto ins=[&](u64 x)->bool{ u64 h=H()( (K)x)&msk; while(T[h]!=~0ULL){ if(T[h]==x) return false; h=(h+1)&msk;} T[h]=x; F.push_back((uint32_t)h); cnt++; return true; };
    auto has=[&](u64 x)->bool{ u64 h=H()( (K)x)&msk; while(T[h]!=~0ULL){ if(T[h]==x) return true; h=(h+1)&msk;} return false; };
    ins(0);
    for(size_t h=0;h<F.size();h++){ K cur=(K)T[F[h]]; St a=unpack(cur);
      if(maxload(a)>=L||A.count(cur)){printf("VERIFY FAIL (state unsafe)\n");return 5;}
      if(a.k>0){ St p; p.k=a.k-1; for(int i=1;i<a.k;i++){p.r[i-1]=a.r[i];p.o[i-1]=a.o[i];} K x=packc(p);
        if(A.count(x)){printf("VERIFY FAIL (expiry enters A)\n");return 5;} ins((u64)x); }
      int c[16]; comps(a,c);
      for(int u=1;u<=n;u++)for(int v=u+1;v<=n;v++) if(c[u]!=c[v]){   // all n bins explicit, no fresh-bin shortcut
        St t=a; t.r[t.k]=u;t.o[t.k]=v;t.k++; K x1=packc(t); bool ok1=maxload(t)<L&&!A.count(x1);
        swap(t.r[t.k-1],t.o[t.k-1]); K x2=packc(t); bool ok2=maxload(t)<L&&!A.count(x2);
        if(!ok1&&!ok2){printf("VERIFY FAIL (offer unanswerable)\n");return 5;}
        if((ok1&&has((u64)x1))||(ok2&&has((u64)x2))) continue;
        ins((u64)(ok1?x1:x2)); }
      if(cnt*4>cap*3){ printf("VERIFY ABORT too big\n"); return 4; } }
    printf("VERIFIED forward: Rule safe closure of %zu states, never load %d: g(%d) < %d\n",F.size(),L,n,L);
  }
  return 0; }
