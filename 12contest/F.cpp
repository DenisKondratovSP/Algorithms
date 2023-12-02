#define _USE_MATH_DEFINES
#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

using Complex = std::complex<long double>;

int GetHigher2Deg(int orig) {
  int x_deg2 = 1;
  while (x_deg2 < orig) {
    x_deg2 *= 2;
  }
  return x_deg2;
}

void FFT(int is_forward, const std::vector<Complex>& poly_val,
         std::vector<Complex>& res, int nn) {
  if (nn == 1) {
    res[0] = poly_val[0];
    return;
  }
  std::vector<Complex> ww(nn);
  ww[0] = 1;
  ww[1] = Complex(std::cos(is_forward * (M_PI * 2) / nn),
                  std::sin(is_forward * (M_PI * 2) / nn));
  for (int i = 2; i < nn; ++i) {
    ww[i] = ww[i - 1] * ww[1];
  }
  std::vector<Complex> poly0_val(nn / 2);
  std::vector<Complex> poly0_val_res(nn / 2);
  std::vector<Complex> poly1_val(nn / 2);
  std::vector<Complex> poly1_val_res(nn / 2);
  for (int i = 0; i < nn / 2; ++i) {
    poly0_val[i] = poly_val[i * 2];
    poly1_val[i] = poly_val[i * 2 + 1];
  }
  FFT(is_forward, poly0_val, poly0_val_res, nn / 2);
  FFT(is_forward, poly1_val, poly1_val_res, nn / 2);
  for (int result_size = 0; result_size < nn / 2; ++result_size) {
    res[result_size] = poly0_val_res[result_size] + ww[result_size] * poly1_val_res[result_size];
    res[nn / 2 + result_size] = poly0_val_res[result_size] - ww[result_size] * poly1_val_res[result_size];
  }
}

std::vector<int> MultPolynoms(const std::vector<int>& int_pol1,
                              const std::vector<int>& int_pol2) {
  std::vector<Complex> pol1(int_pol1.size());
  for (size_t i = 0; i < int_pol1.size(); ++i) {
    pol1[i] = int_pol1[i];
  }
  std::vector<Complex> pol2(int_pol2.size());
  for (size_t i = 0; i < int_pol2.size(); ++i) {
    pol2[i] = int_pol2[i];
  }
  int result_size = GetHigher2Deg(int_pol1.size() + int_pol2.size() - 1);
  pol1.resize(result_size);
  pol2.resize(result_size);
  std::vector<Complex> p_res(result_size);
  FFT(1, pol1, p_res, result_size);
  std::vector<Complex> q_res(result_size);
  FFT(1, pol2, q_res, result_size);
  std::vector<Complex> r_res(result_size);
  for (int i = 0; i < result_size; ++i) {
    r_res[i] = p_res[i] * q_res[i];
  }
  std::vector<Complex> pol(result_size);
  FFT(-1, r_res, pol, result_size);
  std::vector<int> product(int_pol1.size() + int_pol2.size() - 1);
  for (int i = product.size() - 1; i >= 0; --i) {
    product[i] = int(std::round(pol[i].real() / result_size));
  }
  return product;
}

int main() {
  int n1;
  std::cin >> n1;
  ++n1;
  std::vector<int> first_pol(n1);
  for (int i = n1 - 1; i >= 0; --i) {
    std::cin >> first_pol[i];
  }
  int m1;
  std::cin >> m1;
  ++m1;
  std::vector<int> second_pol(m1);
  for (int i = m1 - 1; i >= 0; --i) {
    std::cin >> second_pol[i];
  }

  std::vector<int> product = MultPolynoms(first_pol, second_pol);

  std::cout << product.size() - 1 << " ";
  for (int i = static_cast<int>(product.size()) - 1; i >= 0; --i) {
    std::cout << product[i] << " ";
  }
  std::cout << "\n";
}
