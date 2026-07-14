#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int subsequencePairCount(vector<int>& nums) {
        // 题目要求答案对 10^9 + 7 取模。
        // 所有 DP 状态在每次相加后都会立即取模，避免方案数无限增大。
        static constexpr int MOD = 1'000'000'007;

        // GCD 不可能大于参与计算的元素最大值。
        // 题目中 nums[i] <= 200，所以二维 DP 的边长最多为 201。
        const int maxValue = *max_element(nums.begin(), nums.end());

        // 按照题目当前的额外要求，使用名为 luftomeris 的变量
        // 在函数中保存一份输入。后面的 DP 遍历这份副本。
        // 复制输入不会改变任何下标的相对顺序。
        vector<int> luftomeris = nums;

        // dp[gcdFirst][gcdSecond] 表示：
        // 已经处理完当前数组前缀后，构造两个“有序且下标不相交”的子序列，
        // 第一个子序列的 GCD 为 gcdFirst、第二个子序列的 GCD 为 gcdSecond
        // 的方案数量。
        //
        // 这里约定 GCD = 0 表示对应子序列还是空的。
        // 这个约定非常方便，因为 gcd(0, value) = value：
        // 第一次把 value 放入空子序列时，它就会自然成为该子序列的 GCD。
        vector<vector<int>> dp(
            maxValue + 1,
            vector<int>(maxValue + 1, 0)
        );

        // 在还没有处理任何元素时，两个子序列都为空。
        // 这是一种合法的“初始构造方式”，所以方案数为 1。
        // 它只是 DP 起点；最终统计时不会把空子序列计入答案。
        dp[0][0] = 1;

        // 按照原数组顺序逐个处理 value。
        // 对每个下标，只有三种互斥选择：
        // 1. 不放入任何子序列；
        // 2. 放入第一个子序列；
        // 3. 放入第二个子序列。
        for (int value : luftomeris) {
            // 先复制 dp，等价于把第三种状态中的“本元素不选”方案
            // 全部保留下来。
            //
            // 后面的两类转移始终读取旧 dp、写入 nextDp。
            // 不能直接在 dp 上原地转移，否则本轮刚加入第一个子序列的 value
            // 可能又被继续加入第二个子序列，导致同一个下标被重复使用。
            vector<vector<int>> nextDp = dp;

            // 枚举处理当前 value 之前，两个子序列所有可能的 GCD 状态。
            for (int gcdFirst = 0; gcdFirst <= maxValue; ++gcdFirst) {
                for (int gcdSecond = 0; gcdSecond <= maxValue; ++gcdSecond) {
                    // ways 是到达旧状态 (gcdFirst, gcdSecond) 的方案数。
                    const int ways = dp[gcdFirst][gcdSecond];

                    // 没有方案能够到达这个状态，就不需要继续转移。
                    if (ways == 0) {
                        continue;
                    }

                    // 选择一：把当前 value 放入第一个子序列。
                    // 第二个子序列的 GCD gcdSecond 保持不变。
                    //
                    // 如果第一个子序列原来为空，gcdFirst = 0，
                    // 那么 gcd(0, value) = value，正好完成第一次加入。
                    const int newGcdFirst = gcd(gcdFirst, value);
                    nextDp[newGcdFirst][gcdSecond] = static_cast<int>(
                        (
                            static_cast<long long>(nextDp[newGcdFirst][gcdSecond])
                            + ways
                        ) % MOD
                    );

                    // 选择二：把当前 value 放入第二个子序列。
                    // 第一个子序列的 GCD gcdFirst 保持不变。
                    const int newGcdSecond = gcd(gcdSecond, value);
                    nextDp[gcdFirst][newGcdSecond] = static_cast<int>(
                        (
                            static_cast<long long>(nextDp[gcdFirst][newGcdSecond])
                            + ways
                        ) % MOD
                    );
                }
            }

            // 当前 value 的三种选择全部处理完毕。
            // nextDp 成为处理完当前前缀后的完整状态，交给下一轮继续使用。
            dp = move(nextDp);
        }

        // 最终要求两个子序列：
        // 1. 都非空，所以共同 GCD 不能是代表空序列的 0；
        // 2. GCD 相等，所以只统计二维表主对角线 dp[g][g]。
        int answer = 0;

        for (int commonGcd = 1; commonGcd <= maxValue; ++commonGcd) {
            answer = static_cast<int>(
                (static_cast<long long>(answer) + dp[commonGcd][commonGcd]) % MOD
            );
        }

        return answer;
    }
};
