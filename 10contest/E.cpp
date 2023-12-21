#include <iostream>
#include <string>
#include <vector>

const int kAlph = 10;
const int k10x6 = 1'000'000;

using String = std::string;

struct Trie {
  struct Node {
    std::vector<int> to;
    bool term;
    size_t subtree_term_am;
    size_t depth;
    Node(int depth = 0)
        : to(kAlph, -1), term(false), subtree_term_am(0), depth(depth) {}
  };

  std::vector<Node> tt;

  Trie(const std::vector<String>& dict) : tt(1, Node()) {
    for (size_t i = 0; i < dict.size(); ++i) {
      Add(dict[i]);
    }
  }

  void Add(const String& str) {
    int vv = 0;
    ++tt[vv].subtree_term_am;
    for (size_t i = 0; i < str.size(); ++i) {
      if (tt[vv].to[str[i] - '0'] == -1) {
        tt[vv].to[str[i] - '0'] = tt.size();
        tt.push_back(Node(tt[vv].depth + 1));
      }
      vv = tt[vv].to[str[i] - '0'];
      ++tt[vv].subtree_term_am;
    }
    tt[vv].term = true;
  }

  size_t Size() const { return tt.size(); }
};

int main() {
  int nn;
  size_t kk;
  std::cin >> nn >> kk;
  std::vector<String> ss(nn);
  for (int i = 0; i < nn; ++i) {
    std::cin >> ss[i];
  }

  int qq;
  std::cin >> qq;
  std::vector<int> ll(qq);
  for (int i = 0; i < qq; ++i) {
    std::cin >> ll[i];
  }

  std::vector<String> double_s(nn);
  for (int i = 0; i < nn; ++i) {
    double_s[i] = String(2 * ss[i].size(), '0');
    for (size_t j = 0; j < ss[i].size(); ++j) {
      double_s[i][2 * j] = ss[i][j];
    }
    for (int j = ss[i].size() - 1; j >= 0; --j) {
      double_s[i][1 + 2 * (ss[i].size() - 1 - j)] = ss[i][j];
    }
  }
  Trie trie(double_s);

  std::vector<int> ans(k10x6, 0);

  for (size_t i = 0; i < trie.tt.size(); ++i) {
    if (trie.tt[i].depth % 2 == 0 && trie.tt[i].subtree_term_am >= kk) {
      ans[trie.tt[i].depth / 2] += 1;
    }
  }

  for (int i = 0; i < qq; ++i) {
    std::cout << ans[ll[i]] << "\n";
  }
}