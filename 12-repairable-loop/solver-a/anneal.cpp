// Heuristic UPPER-bound search at fixed length L: simulated annealing on
//   cost(w) = 3*(missing words) + #(isolated cells s with w-s not within one edit of universal).
// cost 0  =>  w universal and every isolated loss fixable  =>  repairable (P3). Results re-checked independently.
// usage: anneal n L seed seconds [T0]
#include <bits/stdc++.h>
using namespace std;
static int N;
static int missing(const vector<char>&v){
    int m=v.size(); static vector<char> seen; seen.assign(1<<N,0); int cnt=0; int c=0; int mask=(1<<N)-1;
    for(int j=0;j<N-1;j++) c=((c<<1)|v[j])&mask;
    for(int i=0;i<m;i++){ c=((c<<1)|v[(i+N-1)%m])&mask; if(!seen[c]){seen[c]=1;cnt++;} }
    return (1<<N)-cnt;
}
static bool fixable(const vector<char>&v){
    int mi=missing(v); if(mi==0) return true; if(mi>N-1) return false; int m=v.size(); vector<char> t;
    for(int k=0;k<m;k++){ if(k>0&&v[k]==v[k-1]) continue;
        t=v; t.erase(t.begin()+k); if(missing(t)==0) return true;
        t=v; t.insert(t.begin()+k,v[k]); if(missing(t)==0) return true; }
    return false;
}
static int cost(const vector<char>&v){
    int m=v.size(); int c=3*missing(v); vector<char> t;
    for(int k=0;k<m;k++){ if(v[k]!=v[(k+1)%m] && v[k]!=v[(k+m-1)%m]){ t=v; t.erase(t.begin()+k); if(!fixable(t)) c++; } }
    return c;
}
int main(int argc,char**argv){
    N=atoi(argv[1]); int L=atoi(argv[2]); mt19937_64 rng(atoll(argv[3])); double secs=atof(argv[4]); double T0=argc>5?atof(argv[5]):0.6;
    clock_t t0=clock(); uniform_real_distribution<double> U(0,1);
    vector<char> v(L); for(auto&c:v) c=rng()&1; int cur=cost(v); int best=cur; long long it=0;
    while(cur>0){ double el=(double)(clock()-t0)/CLOCKS_PER_SEC; if(el>secs) break; it++;
        double T=T0*(1-el/secs)+0.05;
        vector<char> t=v; int mv=rng()%3;
        if(mv==0){ t[rng()%L]^=1; }
        else if(mv==1){ int i=rng()%L; char b=t[i]; t.erase(t.begin()+i); t.insert(t.begin()+rng()%L, b); }
        else { int i=rng()%L; t.erase(t.begin()+i); t.insert(t.begin()+rng()%L,(char)(rng()&1)); }
        int c=cost(t); if(c<=cur || U(rng)<exp((cur-c)/T)){ v=t; cur=c; if(cur<best){ best=cur; } }
    }
    if(cur==0){ printf("%d ",L); for(char c:v) putchar('0'+c); putchar('\n'); }
    fprintf(stderr,"n=%d L=%d iterations=%lld best cost=%d final=%d\n",N,L,it,best,cur);
    return cur==0?0:1;
}
