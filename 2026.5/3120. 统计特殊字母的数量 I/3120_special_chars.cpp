#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

class Solution {
public:
    /**
     * 统计特殊字母的数量
     *
     * 特殊字母定义：如果 word 中同时存在某个字母的小写形式和大写形式，
     * 则称这个字母为特殊字母。
     *
     * @param word 输入字符串
     * @return 特殊字母的数量
     */
    int numberOfSpecialChars(string word) {
        // 使用 unordered_set 记录出现的小写字母和大写字母
        unordered_set<char> lower_set;
        unordered_set<char> upper_set;

        // 遍历字符串，统计大小写字母
        for (char c : word) {
            if (islower(c)) {
                lower_set.insert(c);
            } else if (isupper(c)) {
                upper_set.insert(c);
            }
        }

        // 统计同时存在大小写的字母
        int count = 0;
        for (char c : lower_set) {
            if (upper_set.count(toupper(c))) {
                count++;
            }
        }

        return count;
    }
};

// 测试函数
int main() {
    Solution solution;

    // 测试用例
    vector<pair<string, int>> test_cases = {
        {"aaAbcBC", 3},      // 特殊字母: a, b, c
        {"abc", 0},           // 无特殊字母
        {"ABC", 0},           // 无特殊字母
        {"aA", 1},            // 特殊字母: a
        {"", 0},              // 空字符串
        {"aAbBcCdD", 4},      // 特殊字母: a, b, c, d
    };

    // 运行测试
    for (const auto& test : test_cases) {
        string word = test.first;
        int expected = test.second;
        int result = solution.numberOfSpecialChars(word);

        string status = (result == expected) ? "PASS" : "FAIL";
        cout << "输入: '" << word << "' -> 输出: " << result
             << " (期望: " << expected << ") " << status << endl;
    }

    return 0;
}
