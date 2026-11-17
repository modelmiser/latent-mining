// Randomised DFS over UNIVERSAL loops of length L = 2^n + e starting 0^n 1 ; tests each for repairability.
// usage: randsearch n L seed maxfound seconds
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 W;
static int N,L; static int NW;
struct Loop{ vector<char> v; };
static bool univ(const vector<char>&v){
    int m=v.size(); static vector<char> seen; seen.assign(1<<N,0); int cnt=0; int c=0; int mask=(1<<N)-1;
    if(m<N) return false;
    for(int j=0;j<N-1;j++) c=((c<<1)|v[j])&mask;
    for(int i=0;i<m;i++){ c=((c<<1)|v[(i+N-1)%m])&mask; if(!seen[c]){seen[c]=1;cnt++;} }
    return cnt==(1<<N);
}
static bool fixable(const vector<char>&v){
    if(univ(v)) return true; int m=v.size(); vector<char> t;
    for(int k=0;k<m;k++){ if(k>0&&v[k]==v[k-1]) continue;
        t=v; t.erase(t.begin()+k); if(univ(t)) return true;
        t=v; t.insert(t.begin()+k,v[k]); if(univ(t)) return true; }
    return false;
}
static bool repairable(const vector<char>&v){
    int m=v.size(); vector<char> t;
    // isolated-run losses first (the only hard moves for universal v), then everything else for completeness
    for(int k=0;k<m;k++){ if(v[k]!=v[(k+1)%m] && v[k]!=v[(k+m-1)%m]){ t=v; t.erase(t.begin()+k); if(!fixable(t)) return false; } }
    if(!fixable(v)) return false;
    for(int k=0;k<m;k++){ if(k>0&&v[k]==v[k-1]) continue;
        t=v; t.erase(t.begin()+k); if(!fixable(t)) return false;
        t=v; t.insert(t.begin()+k,v[k]); if(!fixable(t)) return false; }
    return true;
}
static mt19937_64 rng; static long long nodes=0, leaves=0, found=0, maxfound; static double tlimit; static clock_t t0;
static vector<char> v; static vector<int> cnt; static int MAXREP; static long long budget;
static bool timeup(){ return (double)(clock()-t0)/CLOCKS_PER_SEC > tlimit; }
static void dfs(int rep){
    if(found>=maxfound||budget<=0) return; budget--; nodes++;
    if((nodes&0xfffff)==0 && timeup()){ budget=0; found=maxfound+1; return; }
    int len=v.size();
    if(len==L){ leaves++; if(univ(v) && repairable(v)){ found++; for(char c:v) putchar('0'+c); putchar('\n'); fflush(stdout);} return; }
    int first=rng()&1;
    for(int t=0;t<2;t++){ int b=first^t; int c=b; for(int j=1;j<N;j++) c|=v[len-j]<<j; 
        int r=rep+(cnt[c]?1:0); if(r>MAXREP) continue;
        cnt[c]++; v.push_back(b); dfs(r); v.pop_back(); cnt[c]--; }
}
int main(int argc,char**argv){
    N=atoi(argv[1]); L=atoi(argv[2]); rng.seed(atoll(argv[3])); maxfound=atoll(argv[4]); tlimit=atof(argv[5]); t0=clock();
    MAXREP=L-(1<<N); long long restarts=0;
    while(found<maxfound && !timeup()){
        v.assign(N,0); v.push_back(1); cnt.assign(1<<N,0); cnt[0]=1; cnt[1]=1; budget=200000; restarts++; dfs(0); }
    fprintf(stderr,"n=%d L=%d restarts=%lld nodes=%lld universal-candidate leaves=%lld found=%lld\n",N,L,restarts,nodes,leaves,found);
}
