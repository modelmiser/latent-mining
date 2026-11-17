// game.cpp - exact solver for the switch-cost online lateness game.
// usage: game P Tmax L promise(S|F) [onlinecost(1|0)] [maxrel(-1=unlimited)] [witness T]
// Decides for each T=1..Tmax whether the adversary can force lateness >= L+1.
// Model simplification (proved in proofs.txt): idle is never better than a switch tick,
// so a tick is either "run a job" or "empty"; running B != A (A unfinished) is illegal
// only directly after a run tick (which was A's).  State keeps lock = job run last tick if unfinished.
// All deadlines are stored RELATIVE to the current tick. Online deadlines are stored as d+L.
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int curMode=0, SCHEDMODE=0; int LENS=~0; int SLACKMAX=-1; int P,TMAX,L; bool SAME; int ONCOST=1; int MAXREL=-1;
// ---- machine states (lock byte + sorted job bytes; byte = d*8+rem) ----
unordered_map<string,int> sid; vector<string> sstr; vector<signed char> soloMemo;
int intern(const string&s){auto it=sid.find(s); if(it!=sid.end())return it->second; int id=sstr.size(); sid.emplace(s,id); sstr.push_back(s); soloMemo.push_back(-1); return id;}
inline int B(int rem,int d){return d*8+rem;}
int addJob(int id,int p,int d){ string s=sstr[id]; unsigned char b=B(p,d); size_t i=1; while(i<s.size() && (unsigned char)s[i]<b) i++; s.insert(s.begin()+i,(char)b); return intern(s);} 
// successors after one tick (deadlines shifted by -1). cost: switch cost of this machine (1 or 0)
void succ(int id,int cost,vector<int>&out){
  out.clear(); const string s=sstr[id]; int n=s.size()-1; unsigned char lock=s[0];
  // check shiftable: every job needs d>=1 ; after shift d-1 >= rem' required for liveness but we let solo decide; d-1>=0 always since d>=1
  auto shifted=[&](int runIdx)->int{ // runIdx -1 = empty
    string t; t.reserve(s.size()); t.push_back(0); unsigned char newlock=0;
    for(int i=1;i<=n;i++){ unsigned char b=s[i]; int rem=b&7,d=b>>3; if(i==runIdx){rem--; } d--; if(rem==0) continue; if(d<rem) return -1; unsigned char nb=B(rem,d); if(i==runIdx && cost) newlock=nb; t.push_back(nb);} 
    sort(t.begin()+1,t.end()); t[0]=newlock; return intern(t); };
  // curMode restricts the ONLINE scheduler only (set by schedLoses): 0 all moves; 1 'noidle': no empty tick while unlocked with work pending;
  // 2 'np': never an empty tick with work pending (so never switches); 3 'npedf': np + earliest deadline (ties: fewer units left) when free.
  bool allowEmpty = curMode==0 || n==0 || (curMode==1 && lock);
  if(allowEmpty){ int e=shifted(-1); if(e>=0) out.push_back(e); }
  unsigned char prev=0;
  for(int i=1;i<=n;i++){ unsigned char b=s[i]; if(b==prev) continue; prev=b; if(lock && b!=lock) continue; int x=shifted(i); if(x>=0) out.push_back(x); if(curMode==3) break; } 
}
bool solo(int id,int cost){ // can this machine finish everything by deadlines with no more releases
  // memo only valid for one cost per id: online ids and hindsight ids may coincide as strings; with cost 0 no lock ever -> but same string could be queried with both costs.
  // we therefore keep separate memo for cost 0.
  static vector<signed char> memo0;
  vector<signed char>&M = cost? soloMemo: memo0; if(!cost && memo0.size()<sstr.size()) memo0.resize(sstr.size()*2+16,-1);
  if(M[id]>=0) return M[id];
  const string s=sstr[id]; if(s.size()==1){M[id]=1;return true;}
  for(size_t i=1;i<s.size();i++){unsigned char b=s[i]; if((b>>3)<(b&7)){M[id]=0;return false;}}
  vector<int> o; succ(id,cost,o); bool r=false; for(int x:o){ if(solo(x,cost)){r=true;break;} }
  if(!cost && memo0.size()<sstr.size()) memo0.resize(sstr.size()*2+16,-1);
  (cost? soloMemo: memo0)[id]=r; return r; }
