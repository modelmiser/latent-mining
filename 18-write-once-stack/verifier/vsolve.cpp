// Referee solver, written from the problem statement (not from the solvers' code).
// usage: vsolve n t MODE init [out] [nodelimit] [prune]
//   MODE L: Q(start) unchecked; S: Q(start)=top(init).  init: '-' or bit string (bottom first).
//   prune: 1 (default) = pairwise-compatibility + alternating-push weight bound + fresh-cell symmetry; 0 = none of these.
// Method: backtracking over the partial function E (and Q), maintaining for each configuration the set of
// "situations" (stack suffix of length <= r+1, with a bottom flag via its length; r = operations left) it hosts.
// A situation's future depends only on that suffix: r ops expose at most r+1 entries.
#include <bits/stdc++.h>
using namespace std;
int n,t; bool modeS; int prune=1;
struct Sit{int len; unsigned bits; int r;}; // bits: bit i = i-th from top
vector<Sit> sits; map<tuple<int,unsigned,int>,int> sid;
vector<array<int,3>> succ; vector<int> topv; // topv: 0,1, 2=EMPTY
int startSit;
int getsit(int len,unsigned bits,int r){ // truncate to r+1
    if(len>r+1){len=r+1;} bits&=(1u<<len)-1;
    auto k=make_tuple(len,bits,r); auto it=sid.find(k); if(it!=sid.end())return it->second;
    int id=sits.size(); sid[k]=id; sits.push_back({len,bits,r}); succ.push_back({-1,-1,-1}); topv.push_back(len?(bits&1):2);
    if(r>=1){
        int a=getsit(len+1,(bits<<1)|0,r-1); succ[id][0]=a;
        int b=getsit(len+1,(bits<<1)|1,r-1); succ[id][1]=b;
        if(len>=1){int c=getsit(len-1,bits>>1,r-1); succ[id][2]=c;}
    }
    return id;
}
vector<vector<signed char>> compm;
bool comp(int x,int y){
    if(x==y)return true; if(x>y)swap(x,y);
    signed char &m=compm[x][y]; if(m>=0)return m;
    bool ok=true;
    bool xfree=(!modeS&&x==startSit), yfree=(!modeS&&y==startSit);
    if(!xfree&&!yfree&&topv[x]!=topv[y])ok=false;
    for(int op=0;op<3&&ok;op++) if(succ[x][op]>=0&&succ[y][op]>=0) ok=comp(succ[x][op],succ[y][op]);
    return m=ok;
}
int NC; vector<int> Qv; vector<array<int,3>> Ev; vector<vector<char>> host; vector<vector<int>> hostl;
struct Tr{int kind,a,b;}; vector<Tr> trail; vector<pair<int,int>> agenda; size_t apos=0;
long long nodes=0,limit=-1; bool aborted=false;
int need(int x){ // cells a non-free situation still needs: alternating pushes change the top r times
    return sits[x].r;
}
bool add(int c,int x){
    if(host[c][x])return true;
    bool freeq=(!modeS&&x==startSit);
    if(!freeq){
        if(Qv[c]<0){Qv[c]=topv[x];trail.push_back({0,c,0});}
        else if(Qv[c]!=topv[x])return false;
    }
    if(prune){
        if(!freeq && n-__builtin_popcount(c)<need(x))return false;
        for(int y:hostl[c]) if(!comp(x,y))return false;
    }
    host[c][x]=1;hostl[c].push_back(x);trail.push_back({1,c,x});
    for(int op=0;op<3;op++) if(succ[x][op]>=0){
        if(Ev[c][op]>=0){ if(!add(Ev[c][op],succ[x][op]))return false; }
        else agenda.push_back({c,op});
    }
    return true;
}
void undo(size_t ts,size_t as,size_t ap){
    while(trail.size()>ts){Tr u=trail.back();trail.pop_back();
        if(u.kind==0)Qv[u.a]=-1; else if(u.kind==1){host[u.a][u.b]=0;hostl[u.a].pop_back();} else Ev[u.a][u.b]=-1;}
    agenda.resize(as);apos=ap;
}
int usedmask=0;
bool search(){
    if(aborted)return false;
    while(apos<agenda.size()&&Ev[agenda[apos].first][agenda[apos].second]>=0)apos++;
    if(apos==agenda.size())return true;
    if(limit>=0&&++nodes>limit){aborted=true;return false;}
    int c=agenda[apos].first,op=agenda[apos].second;
    size_t ts=trail.size(),as=agenda.size(),ap=apos;
    int freem=(NC-1)&~c;
    // used cells: any cell set in a config that hosts something or is an E target
    int used=0; for(int d=0;d<NC;d++) if(!hostl[d].empty())used|=d;
    vector<int> cands;
    for(int s=freem;;s=(s-1)&freem){
        bool ok=true;
        if(prune){ // fresh (never used) cells are interchangeable: take them lowest-index first
            int fresh=s&~used; int unusedAll=(NC-1)&~used;
            int k=__builtin_popcount(fresh); int pref=0,cnt=0;
            for(int i=0;i<n&&cnt<k;i++) if(unusedAll>>i&1){pref|=1<<i;cnt++;}
            if(fresh!=pref)ok=false;
        }
        if(ok)cands.push_back(c|s);
        if(s==0)break;
    }
    sort(cands.begin(),cands.end(),[&](int a,int b){int pa=__builtin_popcount(a),pb=__builtin_popcount(b); if(pa!=pb)return pa<pb; return a<b;});
    for(int c2:cands){
        Ev[c][op]=c2;trail.push_back({2,c,op});
        bool ok=true;
        vector<int> hs=hostl[c]; // copy: hostl[c] can grow if c2==c
        for(size_t i=0;i<hs.size()&&ok;i++){int y=hs[i]; if(succ[y][op]>=0) ok=add(c2,succ[y][op]);}
        // if c2==c newly added sits at c were propagated inside add (E already set)
        if(ok&&search())return true;
        undo(ts,as,ap);
        if(aborted)return false;
    }
    return false;
}
int main(int argc,char**argv){
    n=atoi(argv[1]);t=atoi(argv[2]);modeS=argv[3][0]=='S';string init=argv[4];if(init=="-")init="";
    string out=argc>5?argv[5]:"";if(argc>6)limit=atoll(argv[6]);if(argc>7)prune=atoi(argv[7]);
    unsigned bits=0;int len=init.size();for(int i=0;i<len;i++)if(init[len-1-i]=='1')bits|=1u<<i;
    if(len>t+1){len=t+1;}
    startSit=getsit(len,bits,t);
    int S=sits.size();compm.assign(S,vector<signed char>(S,-1));
    NC=1<<n;Qv.assign(NC,-1);Ev.assign(NC,{-1,-1,-1});host.assign(NC,vector<char>(S,0));hostl.assign(NC,{});
    bool res=add(0,startSit)&&search();
    printf("n=%d t=%d mode=%c init=%s : %s nodes=%lld\n",n,t,modeS?'S':'L',init.empty()?"-":init.c_str(),aborted?"ABORTED":(res?"SURVIVES":"IMPOSSIBLE"),nodes);
    if(res&&!out.empty()){
        FILE*f=fopen(out.c_str(),"w");fprintf(f,"# referee witness\nn %d\nt %d\ninit %s\n",n,t,init.empty()?"-":init.c_str());
        auto cs=[&](int c){string s;for(int i=0;i<n;i++)s+=(c>>i&1)?'1':'0';return s;};
        const char*qn[]={"0","1","EMPTY"};const char*on[]={"PUSH0","PUSH1","POP"};
        for(int c=0;c<NC;c++){ if(Qv[c]>=0)fprintf(f,"Q %s %s\n",cs(c).c_str(),qn[Qv[c]]); else if(!hostl[c].empty())fprintf(f,"Q %s 0\n",cs(c).c_str());
            for(int op=0;op<3;op++)if(Ev[c][op]>=0)fprintf(f,"E %s %s %s\n",cs(c).c_str(),on[op],cs(Ev[c][op]).c_str());}
        fclose(f);
    }
    return aborted?2:(res?0:1);
}
