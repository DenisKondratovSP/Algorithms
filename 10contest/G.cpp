#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

const int kAlph = 26;

using String = std::string;

struct Trie {
  struct Node {
    std::vector<int> to;
    bool term;
    int str_num;
    Node() : to(kAlph, -1), term(false) {}
  };

  std::vector<Node> tt;

  Trie(const std::vector<String>& dict) : tt(1, Node()) {
    for (size_t i = 0; i < dict.size(); ++i) {
      Add(dict[i], i);
    }
  }

  void Add(const String& str, int str_num) {
    int vv = 0;
    for (size_t i = 0; i < str.size(); ++i) {
      if (tt[vv].to[str[i] - 'a'] == -1) {
        tt[vv].to[str[i] - 'a'] = tt.size();
        tt.push_back(Node());
      }
      vv = tt[vv].to[str[i] - 'a'];
    }
    tt[vv].term = true;
    tt[vv].str_num = str_num;
  }

  int Find(const std::string& str) const {
    int vv = 0;
    for (size_t i = 0; i < str.size(); ++i) {
      if (tt[vv].to[str[i] - 'a'] == -1) {
        return -1;
      }
      vv = tt[vv].to[str[i] - 'a'];
    }
    if (tt[vv].term) {
      return tt[vv].str_num;
    }
    return -1;
  }

  size_t Size() const { return tt.size(); }
};

bool IsPalindrome(std::string ss) {
  int nn = ss.size();
  int half_n = nn / 2;
  for (int i = 0; i < half_n; ++i) {
    if (ss[i] != ss[nn - 1 - i]) {
      return false;
    }
  }
  return true;
}

bool FindPair(std::string mid, std::string tail, const Trie& trie,
              std::vector<std::pair<int, int>>& ans, int str_num) {
  if (!IsPalindrome(mid)) {
    return false;
  }
  std::reverse(tail.begin(), tail.end());
  int tail_str_num = trie.Find(tail);
  if (tail_str_num != -1 && tail_str_num != str_num) {
    ans.push_back(std::pair(str_num, tail_str_num));
    return true;
  }
  return false;
}

void FindPalindromes(std::string cur_s, const Trie& trie,
                     std::vector<std::pair<int, int>>& ans, int str_num) {
  std::string rev_s = cur_s;
  std::reverse(rev_s.begin(), rev_s.end());
  int rev_s_num = trie.Find(rev_s);
  if (rev_s_num != -1 && rev_s_num != str_num) {
    ans.push_back(std::pair(str_num, rev_s_num));
  }
  for (size_t i = 1; i < cur_s.size(); ++i) {
    std::string mid = cur_s.substr(i);
    std::string left = cur_s.substr(0, i);
    FindPair(mid, left, trie, ans, str_num);
  }
  for (int i = cur_s.size() - 1; i >= 1; --i) {
    std::string mid = cur_s.substr(0, i);
    std::string right = cur_s.substr(i);
    if (FindPair(mid, right, trie, ans, str_num)) {
      std::swap(ans[ans.size() - 1].first, ans[ans.size() - 1].second);
    }
  }
}

int main() {
  int nn;
  std::cin >> nn;
  std::vector<std::string> ss(nn);
  for (int i = 0; i < nn; ++i) {
    std::cin >> ss[i];
  }
  Trie trie(ss);
  std::vector<std::pair<int, int>> ans;
  for (int i = 0; i < nn; ++i) {
    std::string cur_s = ss[i];
    FindPalindromes(cur_s, trie, ans, i);
  }
  std::cout << ans.size() << "\n";
  for (size_t i = 0; i < ans.size(); ++i) {
    std::cout << ans[i].first + 1 << " " << ans[i].second + 1 << "\n";
  }
}