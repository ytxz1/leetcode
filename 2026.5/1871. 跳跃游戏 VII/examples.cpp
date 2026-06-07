/*
1871. 跳跃游戏 VII

给你一个下标从 0 开始的二进制字符串 s 和两个整数 minJump 和 maxJump 。
一开始，你在下标 0 处，且该位置的值一定为 '0' 。
当同时满足如下条件时，你可以从下标 i 移动到下标 j 处：

i + minJump <= j <= min(i + maxJump, s.length - 1) 且
s[j] == '0'.

如果你可以到达 s 的下标 s.length - 1 处，请你返回 true ，否则返回 false 。

示例 1：
输入：s = "011010", minJump = 2, maxJump = 3
输出：true
解释：从下标 0 出发，可以按 0 → 3 → 5 的路径到达下标 5。

示例 2：
输入：s = "01101110", minJump = 2, maxJump = 3
输出：false
*/

#include <string>
#include <vector>
#include <queue>
using namespace std;

// 方法一：DP + 前缀和优化
// 思路：dp[i] 表示能否到达下标 i，用前缀和快速判断 [i-maxJump, i-minJump] 范围内是否有可达点

class Solution {
public:
    bool canReach(string s, int minJump, int maxJump) {
        int n = s.size();
        if (s[n - 1] == '1') return false;

        // dp[i] = true 表示可以到达下标 i
        vector<bool> dp(n, false);
        dp[0] = true;

        // prefix[i] = dp[0] + dp[1] + ... + dp[i-1]
        // 用于快速查询 [left, right] 范围内是否有可达点
        vector<int> prefix(n + 1, 0);
        prefix[1] = 1;  // dp[0] = true

        for (int i = 1; i < n; i++) {
            if (s[i] == '0') {
                // 能到达 i 的来源范围：[i-maxJump, i-minJump]
                int left = max(0, i - maxJump);
                int right = i - minJump;

                if (right >= 0) {
                    // 用前缀和判断 [left, right] 范围内是否有可达点
                    int count = prefix[right + 1] - prefix[left];
                    dp[i] = (count > 0);
                }
            }
            // 更新前缀和
            prefix[i + 1] = prefix[i] + (dp[i] ? 1 : 0);
        }

        return dp[n - 1];
    }
};

// 方法二：BFS + 滑动窗口优化
// 思路：用 BFS 遍历所有可达位置，用变量记录已处理的最大位置避免重复遍历

class Solution2 {
public:
    bool canReach(string s, int minJump, int maxJump) {
        int n = s.size();
        if (s[n - 1] == '1') return false;

        queue<int> q;
        q.push(0);

        int maxReach = 0;  // 记录已处理的最大位置，避免重复遍历

        while (!q.empty()) {
            int i = q.front();
            q.pop();

            // 从 i 能跳到的范围：[i+minJump, i+maxJump]
            int start = max(i + minJump, maxReach + 1);
            int end = min(i + maxJump, n - 1);

            for (int j = start; j <= end; j++) {
                if (s[j] == '0') {
                    if (j == n - 1) return true;
                    q.push(j);
                }
            }

            maxReach = end;  // 更新已处理的最大位置
        }

        return false;
    }
};
