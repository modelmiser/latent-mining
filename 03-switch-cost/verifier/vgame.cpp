// Verifier's own solver, written from the game definition (not from the solvers' code).
// Threshold game: can the adversary force max lateness >= L+1 ?
// Plain machine: run / idle / switch are separate actions; state = (prev-tick-was-switch flag,
// current job A if unfinished, multiset of unfinished jobs (d,rem)).
// Promise: exact set ("frontier") of all hindsight machine states at the start of tick t from
// which every released job can still be completed on time.
// usage: vgame P T L mode(S|F) [feastest N seed]
#include <bits/stdc++.h>
using namespace std;
typedef string St;            // [0]=flag, [1]=lock byte (0 = none), [2..]=sorted job bytes d*8+rem
static int P, T, L; static bool SAME;
static inline int JD(unsigned char b){return b>>3;} static inline int JR(unsigned char b){return b&7;}
static inline unsigned char JB(int d,int r){return (unsigned char)(d*8+r);}

// successors of machine state s for one tick; cost = switch cost (1 or 0)
static void succ(const St& s, int cost, vector<St>& out){
    out.clear();
    unsigned char lock = s[1]; bool flag = s[0];
    // idle
    { St n = s; n[0]=0; out.push_back(n); }
    // switch tick (cost-1 machine only). With no current unfinished job the flag is irrelevant: canonical 0.
    if (cost) { St n = s; n[0] = lock ? 1 : 0; out.push_back(n); }
    unsigned char prev = 0;
    for (size_t i=2;i<s.size();i++){
        unsigned char b = s[i]; if (b==prev) continue; prev=b;
        if (cost && lock && b!=lock && !flag) continue;   // needs a switch tick immediately before
        St n = s; n[0]=0;
        int r = JR(b)-1;
        if (r==0){ n.erase(i,1); n[1] = 0; if (!cost) n[1]=0; }
        else { n[i] = JB(JD(b), r); n[1] = cost ? n[i] : 0; sort(n.begin()+2, n.end()); }
        // if b!=lock the old lock job stays in the multiset untouched; if b==lock it was the one run.
        out.push_back(n);
    }
    sort(out.begin(), out.end()); out.erase(unique(out.begin(), out.end()), out.end());
}
static St addjob(const St& s, int d, int p){ St n=s; n.push_back(JB(d,p)); sort(n.begin()+2,n.end()); return n; }
static bool doomed(const St& s, int t, int slack){ for(size_t i=2;i<s.size();i++) if (t+JR(s[i]) > JD(s[i])+slack) return true; return false; }

// ---- hindsight liveness
static unordered_map<St,int> hsId; static vector<St> hsStr;
static int HS(const St& s){ auto it=hsId.find(s); if(it!=hsId.end()) return it->second; int id=hsStr.size(); hsId.emplace(s,id); hsStr.push_back(s); return id; }
static unordered_map<uint64_t,char> liveMemo;
static bool live(int t, int id){
    const St s = hsStr[id];
    if (s.size()==2) return true;
    if (doomed(s,t,0)) return false;
    uint64_t k = ((uint64_t)id<<6)|t; auto it=liveMemo.find(k); if(it!=liveMemo.end()) return it->second;
    vector<St> ss; succ(s, SAME?1:0, ss); bool r=false;
    for (auto& n: ss){ if (live(t+1, HS(n))) { r=true; break; } }
    liveMemo[k]=r; return r;
}
// ---- frontier interning
struct VH{ size_t operator()(const vector<int>& v) const { uint64_t h=1469598103934665603ULL; for(int x: v){ h^=(uint64_t)x; h*=1099511628211ULL; h^=h>>29;} return h; } };
static unordered_map<vector<int>,int,VH> frId; static vector<vector<int>> frVec;
static int FR(const vector<int>& v){ auto it=frId.find(v); if(it!=frId.end()) return it->second; int id=frVec.size(); frId.emplace(v,id); frVec.push_back(v); return id; }
static vector<int> frAdd(const vector<int>& F, int t, int d, int p){
    vector<int> o; for(int id: F){ int n=HS(addjob(hsStr[id],d,p)); if (live(t,n)) o.push_back(n); }
    sort(o.begin(),o.end()); o.erase(unique(o.begin(),o.end()),o.end()); return o;
}
static vector<int> frStep(const vector<int>& F, int t){
    vector<int> o; vector<St> ss;
    for(int id: F){ succ(hsStr[id], SAME?1:0, ss); for(auto& n: ss){ int k=HS(n); if(live(t+1,k)) o.push_back(k);} }
    sort(o.begin(),o.end()); o.erase(unique(o.begin(),o.end()),o.end()); return o;
}
// ---- online states
static unordered_map<St,int> onId; static vector<St> onStr;
static int ON(const St& s){ auto it=onId.find(s); if(it!=onId.end()) return it->second; int id=onStr.size(); onId.emplace(s,id); onStr.push_back(s); return id; }

