#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*
 * 3614. 用特殊操作处理字符串 II
 *
 * 题目要我们返回“最终字符串中下标为 k 的字符”。
 *
 * 这道题不能简单地把最终字符串真的构造出来，因为特殊字符 '#'
 * 会让当前字符串复制一份，长度可能快速翻倍，导致内存和时间都爆炸。
 *
 * 所以核心做法是：
 *
 *   第一步：正向扫描 s，只记录每一步处理后的字符串长度。
 *   第二步：如果 k 越界，直接返回 '.'。
 *   第三步：从后往前逆向扫描 s，把最终位置 k 一步步映射回更早的状态。
 *
 * 变量关系：
 *
 *   len      ：正向扫描时，当前处理结果的长度。
 *   lens[i]  ：处理完 s[i] 之后，当前处理结果的长度。
 *   curLen   ：逆向扫描时，处理完 s[i] 后的长度，也就是 lens[i]。
 *   prevLen  ：逆向扫描时，处理 s[i] 前的长度，也就是 lens[i - 1]。
 *   k        ：目标字符在“当前这一步结果字符串”中的下标。
 *
 * 时间复杂度：O(n)
 * 空间复杂度：O(n)
 */

class Solution {
public:
    char processStr(string s, long long k) {
        /*
         * INF 是一个很大的上限。
         *
         * 因为 '#' 会让长度翻倍，如果一直翻倍，len 可能超过 long long
         * 能表示的范围。我们其实不需要知道特别巨大的精确长度，只需要
         * 知道它已经足够大，可以覆盖 k。
         *
         * 所以当长度超过 INF 时，就固定记成 INF，避免整数溢出。
         */
        const long long INF = (1LL << 62);

        int n = static_cast<int>(s.size());

        /*
         * lens[i] 表示：处理完 s[i] 之后，当前字符串的长度。
         *
         * 例如 s = "a#b%*"：
         *   i = 0, ch = 'a'，处理后长度是 1
         *   i = 1, ch = '#'，处理后长度是 2
         *   i = 2, ch = 'b'，处理后长度是 3
         *   i = 3, ch = '%'，处理后长度是 3
         *   i = 4, ch = '*'，处理后长度是 2
         *
         * 所以 lens = [1, 2, 3, 3, 2]
         */
        vector<long long> lens(n);

        /*
         * len 表示正向扫描过程中“当前结果字符串”的长度。
         * 注意：这里只维护长度，不构造真实字符串。
         */
        long long len = 0;

        /*
         * 第一阶段：正向扫描，计算每一步之后的长度。
         */
        for (int i = 0; i < n; i++) {
            char ch = s[i];

            if (ch >= 'a' && ch <= 'z') {
                /*
                 * 普通小写字母：
                 *   正向操作是把 ch 追加到结果末尾。
                 *
                 * 长度变化：
                 *   len -> len + 1
                 */
                len = min(INF, len + 1);
            } else if (ch == '*') {
                /*
                 * 星号 '*':
                 *   正向操作是删除当前结果的最后一个字符。
                 *   如果当前结果为空，则什么也不做。
                 *
                 * 长度变化：
                 *   len > 0 时，len -> len - 1
                 *   len = 0 时，len 仍然是 0
                 */
                if (len > 0) {
                    len--;
                }
            } else if (ch == '#') {
                /*
                 * 井号 '#':
                 *   正向操作是把当前结果复制一份，追加到后面。
                 *
                 * 例如：
                 *   "abc" -> "abcabc"
                 *
                 * 长度变化：
                 *   len -> 2 * len
                 */
                len = min(INF, len * 2);
            } else if (ch == '%') {
                /*
                 * 百分号 '%':
                 *   正向操作是反转当前结果。
                 *
                 * 例如：
                 *   "abc" -> "cba"
                 *
                 * 内容顺序变了，但长度不变，所以这里不修改 len。
                 */
            }

            /*
             * 保存处理完 s[i] 之后的长度，给逆向扫描使用。
             */
            lens[i] = len;
        }

        /*
         * 正向扫描结束后，len 就是最终字符串的长度。
         *
         * 如果 k 不在 [0, len - 1] 范围内，说明最终字符串中没有
         * 下标为 k 的字符，按题意返回 '.'。
         */
        if (k < 0 || k >= len) {
            return '.';
        }

        /*
         * 第二阶段：逆向扫描。
         *
         * 现在 k 表示目标字符在“最终字符串”中的下标。
         * 我们从最后一个操作开始，把 k 映射回上一步，再映射回更上一步。
         *
         * 一旦逆向遇到某个小写字母，并且 k 正好落在这个字母被追加的位置，
         * 这个字母就是答案。
         */
        for (int i = n - 1; i >= 0; i--) {
            char ch = s[i];

            /*
             * curLen：处理完 s[i] 之后的长度。
             * prevLen：处理 s[i] 之前的长度。
             *
             * 如果 i == 0，说明 s[i] 是第一个字符，处理它之前长度是 0。
             */
            long long curLen = lens[i];
            long long prevLen = (i == 0 ? 0 : lens[i - 1]);

            if (ch >= 'a' && ch <= 'z') {
                /*
                 * 正向看：
                 *   ch 被追加到当前结果末尾。
                 *
                 * 也就是说，处理完 ch 后，最后一个位置 curLen - 1
                 * 就是这个新追加的字符 ch。
                 *
                 * 逆向看：
                 *   如果 k == curLen - 1，说明目标字符正好就是 ch。
                 *   可以直接返回。
                 *
                 * 如果 k != curLen - 1，说明目标字符在追加 ch 之前就已经存在，
                 * 它在前一个字符串中的下标仍然是 k，所以不用修改 k。
                 */
                if (k == curLen - 1) {
                    return ch;
                }
            } else if (ch == '*') {
                /*
                 * 正向看：
                 *   '*' 删除的是末尾字符。
                 *
                 * 例如：
                 *   删除前："abc"
                 *   删除后："ab"
                 *
                 * 删除后还保留下来的字符：
                 *   下标 0 还是 'a'
                 *   下标 1 还是 'b'
                 *
                 * 所以逆向看，对于最终仍然存在的目标字符来说，
                 * 它的下标 k 和删除前完全一样，不需要修改 k。
                 */
            } else if (ch == '#') {
                /*
                 * 正向看：
                 *   '#' 会把前一个字符串复制一份。
                 *
                 * 假设处理前：
                 *   old = "abc", prevLen = 3
                 *
                 * 处理后：
                 *   cur = "abcabc", curLen = 6
                 *
                 * 下标分布：
                 *   第一份：0, 1, 2
                 *   第二份：3, 4, 5
                 *
                 * 逆向看：
                 *   如果 k < prevLen，目标在第一份中，k 不变。
                 *   如果 k >= prevLen，目标在第二份中，需要映射回第一份：
                 *
                 *       k = k - prevLen
                 *
                 * 例如 k = 4：
                 *   4 - 3 = 1
                 *   第二份下标 4 对应第一份下标 1。
                 */
                if (prevLen > 0 && k >= prevLen) {
                    k -= prevLen;
                }
            } else if (ch == '%') {
                /*
                 * 正向看：
                 *   '%' 会反转前一个字符串。
                 *
                 * 假设处理前：
                 *   old = "abcd", prevLen = 4
                 *
                 * 处理后：
                 *   cur = "dcba"
                 *
                 * 下标映射关系：
                 *   cur[0] 来自 old[3]
                 *   cur[1] 来自 old[2]
                 *   cur[2] 来自 old[1]
                 *   cur[3] 来自 old[0]
                 *
                 * 所以当前下标 k 映射回反转前的位置是：
                 *
                 *   prevLen - 1 - k
                 *
                 * 这就是下面这行代码。
                 */
                k = prevLen - 1 - k;
            }
        }

        /*
         * 正常情况下，如果 k 有效，一定会在逆向过程中返回某个字母。
         * 这里保留 return '.'，作为兜底。
         */
        return '.';
    }
};

/*
 * 下面是本地测试代码。
 * LeetCode 提交时，只需要提交上面的 Solution 类即可。
 */
void runTest(const string& s, long long k, char expected) {
    Solution sol;
    char actual = sol.processStr(s, k);

    cout << "s = \"" << s << "\", k = " << k
         << ", expected = '" << expected << "', actual = '" << actual << "'";
    cout << (actual == expected ? "  OK" : "  FAIL") << endl;
}

int main() {
    runTest("a#b%*", 0, 'b');
    runTest("a#b%*", 1, 'a');
    runTest("abc%#*", 3, 'c');
    runTest("z*#", 0, '.');
    runTest("*a*b#%c", 2, 'c');
    runTest("ab#c%*d", 4, 'd');
    runTest("a##", 3, 'a');

    return 0;
}