// ---- dominance between hindsight states ----
// X dominates Y (X at least as good for every future) iff there is an injection m from X's jobs into Y's jobs
// with rem_x <= rem_m(x) and d_x >= d_m(x), and (X unlocked, or X locked on x0, Y locked on y0, m(x0)=y0).
// Proof in proofs.txt (Lemma D). Greedy matching (X by increasing d, best-fit smallest sufficient rem) is exact.
bool matchInto(vector<pair<int,int>> xs, vector<pair<int,int>> ys){ // pairs (d,rem)
  sort(xs.begin(),xs.end()); vector<char> used(ys.size(),0);
  for(auto&x:xs){ int best=-1; for(size_t j=0;j<ys.size();j++){ if(used[j]) continue; if(ys[j].first<=x.first && ys[j].second>=x.second){ if(best<0 || ys[j].second<ys[best].second) best=j; } } if(best<0) return false; used[best]=1; }
  return true; }
bool OLDDOM=false;
bool dominates(const string&X,const string&Y){
  unsigned char lx=X[0], ly=Y[0]; vector<pair<int,int>> xs,ys;
  for(size_t i=1;i<X.size();i++){unsigned char b=X[i]; xs.push_back({b>>3,b&7});} for(size_t i=1;i<Y.size();i++){unsigned char b=Y[i]; ys.push_back({b>>3,b&7});}
  if(xs.size()>ys.size()) return false;
  if(lx){ if(!ly) return false; if((ly>>3)>(lx>>3) || (ly&7)<(lx&7)) return false; xs.erase(find(xs.begin(),xs.end(),make_pair((int)(lx>>3),(int)(lx&7)))); ys.erase(find(ys.begin(),ys.end(),make_pair((int)(ly>>3),(int)(ly&7)))); }
  if(OLDDOM){ for(auto&x:xs){ auto it=find(ys.begin(),ys.end(),x); if(it==ys.end()) return false; ys.erase(it);} return true; }
  return matchInto(xs,ys); }
// ---- hindsight: same-cost => set of states ; free => single EDF profile ----
struct VH{size_t operator()(const vector<int>&v)const{size_t h=1469598103934665603ULL; for(int x:v){h^=x; h*=1099511628211ULL;} return h;}};
unordered_map<vector<int>,int,VH> hsid; vector<vector<int>> hsets; unordered_map<u64,int> haddMemo; vector<int> hadvMemo;
bool NODOM=false;
int hintern(vector<int>&v){ sort(v.begin(),v.end()); v.erase(unique(v.begin(),v.end()),v.end());
  if(!NODOM){ vector<int> keep; for(size_t i=0;i<v.size();i++){ bool dom=false; for(size_t j=0;j<v.size()&&!dom;j++){ if(i==j) continue; if(dominates(sstr[v[j]],sstr[v[i]])){ // strict or tie-break by index
        if(dominates(sstr[v[i]],sstr[v[j]])){ if(j<i) dom=true; } else dom=true; } } if(!dom) keep.push_back(v[i]); } v.swap(keep);} 
  auto it=hsid.find(v); if(it!=hsid.end()) return it->second; int id=hsets.size(); hsid.emplace(v,id); hsets.push_back(v); hadvMemo.push_back(-2); return id; }
