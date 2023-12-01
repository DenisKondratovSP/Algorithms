#include <iostream>
#include <queue>
#include <string>
#include <vector>

const int kAlph = 26;

using String = std::string;

struct Trie {
  struct Node {
    std::vector<int> to;
    bool term;
    std::vector<int> str;
    Node() : to(kAlph, -1), term(false) {}
  };

  std::vector<Node> tt;

  Trie(const std::vector<String>& dict) : tt(1, Node()) {
    for (size_t i = 0; i < dict.size(); ++i) {
      Add(dict[i], i);
    }
  }

  void Add(const String& str, size_t num) {
    int vv = 0;
    for (size_t i = 0; i < str.size(); ++i) {
      if (tt[vv].to[str[i] - 'a'] == -1) {
        tt[vv].to[str[i] - 'a'] = tt.size();
        tt.push_back(Node());
      }
      vv = tt[vv].to[str[i] - 'a'];
    }
    tt[vv].term = true;
    tt[vv].str.push_back(num);
  }

  size_t Size() const { return tt.size(); }
};

struct AhoCorasick {
  Trie bor;
  std::vector<int> link;
  std::vector<std::vector<int>> go;
  std::vector<int> dp;
  std::vector<int> compressed;

  AhoCorasick(const std::vector<String>& dict)
      : bor(dict),
        link(bor.Size()),
        go(bor.Size(), std::vector<int>(kAlph)),
        dp(bor.Size()),
        compressed(bor.Size()) {
    InitRoot();

    std::queue<int> qq;
    qq.push(0);
    while (!qq.empty()) {
      int vv = qq.front();
      qq.pop();
      for (size_t cc = 0; cc < kAlph; ++cc) {
        int uu = bor.tt[vv].to[cc];
        if (uu == -1) {
          continue;
        }

        if (vv == 0) {
          link[uu] = 0;
        } else {
          link[uu] = go[link[vv]][cc];
        }

        for (size_t dd = 0; dd < kAlph; ++dd) {
          if (bor.tt[uu].to[dd] != -1) {
            go[uu][dd] = bor.tt[uu].to[dd];
          } else {
            go[uu][dd] = go[link[uu]][dd];
          }
        }

        dp[uu] = dp[link[uu]];
        if (bor.tt[uu].term) {
          ++dp[uu];
        }

        if (bor.tt[link[uu]].term) {
          compressed[uu] = link[uu];
        } else {
          compressed[uu] = compressed[link[uu]];
        }

        qq.push(uu);
      }
    }
  }

  void InitRoot() {
    link[0] = -1;
    for (size_t cc = 0; cc < kAlph; ++cc) {
      if (bor.tt[0].to[cc] == -1) {
        go[0][cc] = 0;
      } else {
        go[0][cc] = bor.tt[0].to[cc];
      }
    }
    dp[0] = 0;
    compressed[0] = 0;
  }
};

void FindInclusion(const std::vector<std::string>& dict,
                   const std::vector<std::string>& text, int len,
                   std::vector<std::vector<int>>& ans) {
  AhoCorasick ac_str(dict);
  int nn = text.size();
  int mm = text[0].size();
  for (int i = 0; i < nn; ++i) {
    int vv = 0;
    for (int j = 0; j < mm; ++j) {
      vv = ac_str.go[vv][text[i][j] - 'a'];
      if (!ac_str.bor.tt[vv].term) {
        continue;
      }
      for (auto str_ind : ac_str.bor.tt[vv].str) {
        if (i - str_ind >= 0) {
          ++ans[i - str_ind][j - len + 1];
        }
      }
    }
  }
}

int main() {
  int nn;
  int mm;
  std::cin >> nn >> mm;
  std::vector<std::string> text_str(nn, std::string(mm, '0'));
  std::vector<std::vector<int>> ans_str(nn, std::vector<int>(mm, 0));
  for (int i = 0; i < nn; ++i) {
    for (int j = 0; j < mm; ++j) {
      std::cin >> text_str[i][j];
    }
  }

  int aa;
  int bb;
  std::cin >> aa >> bb;
  std::vector<std::string> mipt_str(aa, std::string(bb, '0'));
  for (int i = 0; i < aa; ++i) {
    for (int j = 0; j < bb; ++j) {
      std::cin >> mipt_str[i][j];
    }
  }

  FindInclusion(mipt_str, text_str, bb, ans_str);

  std::vector<std::string> text_row(mm, std::string(nn, '0'));
  std::vector<std::vector<int>> ans_row(mm, std::vector<int>(nn, 0));
  for (int i = 0; i < mm; ++i) {
    for (int j = 0; j < nn; ++j) {
      text_row[i][j] = text_str[j][i];
    }
  }
  std::vector<std::string> mipt_row(bb, std::string(aa, '0'));
  for (int i = 0; i < bb; ++i) {
    for (int j = 0; j < aa; ++j) {
      mipt_row[i][j] = mipt_str[j][i];
    }
  }

  FindInclusion(mipt_row, text_row, aa, ans_row);

  int ans = 0;
  for (int i = 0; i <= nn - aa; ++i) {
    for (int j = 0; j <= mm - bb; ++j) {
      if ((ans_row[j][i] == bb && ans_str[i][j] == aa) ||
          (ans_row[j][i] == bb - 1 && ans_str[i][j] == aa - 1)) {
        ++ans;
      }
    }
  }
  std::cout << ans;
}