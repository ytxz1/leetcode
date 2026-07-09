#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> sumAndMultiply(string s, vector<vector<int>>& queries) {
        const long long MOD = 1000000007LL;
        int n = (int)s.size();

        // nonZeroCountPrefix[i] 表示 s[0..i-1] 中非零数字的个数。
        // 有了它，就能把原串 s 的区间 [l, r] 映射到压缩数组 tDigits 的区间。
        vector<int> nonZeroCountPrefix(n + 1, 0);

        // digitSumPrefix[i] 表示 s[0..i-1] 的数字和。
        // 0 对数字和没有贡献，所以它也等于这些位置中非零数字的数字和。
        vector<int> digitSumPrefix(n + 1, 0);

        // tDigits 存放 s 中所有非零数字，保持原来的出现顺序。
        // 例如 s = "10203004"，则 tDigits = [1, 2, 3, 4]。
        vector<int> tDigits;

        for (int i = 0; i < n; ++i) {
            int digit = s[i] - '0';

            nonZeroCountPrefix[i + 1] = nonZeroCountPrefix[i];
            digitSumPrefix[i + 1] = digitSumPrefix[i] + digit;

            if (digit != 0) {
                ++nonZeroCountPrefix[i + 1];
                tDigits.push_back(digit);
            }
        }

        int k = (int)tDigits.size();

        // pow10[len] = 10^len % MOD。
        // 从一个十进制前缀中扣掉左边部分时，需要乘 10^len 来对齐位数。
        vector<long long> pow10(k + 1, 1);
        for (int len = 1; len <= k; ++len) {
            pow10[len] = pow10[len - 1] * 10 % MOD;
        }

        // valuePrefix[i] 表示 tDigits[0..i-1] 连接成的整数，对 MOD 取余。
        // 例如 tDigits = [1, 2, 3, 4]：
        // valuePrefix = [0, 1, 12, 123, 1234]。
        vector<long long> valuePrefix(k + 1, 0);
        for (int i = 0; i < k; ++i) {
            valuePrefix[i + 1] = (valuePrefix[i] * 10 + tDigits[i]) % MOD;
        }

        vector<int> answer;
        answer.reserve(queries.size());

        for (const auto& query : queries) {
            int l = query[0];
            int r = query[1];

            // s[l..r] 之前已经出现了 left 个非零数字。
            // 所以 s[l..r] 中第一个非零数字，在 tDigits 中的下标就是 left。
            int left = nonZeroCountPrefix[l];

            // s[0..r] 一共出现了 right 个非零数字。
            // 因此 s[l..r] 对应 tDigits 的半开区间 [left, right)。
            int right = nonZeroCountPrefix[r + 1];

            // nonZeroLen 是本次查询子串中非零数字的个数，
            // 也就是连接出的整数 x 的十进制位数；如果为 0，则 x = 0。
            int nonZeroLen = right - left;

            // digitSum 是 x 的数字和。
            // 因为 x 由 s[l..r] 的非零数字组成，所以可用原串数字和前缀直接相减。
            long long digitSum = digitSumPrefix[r + 1] - digitSumPrefix[l];

            // 计算 x：
            // valuePrefix[right] 是 tDigits[0..right-1] 形成的数；
            // valuePrefix[left] 是 tDigits[0..left-1] 形成的数。
            // 把左边前缀乘上 10^nonZeroLen 后扣掉，就剩 tDigits[left..right-1]。
            long long x = (valuePrefix[right] - valuePrefix[left] * pow10[nonZeroLen]) % MOD;

            // C++ 里负数取模仍可能是负数，所以补一次 MOD，让 x 落在 [0, MOD)。
            if (x < 0) {
                x += MOD;
            }

            // 题目要求返回 x * digitSum，并对 MOD 取余。
            answer.push_back((int)(x * digitSum % MOD));
        }

        return answer;
    }
};