// free: profile string h[1..]
unordered_map<string,int> fsid; vector<string> fstr;
int fintern(string s){ while(!s.empty()&&s.back()==0) s.pop_back(); auto it=fsid.find(s); if(it!=fsid.end())return it->second; int id=fstr.size(); fsid.emplace(s,id); fstr.push_back(s); hadvMemo.push_back(-2); return id; }
int hempty(){ if(SAME){ vector<int> v{intern(string(1,'\0'))}; return hintern(v);} return fintern(""); }
int hadd(int hid,int p,int d){ u64 key=((u64)hid<<10)|(p<<6)|d; auto it=haddMemo.find(key); if(it!=haddMemo.end()) return it->second; int r;
  if(SAME){ vector<int> v; for(int x:hsets[hid]){ int y=addJob(x,p,d); if(solo(y,1)) v.push_back(y);} r= v.empty()? -1: hintern(v);} 
  else { string s=fstr[hid]; if((int)s.size()<d) s.resize(d,0); s[d-1]+=p; int sum=0; bool ok=true; for(size_t i=0;i<s.size();i++){ sum+=s[i]; if(sum>(int)i+1){ok=false;break;} } r= ok? fintern(s): -1; }
  haddMemo[key]=r; return r; }
int hadv(int hid){ if(hadvMemo[hid]!=-2) return hadvMemo[hid]; int r;
  if(SAME){ vector<int> v,o; for(int x:hsets[hid]){ succ(x,1,o); for(int y:o) if(solo(y,1)) v.push_back(y);} r=hintern(v); /* nonempty since alive */ }
  else { string s=fstr[hid]; for(size_t i=0;i<s.size();i++) if(s[i]){s[i]--;break;} if(!s.empty()) s.erase(s.begin()); r=fintern(s);} 
  hadvMemo[hid]=r; return r; }
// ---- game ----
unordered_map<u64,char> preMemo, postMemo; u64 nodes=0;
int TOTCAP=-1; int totUsed=0; bool FIRSTWIN=false;
inline u64 K(int H,int h,int o){ return ((u64)(H*16+(TOTCAP>=0?totUsed:0))<<54)|((u64)h<<27)|(u64)o; }
bool advWin(int H,int hid,int oid);
bool schedLoses(int H,int hid,int oid){ u64 k=K(H,hid,oid); auto it=postMemo.find(k); if(it!=postMemo.end()) return it->second; nodes++;
  vector<int> o; curMode=SCHEDMODE; succ(oid,ONCOST,o); curMode=0; int h2=hadv(hid); bool lose=true; for(int x:o){ if(!solo(x,ONCOST)) continue; if(!advWin(H-1,h2,x)){lose=false;break;} }
  postMemo[k]=lose; return lose; }
bool relRec(int H,int hid,int oid,int idx,int cnt){
  if(schedLoses(H,hid,oid)) return true;
  if(MAXREL>=0 && cnt>=MAXREL) return false; if(TOTCAP>=0 && totUsed>=TOTCAP) return false;
  int ntypes=0; for(int d=1;d<=H;d++) for(int p=1;p<=P&&p<=d;p++){ int ti=ntypes++; if(ti<idx) continue; if(!(LENS>>p&1)) continue; if(SLACKMAX>=0 && d-p>SLACKMAX && d!=H) continue; int h2=hadd(hid,p,d); if(h2<0) continue; int o2=addJob(oid,p,d+L); if(!solo(o2,ONCOST)) return true; totUsed++; bool rr=relRec(H,h2,o2,ti,cnt+1); totUsed--; if(rr) return true; }
  return false; }
bool advWin(int H,int hid,int oid){ if(H==0) return false; u64 k=K(H,hid,oid); auto it=preMemo.find(k); if(it!=preMemo.end()) return it->second; bool r=relRec(H,hid,oid,0,0); preMemo[k]=r; return r; }
// ---- witness printing: adversary strategy tree ----
string show(int oid,int t,int shiftL){ const string&s=sstr[oid]; string r="{"; for(size_t i=1;i<s.size();i++){ unsigned char b=s[i]; r+="("+to_string(b&7)+",d"+to_string((b>>3)+t-shiftL)+")"; } r+="}"; if(s[0]) r+=" lock("+to_string(s[0]&7)+",d"+to_string((s[0]>>3)+t-shiftL)+")"; return r; }
bool findRel(int H,int hid,int oid,int idx,int cnt,vector<pair<int,int>>&rel,int&hout,int&oout){
  if(!solo(oid,ONCOST) || schedLoses(H,hid,oid)){hout=hid;oout=oid;return true;}
  if(MAXREL>=0 && cnt>=MAXREL) return false; if(TOTCAP>=0 && totUsed>=TOTCAP) return false;
  int ntypes=0; for(int d=1;d<=H;d++) for(int p=1;p<=P&&p<=d;p++){ int ti=ntypes++; if(ti<idx) continue; if(!(LENS>>p&1)) continue; if(SLACKMAX>=0 && d-p>SLACKMAX && d!=H) continue; int h2=hadd(hid,p,d); if(h2<0) continue; int o2=addJob(oid,p,d+L); rel.push_back({p,d}); totUsed++; bool rr=findRel(H,h2,o2,ti,cnt+1,rel,hout,oout); totUsed--; if(rr) return true; rel.pop_back(); }
  return false; }
