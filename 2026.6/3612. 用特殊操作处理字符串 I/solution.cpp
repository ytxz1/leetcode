#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*
 * 3612. 用特殊操作处理字符串 I
 *
 * 思路：
 *   从左到右扫描字符串 s，用 ans 维护当前已经处理出来的结果。
 *
 *   1. 小写字母：追加到 ans 末尾。
 *   2. '*'：删除 ans 的最后一个字符，如果 ans 为空则不做任何事。
 *   3. '#'：把当前 ans 复制一份，追加到 ans 后面。
 *   4. '%'：反转当前 ans。
 *
 * 时间复杂度：O(n * L)
 *   n 是 s 的长度，L 是处理过程中 ans 的最大长度。
 *   其中 '#' 和 '%' 可能需要操作整段 ans。
 *
 * 空间复杂度：O(L)
 *   ans 用来保存当前结果。
 */

class Solution {
public:
    string processStr(string s) {
        string ans;

        for (char ch : s) {
            if (ch >= 'a' && ch <= 'z') {
                ans.push_back(ch);
            } else if (ch == '*') {
                if (!ans.empty()) {
                    ans.pop_back();
                }
            } else if (ch == '#') {
                ans += ans;
            } else if (ch == '%') {
                reverse(ans.begin(), ans.end());
            }
        }

        return ans;
    }
};

void runTest(const string& s, const string& expected) {
    Solution sol;
    string actual = sol.processStr(s);

    cout << "s = \"" << s << "\", expected = \"" << expected
         << "\", actual = \"" << actual << "\"";
    cout << (actual == expected ? "  OK" : "  FAIL") << endl;
}

int main() {
    runTest("a#b%*", "ba");
    runTest("z*#", "");
    runTest("abc%#*", "cbacb");
    runTest("*a*b#%c", "bbc");
    runTest("leetcode", "leetcode");
    runTest("ab#c%*d", "cbabd");

    return 0;
}
