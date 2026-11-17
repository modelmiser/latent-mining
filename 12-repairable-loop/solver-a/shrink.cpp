// Heuristic UPPER-bound search: start from a de Bruijn cycle written twice (repairable by P1), then
// random length-reducing / length-neutral local moves, keeping the loop universal and repairable.
// For universal loops repairable <=> every isolated-cell loss is fixable (P3); final answers are
// re-checked with the full definition by check_witness.py / dfs-style checker.
// usage: shrink n seed seconds
#include <bits/stdc++.h>
using namespace std;
static int N;
static bool univ(const vector<char>&v){
    int m=v.size(); if(m<N) return false; static vector<char> seen; seen.assign(1<<N,0); int cnt=0; int c=0; int mask=(1<<N)-1;
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
static bool rep_univ(const vector<char>&v){ // v universal assumed
    int m=v.size(); vector<char> t;
    for(int k=0;k<m;k++){ if(v[k]!=v[(k+1)%m] && v[k]!=v[(k+m-1)%m]){ t=v; t.erase(t.begin()+k); if(!fixable(t)) return false; } }
    return true;
}
int main(int argc,char**argv){
    N=atoi(argv[1]); mt19937_64 rng(atoll(argv[2])); double secs=atof(argv[3]); clock_t t0=clock();
    // de Bruijn cycle by random-order DFS (Hamiltonian path on n-words)
    int M=1<<N; vector<char> d; {
        vector<char> used(M,0); vector<int> path; 
        function<bool(int)> go=[&](int w){ used[w]=1; path.push_back(w); if((int)path.size()==M){ if((((w<<1)&(M-1))>>1)==(path[0]>>1) ) { int nx0=((w<<1)&(M-1)), nx1=nx0|1; if(nx0==path[0]||nx1==path[0]) return true; } }
            else { int f=rng()&1; for(int t=0;t<2;t++){ int nx=((w<<1)&(M-1))|(f^t); if(!used[nx] && go(nx)) return true; } }
            used[w]=0; path.pop_back(); return false; };
        go(0); for(int w:path) d.push_back(w&1); }
    vector<char> v=d; v.insert(v.end(),d.begin(),d.end());
    if(!univ(v)||!rep_univ(v)){ fprintf(stderr,"start not repairable?!\n"); return 1; }
    size_t best=v.size(); long long it=0;
    while((double)(clock()-t0)/CLOCKS_PER_SEC<secs){ it++;
        vector<char> t=v; int m=t.size(); int mv=rng()%4;
        if(mv<=1){ t.erase(t.begin()+rng()%m); }                                   // shrink by one
        else if(mv==2){ t.erase(t.begin()+rng()%m); int j=rng()%(m-1); t.insert(t.begin()+j,(char)(rng()&1)); } // neutral: move/replace
        else { int j=rng()%m; t[j]^=1; }                                           // neutral: flip
        if(!univ(t)||!rep_univ(t)) continue;
        v=t; if(v.size()<best){ best=v.size(); printf("%zu ",best); for(char c:v) putchar('0'+c); putchar('\n'); fflush(stdout); }
    }
    fprintf(stderr,"n=%d iterations=%lld best=%zu\n",N,it,best);
}
