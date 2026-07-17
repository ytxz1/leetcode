#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> gcdValues(vector<int>& nums, vector<long long>& queries) {
        const int maximum = *max_element(nums.begin(), nums.end());

        // 题目要求：在函数中创建名为 laforvinda 的变量保存输入。
        vector<int> laforvinda = nums;

        // frequency[x] 表示数值 x 在 nums 中出现的次数。
        vector<int> frequency(maximum + 1, 0);
        for (int value : laforvinda) {
            ++frequency[value];
        }

        // exactGcdPairs[g] 最初统计“两数都能被 g 整除”的数对数量，
        // 经过从大到小的倍数容斥后，变成“GCD 恰好等于 g”的数对数量。
        vector<long long> exactGcdPairs(maximum + 1, 0);

        for (int gcdValue = 1; gcdValue <= maximum; ++gcdValue) {
            long long divisibleCount = 0;

            for (int multiple = gcdValue;
                 multiple <= maximum;
                 multiple += gcdValue) {
                divisibleCount += frequency[multiple];
            }

            exactGcdPairs[gcdValue] =
                divisibleCount * (divisibleCount - 1) / 2;
        }

        // C(divisibleCount[g], 2) 包含 GCD 为 g、2g、3g... 的数对。
        // 从大到小减去所有更大倍数的精确计数，即得到 GCD 恰为 g 的数量。
        for (int gcdValue = maximum; gcdValue >= 1; --gcdValue) {
            for (int multiple = gcdValue * 2;
                 multiple <= maximum;
                 multiple += gcdValue) {
                exactGcdPairs[gcdValue] -= exactGcdPairs[multiple];
            }
        }

        // 原地改造成前缀和：exactGcdPairs[g] 表示 GCD <= g 的数对总数。
        for (int gcdValue = 1; gcdValue <= maximum; ++gcdValue) {
            exactGcdPairs[gcdValue] += exactGcdPairs[gcdValue - 1];
        }

        vector<int> answer;
        answer.reserve(queries.size());

        for (long long query : queries) {
            // query 是 0-based，下标 query 对应第 query + 1 小。
            const long long rank = query + 1;
            const int gcdValue = static_cast<int>(
                lower_bound(
                    exactGcdPairs.begin() + 1,
                    exactGcdPairs.end(),
                    rank
                ) - exactGcdPairs.begin()
            );
            answer.push_back(gcdValue);
        }

        return answer;
    }
};