void witness(int T,int H,int hid,int oid,int depth){ int t=T-H; string ind(depth*2,' ');
  vector<pair<int,int>> rel; int h2,o2; if(!findRel(H,hid,oid,0,0,rel,h2,o2)){ printf("%s?? not winning\n",ind.c_str()); return; }
  printf("%st=%d release:",ind.c_str(),t); if(rel.empty()) printf(" nothing"); for(auto&pr:rel) printf(" (r%d,p%d,d%d)",t,pr.first,pr.second+t); printf("  online now %s\n",show(o2,t,L).c_str());
  if(!solo(o2,ONCOST)){ printf("%s  => online cannot finish all within lateness %d: adversary wins\n",ind.c_str(),L); return; }
  const string s=sstr[o2]; vector<int> o; succ(o2,ONCOST,o); int hn=hadv(h2);
  // label moves: recompute like succ order: empty first, then distinct runnable
  vector<string> labels; labels.push_back("empty"); { unsigned char prev=0,lock=s[0]; for(size_t i=1;i<s.size();i++){unsigned char b=s[i]; if(b==prev)continue; prev=b; if(lock&&b!=lock)continue; labels.push_back("run("+to_string(b&7)+",d"+to_string((b>>3)+t-L)+")"); } }
  // succ may drop dead ones; recompute alignment by redoing succ logic with labels
  // simple approach: enumerate again manually
  {
    int n=s.size()-1; unsigned char lock=s[0]; vector<int> cand; cand.push_back(-1); unsigned char prev=0; for(int i=1;i<=n;i++){unsigned char b=s[i]; if(b==prev)continue; prev=b; if(lock&&b!=lock)continue; cand.push_back(i);} 
    for(size_t c=0;c<cand.size();c++){ int runIdx=cand[c]; string tt; tt.push_back(0); unsigned char newlock=0; bool dead=false; for(int i=1;i<=n;i++){ unsigned char b=s[i]; int rem=b&7,d=b>>3; if(i==runIdx)rem--; d--; if(rem==0)continue; if(d<rem){dead=true;break;} unsigned char nb=B(rem,d); if(i==runIdx&&ONCOST)newlock=nb; tt.push_back(nb);} 
      if(dead){ printf("%s  online %s -> a job can no longer make d+%d: adversary wins\n",ind.c_str(),labels[c].c_str(),L); continue; }
      sort(tt.begin()+1,tt.end()); tt[0]=newlock; int x=intern(tt); if(!solo(x,ONCOST)){ printf("%s  online %s -> state %s cannot be finished within lateness %d: adversary wins\n",ind.c_str(),labels[c].c_str(),show(x,t+1,L).c_str(),L); continue; }
      printf("%s  online %s:\n",ind.c_str(),labels[c].c_str()); witness(T,H-1,hn,x,depth+2); }
  }
}

