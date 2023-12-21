#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

long long FastPow(int root_exp, int pow, int mod) {
  if (pow == 1) {
    return root_exp;
  }
  if (pow % 2 == 0) {
    long long x_k = FastPow(root_exp, pow / 2, mod);
    return (x_k * x_k) % mod;
  }
  return (root_exp * FastPow(root_exp, pow - 1, mod)) % mod;
}

void GetDecomp(int xx, int& mm, int& ss) {
  while (xx % 2 == 0) {
    ++ss;
    xx /= 2;
  }
  mm = xx;
}

int GetQuadraticNonResidue(int mod) {
  int bb = std::rand() % mod;
  while (FastPow(bb, (mod - 1) / 2, mod) != mod - 1) {
    bb = std::rand() % mod;
  }
  return bb;
}

int GetBinOrd(long long u_i, int mod) {
  int kk = 0;
  while (u_i != 1) {
    u_i *= u_i;
    u_i %= mod;
    ++kk;
  }
  return kk;
}

int TwoPowTo(int deg) {
  int xx = 1;
  for (int i = 0; i < deg; ++i) {
    xx *= 2;
  }
  return xx;
}

int DiscRoot(int root_exp, int mod) {
  if (mod == 1) {
    return 0;
  }
  if (mod == 2 || root_exp == 0) {
    return root_exp;
  }
  if (FastPow(root_exp, (mod - 1) / 2, mod) != 1) {
    return -1;
  }
  if (mod % 4 == 3) {
    return std::min(FastPow(root_exp, (mod + 1) / 4, mod),
                    FastPow(mod - root_exp, (mod + 1) / 4, mod));
  }
  int mm = 1;
  int ss = 0;
  GetDecomp(mod - 1, mm, ss);
  int u_i = FastPow(root_exp, mm, mod);
  int v_i = FastPow(root_exp, (mm + 1) / 2, mod);
  while (u_i > 1) {
    int bb = GetQuadraticNonResidue(mod);
    int r_i = GetBinOrd(u_i, mod);
    int cc = FastPow(bb, mm, mod);
    u_i = (u_i * FastPow(cc, TwoPowTo(ss - r_i), mod)) % mod;
    v_i = (v_i * FastPow(cc, TwoPowTo(ss - r_i - 1), mod)) % mod;
  }
  return std::min(v_i, mod - v_i);
}

int main() {
  std::srand(std::time(0));
  int test_amount;
  std::cin >> test_amount;
  for (int i = 0; i < test_amount; ++i) {
    int root_exp;
    int mod;
    std::cin >> root_exp >> mod;
    int ans = DiscRoot(root_exp, mod);
    if (ans == -1) {
      std::cout << "IMPOSSIBLE\n";
      continue;
    }
    std::cout << ans << "\n";
  }
}
