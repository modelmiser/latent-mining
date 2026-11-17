// Exhaustive reachability check, ORIGINAL coordinates, of the saturated-sum history controller
//     u_t = -clamp( o_t*K + (u_{t-d} + ... + u_{t-1}), -1, +1 ),  o_t = sgn(x_{t-d}),  default K = (d+1)/2.
// Physical+controller state at tick t: x_t, signs of x_{t-d..t-1}, adversary rest flag, last d controls.
// (Only signs of past levels can influence future observations, so this is exact.)
// All adversary moves w in {-1,0,1} (0 only if resting) are explored.
// usage: satcheck d cap [K] [log2 initial table size]     prints sup|x| over reachable states, or FAIL if some |x| > cap.
#include <bits/stdc++.h>
using namespace std;
typedef uint64_t u64;
int d, cap, K;
struct Table { vector<u64> a; u64 n = 0, mask; Table(int lg = 20) : a(1ULL << lg, ~0ULL), mask((1ULL << lg) - 1) {}
  static u64 h(u64 x) { x ^= x >> 33; x *= 0xff51afd7ed558ccdULL; x ^= x >> 33; x *= 0xc4ceb9fe1a85ec53ULL; x ^= x >> 33; return x; }
  bool ins(u64 k) { if (2 * (n + 1) > a.size()) grow(); u64 i = h(k) & mask; while (a[i] != ~0ULL) { if (a[i] == k) return false; i = (i + 1) & mask; } a[i] = k; n++; return true; }
  void grow() { vector<u64> o; o.swap(a); a.assign(o.size() * 2, ~0ULL); mask = a.size() - 1; n = 0; for (u64 k : o) if (k != ~0ULL) { u64 i = h(k) & mask; while (a[i] != ~0ULL) i = (i + 1) & mask; a[i] = k; n++; } } };
// packing: [x+32 : 6][rest : 1][signs : 2d][us : 2d]   (sign/control stored as value+1 in 2 bits, oldest in low bits)
int main(int argc, char** argv) {
  d = atoi(argv[1]); cap = atoi(argv[2]); K = argc > 3 ? atoi(argv[3]) : (d + 1) / 2;
  u64 m2d = (1ULL << (2 * d)) - 1, zero = 0; for (int i = 0; i < d; i++) zero |= 1ULL << (2 * i);
  auto pack = [&](int x, int rest, u64 sg, u64 us) { return ((u64)(x + 32) << (4 * d + 1)) | ((u64)rest << (4 * d)) | (sg << (2 * d)) | us; };
  Table T(argc > 4 ? atoi(argv[4]) : 20); vector<u64> todo; u64 s0 = pack(0, 0, zero, zero); T.ins(s0); todo.push_back(s0); int m = 0;
  while (!todo.empty()) {
    u64 s = todo.back(); todo.pop_back();
    u64 us = s & m2d, sg = (s >> (2 * d)) & m2d; int rest = (s >> (4 * d)) & 1, x = (int)(s >> (4 * d + 1)) - 32;
    int o = (int)(sg & 3) - 1, sum = 0; for (int i = 0; i < d; i++) sum += (int)((us >> (2 * i)) & 3) - 1;
    int v = o * K + sum, u = -(v > 0 ? 1 : (v < 0 ? -1 : 0));
    u64 nus = (us >> 2) | ((u64)(u + 1) << (2 * (d - 1)));
    u64 nsg = (sg >> 2) | ((u64)((x > 0) - (x < 0) + 1) << (2 * (d - 1)));
    for (int w = -1; w <= 1; w++) { if (rest && w) continue; int nx = x + u + w;
      if (abs(nx) > cap) { printf("d=%d K=%d cap=%d FAIL: |x| reaches %d (states so far %llu)\n", d, K, cap, abs(nx), (unsigned long long)T.n); return 1; }
      m = max(m, abs(nx)); u64 k = pack(nx, w != 0, nsg, nus); if (T.ins(k)) todo.push_back(k); } }
  printf("d=%d K=%d cap=%d OK: sup|x|=%d reachable_states=%llu\n", d, K, cap, m, (unsigned long long)T.n); return 0; }
