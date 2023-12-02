#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

int CalcAPowK(int base, int m_sqrt, int mod) {
  long long result = base;
  for (int i = 1; i < m_sqrt; ++i) {
    result *= base;
    result %= mod;
  }
  return result;
}

std::unordered_map<int, int> CalcAPowAllK(int m_sqrt, int base, int mod) {
  std::unordered_map<int, int> aki;
  aki.reserve(m_sqrt + 1);
  int a_pow_k = CalcAPowK(base, m_sqrt, mod);
  long long cur_a_pow_ki = 1;
  for (int i = 1; i <= m_sqrt; ++i) {
    cur_a_pow_ki *= a_pow_k;
    cur_a_pow_ki %= mod;
    aki.insert(std::pair<int, int>(cur_a_pow_ki, m_sqrt * i));
  }
  aki.insert(std::pair<int, int>(1, mod - 1));
  return aki;
}

int main() {
  int mod;
  int base;
  int degree;
  while (std::cin >> mod >> base >> degree) {
    if (degree == 1) {
      std::cout << 0 << "\n";
      continue;
    }
    int m_sqrt = std::sqrt(mod);
    std::unordered_map<int, int> aki = CalcAPowAllK(m_sqrt, base, mod);
    long long ba_j = degree;
    int ans = mod;
    for (int j = 0; j < m_sqrt; ++j) {
      if (aki.contains(ba_j)) {
        int ki = aki[ba_j];
        ans = std::min(ans, ki - j) % (mod - 1);
      }
      ba_j *= base;
      ba_j %= mod;
    }
    if (ans == mod) {
      std::cout << "no solution\n";
      continue;
    }
    std::cout << ans << "\n";
  }
}
