// Exact G(n) for all n <= NMAX by BFS restricted to states with both registers <= NMAX.
// Safe: registers never decrease, and a register exceeding the target n can never contribute to reaching n
// (any step that pushes a register above n, and all later steps on that register, can be deleted from a shortest path;
//  the other register's later steps then must all be doublings, which do not read it).
// usage: gbound K  (NMAX=2^K, K<=15). Output: per-t counts, G table file g_K.txt (n G), stats.
#include <bits/stdc++.h>
using namespace std;
int main(int c,char**v){int K=atoi(v[1]); size_t NM=(size_t)1<<K, W=NM+1; vector<uint8_t> d(W*W,255); vector<int> G(W,-1);
 d[W+1]=0; G[1]=0; bool any=true; int t=0;
 while(any){any=false;
  for(size_t a=1;a<=NM;a++) for(size_t b=a;b<=NM;b++) if(d[a*W+b]==t){
    size_t x[4]={2*a,a,a+b,a},y[4]={b,2*b,b,a+b};
    for(int k=0;k<4;k++){size_t p=x[k],q=y[k]; if(p>q)swap(p,q); if(q>NM)continue; uint8_t&r=d[p*W+q]; if(r==255){r=t+1;any=true; if(G[p]<0)G[p]=t+1; if(G[q]<0)G[q]=t+1;}}}
  t++;}
 vector<long> cnt(64,0); for(size_t n=1;n<=NM;n++){ if(G[n]<0){printf("UNREACHED %zu\n",n);return 1;} cnt[G[n]]++; }
 long cum=0; for(int i=0;i<64;i++) if(cnt[i]){cum+=cnt[i]; printf("cost %d count_n<=2^%d %ld cum %ld\n",i,K,cnt[i],cum);}
 FILE*f=fopen((string("g_")+v[1]+".txt").c_str(),"w"); for(size_t n=1;n<=NM;n++)fprintf(f,"%zu %d\n",n,G[n]); fclose(f);
 // n with G(n)=ceil(log2 n)
 printf("G=ceil(log2 n):"); for(size_t n=1;n<=NM;n++){int cl=0; while(((size_t)1<<cl)<n)cl++; if(G[n]==cl)printf(" %zu",n);} puts("");
 // per binary length: max and mean of G
 for(int L=0;L<K;L++){int mx=0;double s=0;size_t am=0; for(size_t n=(size_t)1<<L;n<((size_t)2<<L);n++){s+=G[n]; if(G[n]>mx){mx=G[n];am=n;}} printf("len %d maxG %d (first at %zu) meanG %.4f mean/L %.4f\n",L,mx,am,s/((size_t)1<<L),L?s/((size_t)1<<L)/L:0.0);}
}