// ---- minimal-size adversary strategy tree (min number of nodes) + export ----
unordered_map<u64,long long> mtMemo; const long long MINF=(long long)1e15;
struct Opt{ vector<pair<int,int>> rel; int h,o; };
struct TU{int save; TU(int add){save=totUsed; totUsed+=add;} ~TU(){totUsed=save;}};
void enumRel(int H,int hid,int oid,int idx,int cnt,vector<pair<int,int>>&cur,vector<Opt>&out){
  out.push_back({cur,hid,oid}); if(!solo(oid,ONCOST)) return; if(MAXREL>=0&&cnt>=MAXREL) return; if(TOTCAP>=0&&totUsed>=TOTCAP) return;
  int ntypes=0; for(int d=1;d<=H;d++) for(int p=1;p<=P&&p<=d;p++){ int ti=ntypes++; if(ti<idx) continue; if(!(LENS>>p&1)) continue; if(SLACKMAX>=0 && d-p>SLACKMAX && d!=H) continue; int h2=hadd(hid,p,d); if(h2<0) continue; int o2=addJob(oid,p,d+L); cur.push_back({p,d}); totUsed++; enumRel(H,h2,o2,ti,cnt+1,cur,out); totUsed--; cur.pop_back(); } }
long long minTree(int H,int hid,int oid);
long long optCost(int H,const Opt&op){ TU tu((int)op.rel.size()); if(!solo(op.o,ONCOST)) return 1; if(!schedLoses(H,op.h,op.o)) return MINF; vector<int> o; succ(op.o,ONCOST,o); int hn=hadv(op.h); long long c=1; for(int x:o){ if(!solo(x,ONCOST)) continue; long long m=minTree(H-1,hn,x); if(m>=MINF) return MINF; c+=m; } return c; }
long long minTree(int H,int hid,int oid){ if(H==0||!advWin(H,hid,oid)) return MINF; u64 k=K(H,hid,oid); auto it=mtMemo.find(k); if(it!=mtMemo.end()) return it->second;
  vector<Opt> opts; vector<pair<int,int>> cur; enumRel(H,hid,oid,0,0,cur,opts); long long best=MINF; for(auto&op:opts){ long long c=optCost(H,op); if(c<best) best=c; } mtMemo[k]=best; return best; }
void exportTree(int T,int H,int hid,int oid,int depth,FILE*hf,FILE*jf){ int t=T-H; string ind(depth*2,' ');
  vector<Opt> opts; vector<pair<int,int>> cur; enumRel(H,hid,oid,0,0,cur,opts); long long best=MINF; const Opt*bo=nullptr; for(auto&op:opts){ long long c; if(FIRSTWIN){ TU tu((int)op.rel.size()); c=(!solo(op.o,ONCOST)||schedLoses(H,op.h,op.o))?1:MINF; } else c=optCost(H,op); if(c<best){best=c;bo=&op; if(FIRSTWIN) break;} }
  Opt op=*bo; fprintf(hf,"%st=%d release:",ind.c_str(),t); if(op.rel.empty()) fprintf(hf," nothing"); for(auto&pr:op.rel) fprintf(hf," (r%d,p%d,d%d)",t,pr.first,pr.second+t); fprintf(hf,"  -> online has %s",show(op.o,t,L).c_str());
  fprintf(jf,"{\"rel\":["); for(size_t i=0;i<op.rel.size();i++) fprintf(jf,"%s[%d,%d]",i?",":"",op.rel[i].first,op.rel[i].second+t); fprintf(jf,"],\"moves\":{");
  if(!solo(op.o,ONCOST)){ fprintf(hf,"  ADV WINS (cannot finish within lateness %d)\n",L); fprintf(jf,"}}"); return; }
  fprintf(hf,"\n"); TU tu((int)op.rel.size()); const string s=sstr[op.o]; int n=s.size()-1; unsigned char lock=s[0]; vector<int> cand; cand.push_back(-1); unsigned char prev=0; for(int i=1;i<=n;i++){unsigned char b=s[i]; if(b==prev)continue; prev=b; if(lock&&b!=lock)continue; cand.push_back(i);} 
  int hn=hadv(op.h); bool first=true; string dead;
  for(size_t c=0;c<cand.size();c++){ int runIdx=cand[c]; string lab= runIdx<0? "e": "r"+to_string(s[runIdx]&7)+","+to_string((((unsigned char)s[runIdx])>>3)+t-L);
    string tt; tt.push_back(0); unsigned char newlock=0; bool isdead=false; for(int i=1;i<=n;i++){ unsigned char b=s[i]; int rem=b&7,d=b>>3; if(i==runIdx)rem--; d--; if(rem==0)continue; if(d<rem){isdead=true;break;} unsigned char nb=B(rem,d); if(i==runIdx&&ONCOST)newlock=nb; tt.push_back(nb);} 
    int x=-1; if(!isdead){ sort(tt.begin()+1,tt.end()); tt[0]=newlock; x=intern(tt); if(!solo(x,ONCOST)) isdead=true; }
    if(isdead){ dead+=" "+lab; continue; }
    fprintf(hf,"%s  online %s:\n",ind.c_str(),lab.c_str()); fprintf(jf,"%s\"%s\":",first?"":",",lab.c_str()); first=false; exportTree(T,H-1,hn,x,depth+2,hf,jf); }
  if(!dead.empty()) fprintf(hf,"%s  online%s: ADV WINS at once (lateness %d no longer reachable)\n",ind.c_str(),dead.c_str(),L);
  fprintf(jf,"}}"); }
