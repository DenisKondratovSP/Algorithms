#include <iostream>
#include <string>
#include <vector>

const int kAlph = 256;

std::string GetCycledSubstr(std::string str, size_t pos) {
  std::string new_s = str.substr(pos) + str.substr(0, pos);
  return new_s;
}

int LeftShift(int pos, int kk, int nn) {
  if (pos >= kk) {
    return pos - kk;
  }
  return nn - kk + pos;
}

int RightShift(int pos, int kk, int nn) {
  if (nn - 1 - pos >= kk) {
    return pos + kk;
  }
  return kk - nn + pos;
}

void UpdateC(const std::vector<int>& pp, int nn, std::vector<int>& cc, int kk) {
  int cur_class = 0;
  std::vector<int> new_c(nn);
  new_c[pp[0]] = 0;
  for (int i = 1; i < nn; ++i) {
    if (cc[pp[i]] != cc[pp[i - 1]] ||
        cc[RightShift(pp[i], kk, nn)] != cc[RightShift(pp[i - 1], kk, nn)]) {
      ++cur_class;
    }
    new_c[pp[i]] = cur_class;
  }
  cc = new_c;
}

int main() {
  std::string str;
  std::cin >> str;
  int nn = str.size();
  std::vector<int> pp(nn);
  std::vector<int> cnt(kAlph, 0);

  for (int i = 0; i < nn; ++i) {
    ++cnt[str[i]];
  }
  for (int i = 1; i < kAlph; ++i) {
    cnt[i] += cnt[i - 1];
  }
  for (int i = nn - 1; i >= 0; --i) {
    pp[--cnt[str[i]]] = i;
  }

  int cur_class = 0;
  std::vector<int> cc(nn);
  cc[pp[0]] = 0;
  for (int i = 1; i < nn; ++i) {
    if (str[pp[i]] != str[pp[i - 1]]) {
      ++cur_class;
    }
    cc[pp[i]] = cur_class;
  }

  int kk = 1;
  while (kk < nn) {
    std::vector<int> p_new(nn);
    for (int i = 0; i < nn; ++i) {
      p_new[i] = LeftShift(pp[i], kk, nn);
    }

    cnt = std::vector<int>(nn, 0);
    for (int i = 0; i < nn; ++i) {
      ++cnt[cc[p_new[i]]];
    }
    for (int i = 1; i < nn; ++i) {
      cnt[i] += cnt[i - 1];
    }
    for (int i = nn - 1; i >= 0; --i) {
      pp[--cnt[cc[p_new[i]]]] = p_new[i];
    }

    UpdateC(pp, nn, cc, kk);

    kk *= 2;
  }

  for (int i = 0; i < nn; ++i) {
    char pos = str[nn - 1];
    if (pp[i] != 0) {
      pos = str[pp[i] - 1];
    }
    std::cout << pos;
  }
}