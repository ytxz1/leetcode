#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
using namespace std;

// 方法一：使用哈希表记录首次出现和最后出现位置
// 时间复杂度: O(n)，空间复杂度: O(1)
class Solution1 {
public:
    int numberOfSpecialChars(string word) {
        // 记录每个字母首次出现的大写位置
        vector<int> firstUpper(26, -1);
        // 记录每个字母最后出现的小写位置
        vector<int> lastLower(26, -1);

        for (int i = 0; i < word.size(); i++) {
            char c = word[i];
            if (c >= 'a' && c <= 'z') {
                // 小写字母，更新最后出现位置
                lastLower[c - 'a'] = i;
            } else if (c >= 'A' && c <= 'Z') {
                // 大写字母，记录首次出现位置
                if (firstUpper[c - 'A'] == -1) {
                    firstUpper[c - 'A'] = i;
                }
            }
        }

        int count = 0;
        for (int i = 0; i < 26; i++) {
            // 必须同时存在小写和大写
            // 且最后的小写位置 < 首次的大写位置
            if (lastLower[i] != -1 && firstUpper[i] != -1 &&
                lastLower[i] < firstUpper[i]) {
                count++;
            }
        }
        return count;
    }
};

// 方法二：使用集合和位置检查
// 时间复杂度: O(n)，空间复杂度: O(1)
class Solution2 {
public:
    int numberOfSpecialChars(string word) {
        // 记录每个字母的出现位置
        vector<vector<int>> lowerPos(26);  // 小写位置
        vector<vector<int>> upperPos(26);  // 大写位置

        for (int i = 0; i < word.size(); i++) {
            char c = word[i];
            if (c >= 'a' && c <= 'z') {
                lowerPos[c - 'a'].push_back(i);
            } else if (c >= 'A' && c <= 'Z') {
                upperPos[c - 'A'].push_back(i);
            }
        }

        int count = 0;
        for (int i = 0; i < 26; i++) {
            if (lowerPos[i].empty() || upperPos[i].empty()) {
                continue;  // 必须同时存在
            }
            // 检查是否所有小写位置都在首个大写位置之前
            int firstUpper = upperPos[i][0];
            bool valid = true;
            for (int pos : lowerPos[i]) {
                if (pos >= firstUpper) {
                    valid = false;
                    break;
                }
            }
            if (valid) count++;
        }
        return count;
    }
};

// 方法三：单次遍历，状态标记
// 时间复杂度: O(n)，空间复杂度: O(1)
class Solution3 {
public:
    int numberOfSpecialChars(string word) {
        // 记录每个字母的状态
        // 0: 未出现, 1: 只出现小写, 2: 出现大写后小写(无效), 3: 有效特殊字母
        vector<int> state(26, 0);
        // 记录每个字母是否出现过大写
        vector<bool> seenUpper(26, false);

        for (char c : word) {
            if (c >= 'a' && c <= 'z') {
                int idx = c - 'a';
                if (seenUpper[idx]) {
                    // 大写之后又出现小写，无效
                    state[idx] = 2;
                } else if (state[idx] == 0) {
                    state[idx] = 1;
                }
            } else if (c >= 'A' && c <= 'Z') {
                int idx = c - 'A';
                seenUpper[idx] = true;
                if (state[idx] == 1) {
                    // 之前有小写，现在首次出现大写
                    state[idx] = 3;
                } else if (state[idx] == 0) {
                    // 只有大写
                    state[idx] = 0;
                }
            }
        }

        int count = 0;
        for (int s : state) {
            if (s == 3) count++;
        }
        return count;
    }
};

// 方法四：使用位运算优化
// 时间复杂度: O(n)，空间复杂度: O(1)
class Solution4 {
public:
    int numberOfSpecialChars(string word) {
        // 使用两个整数的位来表示26个字母
        // seenLower: 是否出现过小写
        // seenUpper: 是否出现过大写
        // validSpecial: 是否是有效的特殊字母
        int seenLower = 0, seenUpper = 0;
        int invalidAfterUpper = 0;  // 大写之后又出现小写的标记

        for (char c : word) {
            if (c >= 'a' && c <= 'z') {
                int bit = 1 << (c - 'a');
                if (seenUpper & bit) {
                    // 大写之后出现小写，标记为无效
                    invalidAfterUpper |= bit;
                }
                seenLower |= bit;
            } else if (c >= 'A' && c <= 'Z') {
                int bit = 1 << (c - 'A');
                seenUpper |= bit;
            }
        }

        // 有效特殊字母：同时出现小写和大写，且没有大写后小写
        int special = seenLower & seenUpper & ~invalidAfterUpper;

        // 计算位数
        int count = 0;
        while (special) {
            count += special & 1;
            special >>= 1;
        }
        return count;
    }
};

int main() {
    Solution1 sol1;
    Solution2 sol2;
    Solution3 sol3;
    Solution4 sol4;

    // 测试用例1
    string word1 = "aaAbcBC";
    cout << "测试用例1: " << word1 << endl;
    cout << "方法一: " << sol1.numberOfSpecialChars(word1) << endl;
    cout << "方法二: " << sol2.numberOfSpecialChars(word1) << endl;
    cout << "方法三: " << sol3.numberOfSpecialChars(word1) << endl;
    cout << "方法四: " << sol4.numberOfSpecialChars(word1) << endl;
    cout << endl;

    // 测试用例2
    string word2 = "abc";
    cout << "测试用例2: " << word2 << endl;
    cout << "方法一: " << sol1.numberOfSpecialChars(word2) << endl;
    cout << "方法二: " << sol2.numberOfSpecialChars(word2) << endl;
    cout << "方法三: " << sol3.numberOfSpecialChars(word2) << endl;
    cout << "方法四: " << sol4.numberOfSpecialChars(word2) << endl;
    cout << endl;

    // 测试用例3
    string word3 = "AbBCab";
    cout << "测试用例3: " << word3 << endl;
    cout << "方法一: " << sol1.numberOfSpecialChars(word3) << endl;
    cout << "方法二: " << sol2.numberOfSpecialChars(word3) << endl;
    cout << "方法三: " << sol3.numberOfSpecialChars(word3) << endl;
    cout << "方法四: " << sol4.numberOfSpecialChars(word3) << endl;

    return 0;
}