int main(int argc,char**argv){ if(argc<5){fprintf(stderr,"usage\n");return 1;} P=atoi(argv[1]);TMAX=atoi(argv[2]);L=atoi(argv[3]);SAME=argv[4][0]=='S'; if(argc>5)ONCOST=atoi(argv[5]); if(argc>6)MAXREL=atoi(argv[6]); int WT= argc>7? atoi(argv[7]):0; if(getenv("NODOM")) NODOM=true; if(getenv("SCHED")) SCHEDMODE=atoi(getenv("SCHED")); if(getenv("FIRSTWIN")) FIRSTWIN=true; if(getenv("SLACKMAX")) SLACKMAX=atoi(getenv("SLACKMAX")); if(getenv("TOTCAP")) TOTCAP=atoi(getenv("TOTCAP")); if(getenv("LENS")){ LENS=0; for(const char*c=getenv("LENS");*c;c++) if(*c>='1'&&*c<='7') LENS|=1<<(*c-'0'); } if(getenv("OLDDOM")) OLDDOM=true;
  int e=intern(string(1,'\0')); int h0=hempty();
  if(WT && argc>8){ if(!advWin(WT,h0,e)){printf("adversary does not win at T=%d\n",WT);return 0;} long long c= FIRSTWIN? -1: minTree(WT,h0,e); string base=argv[8]; FILE*hf=fopen((base+".txt").c_str(),"w"); FILE*jf=fopen((base+".json").c_str(),"w"); fprintf(hf,"# minimal-size adversary strategy forcing lateness >= %d. P=%d T=%d promise=%s maxrel=%d nodes=%lld\n# deadlines shown are TRUE deadlines; 'e' = empty tick (idle or switch); rX,Y = run one unit of a job with X units left and deadline Y\n",L+1,P,WT,argv[4],MAXREL,c); fprintf(jf,"{\"P\":%d,\"T\":%d,\"L\":%d,\"promise\":\"%s\",\"tree\":",P,WT,L,argv[4]); exportTree(WT,WT,h0,e,0,hf,jf); fprintf(jf,"}\n"); fclose(hf); fclose(jf); printf("tree nodes=%lld written %s.txt/.json\n",c,base.c_str()); return 0; }
  if(WT){ if(!advWin(WT,h0,e)){printf("adversary does not win at T=%d\n",WT);return 0;} printf("# adversary strategy forcing lateness >= %d, P=%d T=%d promise=%s (deadlines shown are the TRUE deadlines d)\n",L+1,P,WT,argv[4]); witness(WT,WT,h0,e,0); return 0; }
  for(int T=1;T<=TMAX;T++){ auto t0=chrono::steady_clock::now(); bool w=advWin(T,h0,e); double sec=chrono::duration<double>(chrono::steady_clock::now()-t0).count();
    printf("P=%d T=%d L=%d promise=%s oncost=%d maxrel=%d : adversary forces lateness>=%d ? %s   [postnodes=%llu states=%zu hsets=%zu %.1fs]\n",P,T,L,argv[4],ONCOST,MAXREL,L+1,w?"YES":"no",nodes,sstr.size(),SAME?hsets.size():fstr.size(),sec); fflush(stdout);} }
