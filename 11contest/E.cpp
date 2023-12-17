#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <vector>

const int kAlph = 256;

const int kG = 25;

const int kR = 26;

const int kT = 10;

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

std::string BWT(std::string str) {
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

  std::string ans;
  for (int i = 0; i < nn; ++i) {
    char ch = str[nn - 1];
    if (pp[i] != 0) {
      ch = str[pp[i] - 1];
    }
    ans += ch;
  }

  return ans;
}

std::string MTF(std::string str) {
  std::list<char> list;
  for (int i = kG; i >= 0; --i) {
    list.push_front(char(int('a') + i));
  }
  std::vector<int> ans;
  for (auto cc : str) {
    int pos = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
      if (*it != cc) {
        ++pos;
        continue;
      }
      ans.push_back(pos);
      list.erase(it);
      list.push_front(cc);
      break;
    }
  }
  std::string ans_s;
  for (auto ii : ans) {
    ans_s += char('a' + ii);
  }
  return ans_s;
}

void AddNum(std::string& ss, int num) {
  std::string s_num;
  if (num == 0) {
    s_num += '0';
  }
  while (num > 0) {
    s_num += char((num % kT) + '0');
    num /= kT;
  }
  std::reverse(s_num.begin(), s_num.end());
  ss += s_num;
}

std::string RLE(std::string ss) {
  std::string ans;
  if (ss.empty()) {
    return ans;
  }
  ans = ss[0];
  char prev_c = ss[0];
  int count = 1;
  for (size_t i = 1; i < ss.size(); ++i) {
    if (ss[i] == prev_c) {
      ++count;
      continue;
    }
    AddNum(ans, count);
    prev_c = ss[i];
    ans += prev_c;
    count = 1;
  }
  AddNum(ans, count);
  return ans;
}

int StrToNUm(std::string ss) {
  int xx = 0;
  for (char cc : ss) {
    xx *= kT;
    xx += (int(cc) - '0');
  }
  return xx;
}

std::string DecodeRle(std::string after_rle) {
  std::string ans;
  for (size_t i = 0; i < after_rle.size(); ++i) {
    char cc = after_rle[i];
    ++i;
    std::string number;
    while (after_rle[i] >= '0' && after_rle[i] <= '9') {
      number += after_rle[i];
      ++i;
    }
    int amount = StrToNUm(number);
    ans += std::string(amount, cc);
    --i;
  }
  return ans;
}

std::string DecodeMTF(std::string after_mtf) {
  std::list<char> list;
  for (int i = kG; i >= 0; --i) {
    list.push_front(char(int('a') + i));
  }
  std::string ans;
  for (auto cc : after_mtf) {
    int pos = cc - 'a';
    auto it = list.begin();
    for (int i = 0; i < pos; ++i) {
      ++it;
    }
    char aa = *it;
    ans.push_back(aa);
    list.erase(it);
    list.push_front(aa);
  }
  return ans;
}

void CountAmount(const std::string& sorted_s, std::vector<int>& first_in,
                 std::vector<int>& alph_amount) {
  int nn = sorted_s.size();
  int count = 1;
  char cur_c = sorted_s[0];
  first_in[cur_c - 'a'] = 0;
  for (int i = 1; i < nn; ++i) {
    if (sorted_s[i] == cur_c) {
      ++count;
      continue;
    }
    alph_amount[cur_c - 'a'] = count;
    cur_c = sorted_s[i];
    first_in[cur_c - 'a'] = i;
    count = 1;
  }
  alph_amount[cur_c - 'a'] = count;
}

std::string DecodeBWT(std::string ss, int kk) {
  int nn = ss.size();
  std::string sorted_s = ss;
  std::sort(sorted_s.begin(), sorted_s.end());
  std::vector<int> tt(nn);
  std::vector<int> alph_amount(kR, 0);
  std::vector<int> first_in(kR, -1);
  CountAmount(sorted_s, first_in, alph_amount);
  for (int i = nn - 1; i >= 0; --i) {
    int ind = first_in[ss[i] - 'a'] + alph_amount[ss[i] - 'a'] - 1;
    --alph_amount[ss[i] - 'a'];
    tt[ind] = i;
  }
  std::string ans;
  for (int i = 0; i < nn; ++i) {
    ans += ss[tt[kk]];
    kk = tt[kk];
  }
  return ans;
}

int main() {
  int mm;
  std::cin >> mm;
  if (mm == 1) {
    std::string str;
    std::cin >> str;
    std::string after_bwt = BWT(str);
    std::string after_mtf = MTF(after_bwt);
    std::cout << RLE(after_mtf) << "\n";
    return 0;
  }
  if (mm == 2) {
    std::string after_rle;
    int kk;
    std::cin >> after_rle >> kk;
    std::string after_mtf = DecodeRle(after_rle);
    std::string after_bwt = DecodeMTF(after_mtf);
    std::string before_bwt = DecodeBWT(after_bwt, kk);
    std::cout << before_bwt << "\n";
    return 0;
  }
}