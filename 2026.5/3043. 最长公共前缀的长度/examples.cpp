#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
using namespace std;

// 方法一：Trie（前缀树）
// 思路：将 arr1 中的数字转为字符串插入 Trie，然后用 arr2 中的数字在 Trie 中查找最长公共前缀

struct TrieNode {
    TrieNode* children[10] = {nullptr};  // 数字 0-9
};

class Solution {
public:
    int longestCommonPrefix(vector<int>& arr1, vector<int>& arr2) {
        TrieNode* root = new TrieNode();

        // 将 arr1 中所有数字插入 Trie
        for (int num : arr1) {
            string s = to_string(num);
            TrieNode* node = root;
            for (char c : s) {
                int idx = c - '0';
                if (!node->children[idx]) {
                    node->children[idx] = new TrieNode();
                }
                node = node->children[idx];
            }
        }

        // 用 arr2 中的数字在 Trie 中查找最长前缀
        int maxLen = 0;
        for (int num : arr2) {
            string s = to_string(num);
            TrieNode* node = root;
            int len = 0;
            for (char c : s) {
                int idx = c - '0';
                if (!node->children[idx]) break;  // 无法继续匹配
                node = node->children[idx];
                len++;
            }
            maxLen = max(maxLen, len);
        }

        return maxLen;
    }
};

// 方法二：哈希集合（更简洁）
// 思路：将 arr1 中所有数字的所有前缀存入集合，然后检查 arr2 中每个数字的前缀是否在集合中

class Solution2 {
public:
    int longestCommonPrefix(vector<int>& arr1, vector<int>& arr2) {
        // 将 arr1 中每个数字的所有前缀存入集合
        unordered_set<string> prefixes;
        for (int num : arr1) {
            string s = to_string(num);
            for (int i = 1; i <= s.size(); i++) {
                prefixes.insert(s.substr(0, i));
            }
        }

        // 检查 arr2 中每个数字的前缀
        int maxLen = 0;
        for (int num : arr2) {
            string s = to_string(num);
            for (int i = s.size(); i >= 1; i--) {
                if (prefixes.count(s.substr(0, i))) {
                    maxLen = max(maxLen, i);
                    break;  // 已找到该数字的最长前缀
                }
            }
        }

        return maxLen;
    }
};

// 方法三：枚举前缀 + 哈希表（全部使用 int 类型）
// 思路：用整数除法截取前缀，避免字符串转换，用 unordered_set<int> 存储前缀值
// 技巧：num / 10 不断去掉末位数字，自然从长到短枚举前缀

class Solution3 {
public:
    int longestCommonPrefix(vector<int>& arr1, vector<int>& arr2) {
        unordered_set<int> prefixes;

        // 将 arr1 中每个数字的所有前缀（整数形式）存入哈希集合
        for (int num : arr1) {
            while (num > 0) {
                prefixes.insert(num);
                num /= 10;  // 去掉末位，得到更短的前缀
            }
        }

        // 对 arr2 中每个数字，从长到短枚举前缀，检查是否在集合中
        int maxLen = 0;
        for (int num : arr2) {
            int temp = num;
            while (temp > 0) {
                if (prefixes.count(temp)) {
                    // 计算当前前缀的位数
                    int digits = 0;
                    int t = temp;
                    while (t > 0) {
                        digits++;
                        t /= 10;
                    }
                    maxLen = max(maxLen, digits);
                    break;  // 已找到该数字的最长匹配前缀
                }
                temp /= 10;  // 去掉末位，尝试更短的前缀
            }
        }

        return maxLen;
    }
};
