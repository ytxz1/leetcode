#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> pathExistenceQueries(int n, vector<int>& nums, int maxDiff, vector<vector<int>>& queries) {
        // sortedNodes 存放 {nums[i], i}，先按节点值排序，再保留原始节点编号。
        // 原图中两个点能否连边只由 nums 值决定，所以先按值排序会让可达范围变成连续区间。
        vector<pair<int, int>> sortedNodes;
        sortedNodes.reserve(n);
        for (int node = 0; node < n; ++node) {
            sortedNodes.push_back({nums[node], node});
        }
        sort(sortedNodes.begin(), sortedNodes.end());

        // sortedValues[pos] 表示排序后位置 pos 对应的 nums 值。
        // sortedIndexOfNode[node] 表示原始节点 node 在排序数组中的位置。
        vector<int> sortedValues(n);
        vector<int> sortedIndexOfNode(n);
        for (int pos = 0; pos < n; ++pos) {
            int value = sortedNodes[pos].first;
            int node = sortedNodes[pos].second;
            sortedValues[pos] = value;
            sortedIndexOfNode[node] = pos;
        }

        // farthest[pos] 表示从排序位置 pos 出发，走 1 条边最多能到达的最右排序位置。
        //
        // 因为 sortedValues 已经非递减：
        // 能和 pos 直接连边的右侧位置满足 sortedValues[right] - sortedValues[pos] <= maxDiff。
        // 所以用 upper_bound 找到第一个 > sortedValues[pos] + maxDiff 的位置，再减 1。
        vector<int> farthest(n);
        for (int pos = 0; pos < n; ++pos) {
            int limitValue = sortedValues[pos] + maxDiff;
            int firstGreater = (int)(upper_bound(sortedValues.begin(), sortedValues.end(), limitValue) - sortedValues.begin());
            farthest[pos] = firstGreater - 1;
        }

        // LOG 要能覆盖最多 n 次跳跃。
        int LOG = 1;
        while ((1 << LOG) <= n) {
            ++LOG;
        }

        // jump[p][pos] 表示从 pos 开始，连续走 2^p 步，每步都贪心跳到当前能到的最右位置后，
        // 最终能到达的最右排序位置。
        vector<vector<int>> jump(LOG, vector<int>(n));
        jump[0] = farthest;
        for (int p = 1; p < LOG; ++p) {
            for (int pos = 0; pos < n; ++pos) {
                jump[p][pos] = jump[p - 1][jump[p - 1][pos]];
            }
        }

        vector<int> answer;
        answer.reserve(queries.size());

        for (const auto& query : queries) {
            int u = query[0];
            int v = query[1];

            // 同一个节点到自己的最短距离是 0。
            if (u == v) {
                answer.push_back(0);
                continue;
            }

            // 把原始节点编号映射到排序位置。
            int left = sortedIndexOfNode[u];
            int right = sortedIndexOfNode[v];
            if (left > right) {
                swap(left, right);
            }

            // 如果一条边就能从 left 覆盖到 right，最短距离就是 1。
            if (farthest[left] >= right) {
                answer.push_back(1);
                continue;
            }

            // 用倍增贪心地“尽量多跳但仍然到不了 right”。
            // 循环结束后，再补 1 步尝试到达 right。
            int currentPosition = left;
            int steps = 0;

            for (int p = LOG - 1; p >= 0; --p) {
                int nextPosition = jump[p][currentPosition];

                // 如果跳 2^p 步后仍然在 right 左边，就说明这些步数可以安全使用，
                // 因为我们还没有到达目标，需要继续从更靠右的位置尝试。
                if (nextPosition < right) {
                    currentPosition = nextPosition;
                    steps += (1 << p);
                }
            }

            // 如果 currentPosition 已经无法继续向右推进，却仍然没到 right，说明两个点不连通。
            if (farthest[currentPosition] == currentPosition) {
                answer.push_back(-1);
            } else {
                // 此时 steps 步后还没到 right，但再走 1 步就能到达或超过 right。
                answer.push_back(steps + 1);
            }
        }

        return answer;
    }
};