struct KH{ size_t operator()(const array<int,3>& a) const { uint64_t h=a[0]*0x9E3779B97F4A7C15ULL; h^=(uint64_t)a[1]*0xC2B2AE3D27D4EB4FULL; h^=h>>31; h+=(uint64_t)a[2]*0x165667B19E3779F9ULL; h^=h>>29; return h; } };
static unordered_map<array<int,3>,char,KH> memo;
static unsigned long long nodes=0, relsTried=0;

static bool advWins(int t, int on, int fr);
// after release: adversary wins iff every online action leads to an adversary win
static bool afterRelease(int t, const St& os, const vector<int>& F){
    relsTried++;
    vector<St> ss; succ(os, 1, ss);
    int nf = -1;
    for (auto& n: ss){
        if (doomed(n, t+1, L)) continue;           // this action already loses for the scheduler
        if (n.size()==2 && t+1>=T) return false;   // everything done, no more releases
        if (nf<0) nf = (t+1<T) ? FR(frStep(F,t)) : 0;
        if (!advWins(t+1, ON(n), nf)) return false;
    }
    return true;
}
static bool enumRel(int t, const St& os, const vector<int>& F, int kmin){
    if (afterRelease(t, os, F)) return true;
    int k=0;
    for (int p=1;p<=P;p++) for (int d=t+p; d<=T; d++, k++){
        if (k<kmin) continue;
        vector<int> F2 = frAdd(F,t,d,p);
        if (F2.empty()) continue;                 // promise broken (and stays broken for supersets)
        if (enumRel(t, addjob(os,d,p), F2, k)) return true;
    }
    return false;
}
static bool advWins(int t, int on, int fr){
    array<int,3> key={t,on,fr}; auto it=memo.find(key); if(it!=memo.end()) return it->second;
    nodes++;
    bool r;
    const St os = onStr[on];
    if (t>=T) { static const vector<int> none; r = afterRelease(t, os, none); }
    else { const vector<int> F = frVec[fr]; r = enumRel(t, os, F, 0); }
    memo[key]=r; return r;
}
// feasibility of a full job list via the frontier machinery (for testing against brute force)
static bool feasList(const vector<array<int,3>>& jobs){
    vector<int> F; F.push_back(HS(St(2,'\0')));
    int H=0; for(auto&j: jobs) H=max(H,j[2]);
    for (int t=0;t<H;t++){
        for(auto&j: jobs) if (j[0]==t){ F=frAdd(F,t,j[2],j[1]); if(F.empty()) return false; }
        F=frStep(F,t);
        if (F.empty()) return false;
    }
    return true;
}
int main(int argc,char**argv){
    P=atoi(argv[1]); T=atoi(argv[2]); L=atoi(argv[3]); SAME=(argv[4][0]=='S');
    if (argc>5 && string(argv[5])=="feaslist"){
        // read lines: n r p d r p d ... ; print 0/1 per line
        string line; while(getline(cin,line)){ istringstream is(line); int n; if(!(is>>n)) continue; vector<array<int,3>> jobs(n); for(auto&j: jobs) is>>j[0]>>j[1]>>j[2]; printf("%d\n", feasList(jobs)?1:0); }
        return 0;
    }
    St e(2,'\0'); vector<int> F0; F0.push_back(HS(e));
    bool r = advWins(0, ON(e), FR(F0));
    printf("P=%d T=%d %s: adversary forces lateness >= %d ? %s   [nodes=%llu rel=%llu hs=%zu fr=%zu on=%zu]\n", P,T,SAME?"S":"F",L+1, r?"YES":"NO", nodes, relsTried, hsStr.size(), frVec.size(), onStr.size());
    fflush(stdout);
    return 0;
}
