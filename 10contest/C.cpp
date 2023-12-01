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

  std::vector<Node> trie;

  Trie(const std::vector<String>& dict) : trie(1, Node()) {
    for (size_t i = 0; i < dict.size(); ++i) {
      Add(dict[i], i);
    }
  }

  void Add(const String& str, size_t num) {
    int cur_v = 0;
    for (size_t i = 0; i < str.size(); ++i) {
      if (trie[cur_v].to[str[i] - 'a'] == -1) {
        trie[cur_v].to[str[i] - 'a'] = trie.size();
        trie.push_back(Node());
      }
      cur_v = trie[cur_v].to[str[i] - 'a'];
    }
    trie[cur_v].term = true;
    trie[cur_v].str.push_back(num);
  }

  size_t Size() const { return trie.size(); }
};

struct AhoCorasick {
  Trie bor;

  struct AhoNode {
    int link;
    int dp;
    int compressed;
  };

  std::vector<AhoNode> links;

  std::vector<std::vector<int>> go;

  AhoCorasick(const std::vector<String>& dict)
      : bor(dict), links(bor.Size()), go(bor.Size(), std::vector<int>(kAlph)) {
    CalcLinks();
  }

  void CalcLinks() {
    InitRoot();
    std::queue<int> verts_queue;
    verts_queue.push(0);
    while (!verts_queue.empty()) {
      int cur_v = verts_queue.front();
      verts_queue.pop();
      for (size_t cc = 0; cc < kAlph; ++cc) {
        int uu = bor.trie[cur_v].to[cc];
        if (uu == -1) {
          continue;
        }

        if (cur_v == 0) {
          links[uu].link = 0;
        } else {
          links[uu].link = go[links[cur_v].link][cc];
        }

        for (size_t dd = 0; dd < kAlph; ++dd) {
          if (bor.trie[uu].to[dd] != -1) {
            go[uu][dd] = bor.trie[uu].to[dd];
          } else {
            go[uu][dd] = go[links[uu].link][dd];
          }
        }

        links[uu].dp = links[links[uu].link].dp;
        if (bor.trie[uu].term) {
          ++links[uu].dp;
        }

        if (bor.trie[links[uu].link].term) {
          links[uu].compressed = links[uu].link;
        } else {
          links[uu].compressed = links[links[uu].link].compressed;
        }

        verts_queue.push(uu);
      }
    }
  }

  void InitRoot() {
    links[0].link = -1;
    for (size_t cc = 0; cc < kAlph; ++cc) {
      if (bor.trie[0].to[cc] == -1) {
        go[0][cc] = 0;
      } else {
        go[0][cc] = bor.trie[0].to[cc];
      }
    }
    links[0].dp = 0;
    links[0].compressed = 0;
  }

  std::vector<std::vector<int>> FindInclusions(const std::string& text,
                                               std::vector<std::string>& dict) {
    std::vector<std::vector<int>> ans(dict.size());
    int cur_v = 0;
    for (size_t i = 0; i < text.size(); ++i) {
      cur_v = go[cur_v][text[i] - 'a'];
      int uu = cur_v;
      if (!bor.trie[uu].term) {
        uu = links[uu].compressed;
      }
      while (uu != 0) {
        for (auto str_i : bor.trie[uu].str) {
          ans[str_i].push_back(i + 1 - dict[str_i].size() + 1);
        }
        uu = links[uu].compressed;
      }
    }
    return ans;
  }
};

int main() {
  String text;
  std::cin >> text;
  int nn;
  std::cin >> nn;
  std::vector<String> dict(nn);
  for (int i = 0; i < nn; ++i) {
    std::cin >> dict[i];
  }
  AhoCorasick ac(dict);
  std::vector<std::vector<int>> ans = ac.FindInclusions(text, dict);
  for (size_t i = 0; i < ans.size(); ++i) {
    std::cout << ans[i].size() << " ";
    for (size_t j = 0; j < ans[i].size(); ++j) {
      std::cout << ans[i][j] << " ";
    }
    std::cout << "\n";
  }
}