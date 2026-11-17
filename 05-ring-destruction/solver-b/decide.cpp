// Decision solver: survive(T) = adversary can keep every process incomplete through round T.
// F(n,k) = 1 + max{T : survive(T)}.
// State in extents: K_t(i) = {i-L_i..i+R_i}; link A_i = (i+1 -> i) feeds R_i; link B_i = (i-1 -> i) feeds L_i.
// Prunings (all safe; see proofs.txt section P):
//  (1) only links whose destruction changes the next state are considered ("useful");
//  (2) prevD restricted to useful links;  (3) dihedral canonical form for the memo;
//  (4) half-speed bound: with rho rounds left, R_i(T) >= max_{0<=j<=rho/2} R_{i+j}+j (same for L);
//      if some i then has R+L >= n-1 the adversary has already lost. Disable with prune=0.
// Memo: failed (state,prev) -> smallest rho known to fail (fail is monotone in rho).
// usage: decide n k [prune=1] [Tmax]
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 u128; typedef unsigned long long u64;
int n,k,PR=1;
struct St{ int8_t L[13],R[13]; uint32_t prev; }; // prev bits: i -> A_i ; n+i -> B_i
struct Tab{ vector<u128> a; size_t cnt=0,mask;
  Tab(){a.assign(1<<20,0);mask=a.size()-1;}
  static u64 h(u128 x){ u64 y=(u64)x^((u64)(x>>64)*0x9E3779B97F4A7C15ULL); y^=y>>31;y*=0xBF58476D1CE4E5B9ULL;y^=y>>29;y*=0x94D049BB133111EBULL;y^=y>>32;return y;}
  // entry = key<<8 | val ; key nonzero guaranteed by adding marker bit
  int get(u128 key){ size_t p=h(key)&mask; while(a[p]){ if((a[p]>>8)==key) return (int)(a[p]&255); p=(p+1)&mask;} return -1;}
  void put(u128 key,int v){ if(cnt*10>a.size()*6) grow(); size_t p=h(key)&mask; while(a[p]){ if((a[p]>>8)==key){a[p]=(key<<8)|v;return;} p=(p+1)&mask;} a[p]=(key<<8)|v; cnt++; }
  void grow(){ vector<u128> o; o.swap(a); a.assign(o.size()*2,0); mask=a.size()-1; cnt=0; for(u128 e:o) if(e){ size_t p=h(e>>8)&mask; while(a[p]) p=(p+1)&mask; a[p]=e; cnt++; } }
} memo;
u128 pack(const St&s){ u128 x=1; for(int i=0;i<n;i++){ x=(x<<4)|s.L[i]; x=(x<<4)|s.R[i]; } x=(x<<(2*n))|s.prev; return x; }
u128 canon(const St&s){
  u128 best=0; bool first=true;
  for(int ref=0;ref<2;ref++) for(int sh=0;sh<n;sh++){
    St b; b.prev=0;
    for(int i=0;i<n;i++){
      int j= ref? ((n-i)%n+sh)%n : (i+sh)%n;
      if(ref){ b.L[j]=s.R[i]; b.R[j]=s.L[i]; if(s.prev>>i&1) b.prev|=1u<<(n+j); if(s.prev>>(n+i)&1) b.prev|=1u<<j; }
      else { b.L[j]=s.L[i]; b.R[j]=s.R[i]; if(s.prev>>i&1) b.prev|=1u<<j; if(s.prev>>(n+i)&1) b.prev|=1u<<(n+j); }
    }
    u128 p=pack(b); if(first||p<best){best=p;first=false;}
  }
  return best;
}
u64 nodes=0;
vector<uint32_t> path;
bool surv(St s,int rho){
  if(rho==0) return true;
  // useful links
  uint32_t um=0;
  for(int i=0;i<n;i++){ if(s.R[(i+1)%n]+1>s.R[i]) um|=1u<<i; if(s.L[(i+n-1)%n]+1>s.L[i]) um|=1u<<(n+i); }
  s.prev&=um;
  if(PR){
    for(int i=0;i<n;i++){ int r=s.R[i],l=s.L[i];
      for(int j=1;j<=rho/2;j++){ r=max(r,s.R[(i+j)%n]+j); l=max(l,s.L[((i-j)%n+n)%n]+j); }
      if(r+l>=n-1) return false; }
  }
  u128 key=canon(s);
  int m=memo.get(key); if(m>=0 && m<=rho) return false;
  nodes++;
  int al[26],na=0; for(int l=0;l<2*n;l++) if((um>>l&1)&&!(s.prev>>l&1)) al[na++]=l;
  bool ok=false;
  function<void(int,uint32_t,int)> rec=[&](int idx,uint32_t D,int cnt){
    if(ok) return;
    if(idx==na){
      St t; t.prev=D; bool comp=false;
      for(int i=0;i<n;i++){
        t.R[i]= (D>>i&1)? s.R[i] : max<int>(s.R[i],s.R[(i+1)%n]+1);
        t.L[i]= (D>>(n+i)&1)? s.L[i] : max<int>(s.L[i],s.L[(i+n-1)%n]+1);
        if(t.R[i]+t.L[i]>=n-1) comp=true;
      }
      if(comp) return;
      path.push_back(D);
      if(surv(t,rho-1)){ok=true;return;}
      path.pop_back();
      return;
    }
    if(cnt<k) rec(idx+1,D|(1u<<al[idx]),cnt+1);
    rec(idx+1,D,cnt);
  };
  rec(0,0,0);
  if(!ok) memo.put(key,rho);
  return ok;
}
int main(int argc,char**argv){
  n=atoi(argv[1]);k=atoi(argv[2]); if(argc>3) PR=atoi(argv[3]); int Tmax=argc>4?atoi(argv[4]):2*(n/2);
  St s; for(int i=0;i<n;i++){s.L[i]=0;s.R[i]=0;} s.prev=0;
  vector<uint32_t> best; int T;
  for(T=1;T<=Tmax;T++){ path.clear(); if(surv(s,T)) best=path; else break; }
  // T = least T with no survival (or Tmax+1). surviving T-1 rounds => F = T (if T<=Tmax)
  printf("%d %d F=%d%s nodes=%llu memo=%zu\n",n,k,T,T>Tmax?" (>=, cut)":"",(unsigned long long)nodes,memo.cnt);
  char fn[64]; sprintf(fn,"surv_n%d_k%d.txt",n,k); FILE*f=fopen(fn,"w");
  fprintf(f,"# n=%d k=%d: %d rounds with no process complete (so F>=%d). line: t then destroyed links src>dst\n",n,k,(int)best.size(),(int)best.size()+1);
  for(size_t t=0;t<best.size();t++){ fprintf(f,"%zu",t+1); for(int i=0;i<n;i++){ if(best[t]>>i&1) fprintf(f," %d>%d",(i+1)%n,i); if(best[t]>>(n+i)&1) fprintf(f," %d>%d",(i+n-1)%n,i);} fprintf(f,"\n"); }
  fclose(f);
}
